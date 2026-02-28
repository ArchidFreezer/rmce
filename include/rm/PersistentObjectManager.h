#pragma once

#include <PersistentCache.h>

namespace rm {

	/**
	 * @class PersistentObjectManager
	 * @brief Factory class to manage the creation and retrieval of PersistentObject objects
	 *
	 * Persistent objects that need to be serialised should be managed through the #get methods.
	 *
	 * If the object already exists it will be retrieved from the cache and if not it will be created and moved into the cache
	 * first and then retrieved and returned. In all cases the actual object reference returned will come from the cache to
	 * ensure that the reference is to a persistent object.
	 *
	* Transient objects should use the #create method which does not use the cache and the objects will not be serialised.
		*/
	class PersistentObjectManager {
	public:

		/**
		 * @brief Deleted default constructor to ensure the cache is initiated
		 */
		PersistentObjectManager() = delete;

		/**
		 * @brief Constructor to initiate the cache
		 * @param cache PersistentObjectCache cache to move the created objects into
		 */
		PersistentObjectManager(PersistentCache& cache) : cache_{ cache } {}

		/**
	 * @brief Populates a set with all the game object ids for a specific rule type
	 *
	 * The parameter is first erased and then populated with the key data so following the call it will only contain the ids
	 *
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from PersistentObject
	 * @param keys Set of strings to populate with the ids of the game objects
	 */
		template <persistent_object T>
		void keys(std::set<std::string>& keys) {
			cache_.keys<T>(keys);
		}

		/**
		 * @brief Get a new PersistentObject object with a randomly generated UUID as its ID
		 *
		 * The object is not added to the cache and is suitable for temporary objects that do not need to be referenced by ID, e.g. objects that are only used in the context of a single function call.
		 *
		 * Objects created with this method will not be serialised.
		 *
		 * @tparam T type of PersistentObject object to create
		 * @return Uniqur pointer to PersistentObject object of type @a T
		 */
		template <persistent_object T>
		std::unique_ptr<T> create() { return std::unique_ptr<T>(new T()); }

		/**
		 * @brief Get a new PersistentObject object with a randomly generated UUID as its ID and add it to the cache
		 *
		 * Objects created with this method will be serialised.
		 *
		 * @tparam T type of PersistentObject object to create
		 * @return PersistentObject object from the cache of type @a T
		 */
		template<persistent_object T>
		T& get() {
			// Create a new object and add it to the cache.
			std::unique_ptr<T> obj(new T());
			std::string id = obj->id(); // We need to store this as adding the object to the cache moves it, invalidating the object we have created
			cache_.add<T>(std::move(obj));
			return cache_.get<T>(id);
		}

		/**
		 * @brief Get standard PersistentObject objects that may be created with an ID only
		 *
		 * Objects created with this method will be serialised.
		 *
		 * @tparam T type of PersistentObject object to create
		 * @param id Unique ID of the object
		 * @return PersistentObject object from the cache of type @a T
		 */
		template<persistent_object T>
		T& get(std::string id) {
			// If the object already exists in the cache then we can return it without creating a new one
			if (cache_.exists<T>(id)) return cache_.get<T>(id);

			// Create a new object and add it to the cache. We need to store the id as adding the object to the cache moves it, invalidating the object we have created
			std::unique_ptr<T> obj(new T());
			obj->setId(id);
			cache_.add<T>(std::move(obj));
			return cache_.get<T>(id);
		}

	private:
		PersistentCache& cache_; /**< Reference to a cache to store the objects. */
	};

} // namespace rm