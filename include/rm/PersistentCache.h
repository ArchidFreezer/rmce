#pragma once
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>

#include <Persistent.h>

namespace rm {

	/**
	 * @class PersistentCache
	 * @brief Class to persistently store game objects allowing other objects to safely store pointers.
	 *
	 * Because the game objects are stored in the cache, other objects can store pointers to them without worrying about ownership and lifetime issues. This allows for more efficient memory usage and easier management of game objects.
	 *
	 * The class is thread safe and can cache any object type that is derived from the polymorphic Persistent class
	 *
	 * The game object that is stored must be passed as a unique_ptr which ensures that there is only a single copy and the cache retains ownership.
	 */
	class PersistentCache {
	public:
		/**
		 * @brief Default constructor
		 */
		PersistentCache() = default;
		/**
		 * @brief Default destructor
		 */
		~PersistentCache() = default;

		/**
		 * @brief Copy constructor
		 *
		 * This is removed as there should one be a single cache instance
		 *
		 * @param other PersistentCache to copy
		 */
		PersistentCache(const PersistentCache& other) = delete;
		/**
		 * @brief Assignment operator
		 *
		 * This is removed as there should one be a single cache instance
		 *
		 * @param other PersistentCache to assign
		 * @return New cache object
		 */
		PersistentCache& operator=(const PersistentCache& other) = delete;

		/**
		 * @brief Move constructor
		 *
		 * This is removed as there should one be a single cache instance
		 *
		 * @param other PersistentCache to copy
		 */
		PersistentCache(PersistentCache&& other) = delete;
		/**
		 * @brief Move assignment operator
		 *
		 * This is removed as there should one be a single cache instance
		 *
		 * @param other PersistentCache to assign
		 * @return New cache object
		 */
		PersistentCache& operator=(PersistentCache&& other) = delete;

		/**
		 * @brief Get the game object with the given id
		 *
		 * @tparam T Class of the data object to be retrieved
		 *           Must be derived from Persistent
		 * @param id Id of the object being retrieved
		 * @return Reference to the data object
		 * @throws out_of_range if there is no object for the id with the given type
		 */
		template <persistent_object T>
		T& get(std::string id);

		/**
		 * @brief Move game object to the cache
		 *
		 * This changes ownership of the game object to the cache
		 *
		 * @tparam T Class of the game object to be retrieved
		 *           Must be derived from Persistent
		 * @param datum Object to move to the cache
		 */
		template <persistent_object T>
		void add(std::unique_ptr <T> datum);

		/**
		 * @brief Check if game object exists in the cache
		 * @tparam T Class of the game object to be retrieved
		 *           Must be derived from Persistent
		 * @param id Identifier of the game object
		 * @return Reference to the game object
		 */
		template <persistent_object T>
		bool exists(std::string id);

		/**
		 * @brief Populates a set with all the game object ids for a specific rule type
		 *
		 * The parameter is first erased and then populated with the key data so following the call it will only contain the ids
		 *
		 * @tparam T Class of the game object to be retrieved
		 *           Must be derived from Persistent
		 * @param keys Set of strings to populate with the ids of the game objects
		 */
		template <persistent_object T>
		void keys(std::set<std::string>& keys);

		/**
		 * @brief Get the number of game objects of a specific type in the cache
		 * @tparam T Class of the game object to be retrieved
		 *           Must be derived from Persistent
		 * @return Number of game objects of the specified type in the cache
		 */
		template <persistent_object T>
		int size();

	private:
		/**
		 * @brief Stores the state of the cache
		 */
		std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<Persistent>>> state;
		/**
		 * @brief Stores mutexes to make the class threads safe
		 */
		std::unordered_map<std::type_index, std::mutex> mutexes;
	};

	template<persistent_object T>
	inline T& PersistentCache::get(std::string id) {
		// Grab a mutex for thread safety
		auto& mutex = mutexes[typeid(T)];
		std::lock_guard<std::mutex> guard(mutex);

		// Grab a reference to the hash map for the particular type of data we want
		auto& ruledata_hash_map = state[typeid(T)];

		// Search the resource hash map for the existance of the requested resource
		auto ruledata_it = ruledata_hash_map.find(id);

		// If resource is found, then we return it as a reference
		if (ruledata_it != ruledata_hash_map.end()) {
			return *dynamic_cast<T*>(ruledata_it->second.get());
		} else {
			throw std::out_of_range("There is no game object with id " + id);
		}
	}

	template<persistent_object T>
	inline void PersistentCache::add(std::unique_ptr <T> datum) {
		// Grab a mutex for thread safety
		auto& mutex = mutexes[typeid(T)];
		std::lock_guard<std::mutex> guard(mutex);

		// Grab a reference to the hash map for the particular type of data we want
		auto& ruledata_hash_map = state[typeid(T)];

		// Add the new rule data to our cache
		ruledata_hash_map.insert_or_assign(datum.get()->id(), std::move(datum));
	}

	template<persistent_object T>
	inline bool PersistentCache::exists(std::string id) {
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

	template<persistent_object T>
	inline void PersistentCache::keys(std::set<std::string>& keys) {
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

	template<persistent_object T>
	inline int PersistentCache::size() {
		// Grab a mutex for thread safety
		auto& mutex = mutexes[typeid(T)];
		std::lock_guard<std::mutex> guard(mutex);
		// Grab a reference to the hash map for the particular type of data we want
		auto& ruledata_hash_map = state[typeid(T)];
		return ruledata_hash_map.size();
	}

} // namespace rm