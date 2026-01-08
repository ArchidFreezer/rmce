#pragma once
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "GameRuleData.h"

/**
 * @class GameRuleDataCache
 * @brief Class to store game rule data, such as profession definitions, spell lists, etc.
 * 
 * The class is thread safe and can cache any object type that is derived from the polymorphic #GameRuleData class
 * 
 * The game rule data that is stored must be passed as a unique_ptr which ensures that there is only a single copy and it
 * is the cache that owns the object. This ensures that all objects are correctly
 */
class GameRuleDataCache {
public:
	/**
	 * @brief Default constructor
	 */
	GameRuleDataCache() = default;
	/**
	 * @brief Default destructor
	 */
	~GameRuleDataCache() = default;

	/**
	 * @brief Copy constructor
	 * 
	 * This is removed as there should one be a single cache instance
	 * 
	 * @param other GameRuleDataCache to copy
	 */
	GameRuleDataCache(const GameRuleDataCache& other) = delete;
	/**
	 * @brief Assignment operator
	 * 
	 * This is removed as there should one be a single cache instance
	 * 
	 * @param other GameRuleDataCache to assign
	 * @return New cache object
	 */
	GameRuleDataCache& operator=(const GameRuleDataCache& other) = delete;

	/**
	 * @brief Move constructor
	 *
	 * This is removed as there should one be a single cache instance
	 *
	 * @param other GameRuleDataCache to copy
	 */
	GameRuleDataCache(GameRuleDataCache&& other) = delete;
	/**
	 * @brief Move assignment operator
	 *
	 * This is removed as there should one be a single cache instance
	 *
	 * @param other GameRuleDataCache to assign
	 * @return New cache object
	 */
	GameRuleDataCache& operator=(GameRuleDataCache&& other) = delete;

	/**
	 * @brief Get the rule data object with the given id
	 * 
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from #GameRuleData
	 * @param id Id of the object being retrieved
	 * @return Reference to the data object
	 * @throws out_of_range if there is no rule data for the id with the given type
	 */
	template <class T>
	T& get(std::string& id);

	/**
	 * @brief Move rule data object to the cache
	 * 
	 * This changes ownership of the rule data to the cache
	 * 
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from #GameRuleData
	 * @param datum Object to move to the cache
	 * @param id Identifier of the data object
	 */
	template <class T>
	void add(std::unique_ptr <T> datum, std::string& id);

	/**
	 * @brief Check if data object exists in the cache
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from #GameRuleData
	 * @param id Identifier of the data object
	 * @return Reference to the data object
	 */
	template <class T>
	bool exists(std::string& id);

	/**
	 * @brief Populates a set with all the rule data ids for a specific rule type
	 * 
	 * The parameter is first erased and then populated with the key data so following the call it will only contain the ids
	 * 
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from #GameRuleData
	 * @param keys Set of strings to populate with the ids of the data objects
	 */
	template <class T>
	void keys(std::set<std::string>& keys);
	
private:
	/**
	 * @brief Stores the state of the cache
	 */
	std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<GameRuleData>>> state;
	/**
	 * @brief Stores mutexes to make the class threads safe
	 */
	std::unordered_map<std::type_index, std::mutex> mutexes;
};

template<class T>
inline T& GameRuleDataCache::get(std::string& id)
{
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	// Grab a mutex for thread safety
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
	else {
		throw std::out_of_range("There is no rule data with id " + id);
	}
}

template<class T>
inline void GameRuleDataCache::add(std::unique_ptr <T> datum, std::string& id)
{
	// Check we are requesting an appropriate class
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	// Grab a mutex for thread safety
	auto& mutex = mutexes[typeid(T)];
	std::lock_guard<std::mutex> guard(mutex);

	// Grab a reference to the hash map for the particular type of data we want
	auto& ruledata_hash_map = state[typeid(T)];

	// Add the new rule data to our cache
	ruledata_hash_map.insert_or_assign(id, std::move(datum));
}

template<class T>
inline bool GameRuleDataCache::exists(std::string& id)
{
	// Check we are requesting an appropriate class
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	// Grab a mutex for thread safety
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
inline void GameRuleDataCache::keys(std::set<std::string>& keys)
{
	// Check we are requesting an appropriate class
	static_assert(std::is_base_of<GameRuleData, T>::value, "T must be derived from GameRuleData");

	// Grab a mutex for thread safety
	auto& mutex = mutexes[typeid(T)];
	std::lock_guard<std::mutex> guard(mutex);

	// Grab a reference to the hash map for the particular type of data we want
	auto& ruledata_hash_map = state[typeid(T)];

	// Empty the set before we start so we ensure that it only contains the keys
	keys.clear();

	// Iterate through the map keys and add them to the set
	for (auto it = ruledata_hash_map.begin(); it != ruledata_hash_map.end(); ++it) {
		keys.insert(it->first);
	}
}
