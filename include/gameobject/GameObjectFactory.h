#pragma once

#include <GameObject.h>
#include <GameObjectCache.h>

/**
 * @class GameObjectFactory
 * @brief Factory class to get GameObject objects from their ID
 *
 * If the object already exists it will be retrieved from the cache and if not it will be created and moved into the cache
 * first and then retrieved and returned. In all cases the actual object reference returned will come from the cache to
 * ensure that the reference is to a persistent object.
 */
class GameObjectFactory {
public:

	/**
	 * @brief Deleted default constructor to ensure the cache is initiated
	 */
	GameObjectFactory() = delete;

	/**
	 * @brief Constructor to initiate the cache
	 * @param cache GameObjectCache cache to move the created objects into
	 */
	GameObjectFactory(GameObjectCache& cache) : cache_{ cache } {}

	/**
 * @brief Populates a set with all the game object ids for a specific rule type
 *
 * The parameter is first erased and then populated with the key data so following the call it will only contain the ids
 *
 * @tparam T Class of the data object to be retrieved
 *           Must be derived from GameObject
 * @param keys Set of strings to populate with the ids of the game objects
 */
	template <game_object T>
	void keys(std::set<std::string>& keys) {
		return cache_.keys<T>(keys);
	}

	/**
	 * @brief Get a new GameObject object with a randomly generated UUID as its ID
	 * 
	 * The object is not added to the cache and is suitable for temporary objects that do not need to be referenced by ID, e.g. objects that are only used in the context of a single function call and do not need to be stored persistently.
	 * @tparam T type of GameObject object to create
	 * @return Uniqur pointer to GameObject object of type @a T
	 */
	template <game_object T>
	std::unique_ptr<T> create() { return std::move(std::make_unique<T>());}

	/**
	 * @brief Get a new GameObject object with a randomly generated UUID as its ID and add it to the cache
	 * @tparam T type of GameObject object to create
	 * @return GameObject object from the cache of type @a T
	 */
	template<game_object T>
	T& get() {
		std::unique_ptr<T> obj = std::make_unique<T>();
		std::string id = obj->id(); // We need to store this as adding the object to the cache moves it, invalidating the object we have created
		cache_.add<T>(std::move(obj));
		return cache_.get<T>(id);
	}

	/**
	 * @brief Get standard GameObject objects that may be created with an ID only
	 * @tparam T type of GameObject object to create
	 * @param id Unique ID of the object
	 * @return GameObject object from the cache of type @a T
	 */
	template<game_object T>
	T& get(std::string id) {
		if (cache_.exists<T>(id)) return cache_.get<T>(id);
		std::unique_ptr<T> obj = std::make_unique<T>();
		obj->setId(id);
		cache_.add<T>(std::move(obj));
		return cache_.get<T>(id);
	}

private:
	GameObjectCache& cache_; /**< Reference to a cache object to store the data objects */

};
