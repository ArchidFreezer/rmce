#pragma once
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "GameRuleData.h"

// Class that caches static game rule data, such as professions, spell lists, etc.
// The class can cache any object type that is derived from the polymorphic GameRuleData class.
// The class is thread safe 
class GameRuleDataCache {
public:
	GameRuleDataCache() = default;
	~GameRuleDataCache() = default;

	// Remove the copy constuctor as there should one be a single cache instance
	GameRuleDataCache(const GameRuleDataCache&) = delete;
	// Remove the assignment operator as there should one be a single cache instance
	GameRuleDataCache& operator=(const GameRuleDataCache&) = delete;

	// Remove the move constuctor as it should never need to be moved
	GameRuleDataCache(GameRuleDataCache&&) = delete;
	// Remove the assignment operator as it should never need to be moved
	GameRuleDataCache& operator=(GameRuleDataCache&&) = delete;

	// Get the rule data with the given ID
	// If the data is not found a new empty rule is created and added to the cache, this
	// is probably not what is wanted so it is typically better to test if the data exists
	// prior to calling this function
	// TODO: Create and throw an exception if the data is not found
	template <class T>
	T& GetRuleData(std::string& id);

	// Move rule data to the cache
	// This changes ownership of the rule data to the cache
	template <class T>
	void AddRuleData(std::unique_ptr <T> datum, std::string& id);

	// Check if data exists in the cache
	template <class T>
	bool RuleDataExists(std::string& id);

	// Gets a list of all the rule data ids for a specific rule type
	template <class T>
	std::set<std::string> GetRuleDataIds();
	
private:
	// Store the state of the cache
	// This is made static so that all instances of the cache are reading the same data
	std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<GameRuleData>>> state;
	// Store mutexes to make the class threads safe
	std::unordered_map<std::type_index, std::mutex> mutexes;
};

template<class T>
inline T& GameRuleDataCache::GetRuleData(std::string& id)
{
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	auto& mutex = mutexes[typeid(T)];
	std::lock_guard<std::mutex> guard(mutex);

	// Grab a reference to the hash map for the particular type of data we want
	auto& ruledata_hash_map = state[typeid(T)];

	// Search the resource hash map for the existance of the requested resource
	auto ruledata_it = ruledata_hash_map.find(id);

	// If resource is found, then we return it as a reference
	if (ruledata_it != ruledata_hash_map.end())	{
		return *dynamic_cast<T*>(ruledata_it->second.get());
	}

	// Otherwise if the resource was not found, we create a dummy value
	auto new_ruledata = std::make_unique<T>(id);

	// Move it to the hash map so that the cache owns it
	auto ret = ruledata_hash_map.emplace(id, std::move(new_ruledata));

	// We return it as a reference
	return *dynamic_cast<T*>(ret.first->second.get());
}

template<class T>
inline void GameRuleDataCache::AddRuleData(std::unique_ptr <T> datum, std::string& id)
{
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	auto& mutex = mutexes[typeid(T)];
	std::lock_guard<std::mutex> guard(mutex);

	// Grab a reference to the hash map for the particular type of data we want
	auto& ruledata_hash_map = state[typeid(T)];

	// Add the new rule data to our cache
	ruledata_hash_map.insert_or_assign(id, std::move(datum));
}

template<class T>
inline bool GameRuleDataCache::RuleDataExists(std::string& id)
{
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	auto& mutex = mutexes[typeid(T)];
	std::lock_guard<std::mutex> guard(mutex);
		
	// Grab a reference to the hash map for the particular type of data we want
	auto& ruledata_hash_map = state[typeid(T)];

	// Search the data hash map for the existance of the requested data
	auto ruledata_it = ruledata_hash_map.find(id);

	// Return if the data is found or not
	return ruledata_it != ruledata_hash_map.end();
}

template<class T>
inline std::set<std::string> GameRuleDataCache::GetRuleDataIds()
{
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	auto& mutex = mutexes[typeid(T)];
	std::lock_guard<std::mutex> guard(mutex);

	std::set<std::string> keys{};

	// Grab a reference to the hash map for the particular type of data we want
	auto& ruledata_hash_map = state[typeid(T)];

	for (auto it = ruledata_hash_map.begin(); it != ruledata_hash_map.end(); ++it) {
		keys.insert(it->first);
	}

	return keys;
}
