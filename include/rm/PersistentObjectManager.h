#pragma once

#include <optional>
#include <string>

#include <PersistentCache.h>
#include <SubcategoriedSkillData.h>

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
	PersistentObjectManager(PersistentCache& cache) : cache_{cache} {
	}

	/**
	 * @brief Populates a set with all the game object ids for a specific rule type
	 *
	 * The parameter is first erased and then populated with the key data so following the call it will only contain the ids
	 *
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from PersistentObject
	 * @param keys Set of strings to populate with the ids of the game objects
	 */
	template<persistent_object T>
	void keys(std::set<std::string>& keys);

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
	template<default_persistent_object T>
	std::unique_ptr<T> create() {
		return std::unique_ptr<T>(new T());
	}

	/**
	 * @brief Get a PersistentObject object that may be created with an ID only
	 *
	 * The object is not added to the cache and is suitable for temporary objects that do not need to be referenced by ID, e.g. objects that are only used in the context of a single function call.
	 *
	 * Objects created with this method will not be serialised.
	 *
	 * @tparam T type of PersistentObject object to create
	 * @return Uniqur pointer to PersistentObject object of type @a T
	 */
	template<id_persistent_object T>
	std::unique_ptr<T> create(std::string id) {
		std::unique_ptr<T> obj(new T(id));
		obj.get()->generateId(); // We need to generate the id as the constructor only sets the base id and does not generate the full id in the standard format, which is required for the object to be usable
		return obj;
	}

	/**
	 * @brief Check if a PersistentObject object with a specific ID exists in the cache
	 *
	 * @tparam T type of PersistentObject object to check
	 * @param id Unique ID of the object
	 * @return `true` if an object of type @a T with the specified ID exists in the cache, `false` otherwise
	 */
	template<persistent_object T>
	bool exists(std::string id) {
		return cache_.exists<T>(id);
	}

	/**
	 * @brief Check if a GameRuleData object with a specific ID exists in the cache without knowing the type
	 *
	 * This is useful for deserialisation when we want to check if an object with a specific ID exists but we do not know the type of the object. The method will search through all types of GameRuleData objects in the cache and return
	 * `true` if one with the matching ID is found. If there are multiple objects with the same ID then it is undefined which one will be returned.
	 *
	 * @param id Unique ID of the object
	 * @return `true` if an object with the specified ID exists in the cache, `false` otherwise
	 */
	bool existsAny(std::string id) {
		return (getAny(id) != nullptr);
	}

	/**
	 * @brief Get a PersistentObject object with a randomly generated UUID as its ID and add it to the cache
	 *
	 * Objects created with this method will be serialised.
	 *
	 * @tparam T type of PersistentObject object to create
	 * @return PersistentObject object from the cache of type @a T
	 */
	template<default_persistent_object T>
	T& get();

	/**
	 * @brief Get a PersistentObject object
	 *
	 * Objects created with this method will be serialised.
	 * 
	 * If an object with the specified ID already exists in the cache then it will be returned as a reference, but if not then a new object will be created with the specified ID, added to the cache and then returned as a reference. In
	 * either case the reference returned will be to the object in the cache to ensure that it is a persistent object and can be safely referenced without worrying about invalidating references by deleting objects from the cache.
	 *
	 * @tparam T type of PersistentObject object to create
	 * @param id Unique ID of the object
	 * @return PersistentObject object from the cache of type @a T
	 * @throws out_of_range if the object id has been flagged as deleted
	 */
	template<persistent_object T>
	T& get(std::string id);

	/**
	 * @brief Get all PersistentObject objects of a specific type from the cache
	 *
	 * This is useful for iterating over all objects of a specific type, e.g. when serialising all objects of a specific type to a JSON file.
	 *
	 * This function excludes objects that have been flagged as deleted or incomplete.
	 *
	 * @tparam T type of PersistentObject objects to retrieve
	 * @return Vector of references to PersistentObject objects of type @a T from the cache
	 */
	template<persistent_object T>
	std::vector<std::reference_wrapper<T>> getAll();

	/**
	 * @brief Get a GameRuleData object with a specific ID without knowing the type
	 *
	 * This is useful for deserialisation when we want to get an object by its ID but we do not know the type of the object. The method will search through all types of GameRuleData objects in the cache and return the one with the matching
	 * ID. If there are multiple objects with the same ID then it is undefined which one will be returned.
	 *
	 * @param id Unique ID of the object
	 * @return Pointer to the GameRuleData object with the matching ID or nullptr if no such object exists
	 */
	const rm::rule::GameRuleData* getAny(std::string id);

	/**
	 * @brief Get all IDs of GameRuleData objects with a specific prefix
	 *
	 * This is useful for deserialisation when we want to get all objects with a specific prefix in their ID, e.g. all skills that have IDs starting with "SKILL_". The method will search through all types of GameRuleData objects in the
	 * cache and return the IDs of those with the matching prefix.
	 *
	 * @param prefix Prefix to match in the IDs
	 * @return Set of strings containing the IDs of the matching GameRuleData objects
	 */
	const std::set<std::string> getAllIds(std::string_view prefix) const;

	/**
	 * @brief Get SubcategoriedSkillData objects
	 *
	 * SubcategoriedSkillData cannot be created with ID only as the ID is derived from optional arguments. In order to
	 * allow these to be safely created and cached a custom factory method has been created
	 * @param skill_data SkillData that is being wrapped
	 * @param subcategory optional subcategory of @a skill_data
	 * @return Reference to the SubcategoriedSkillData object with the matching skill and subcategory. If the object does not exist it will be created and added to the cache before being returned.
	 * @throws out_of_range if a SubcategoriedSkillData with the given id has been flagged as deleted
	 */
	rm::rule::SubcategoriedSkillData& subcategoriedSkillData(const rm::rule::SkillData& skill_data, std::optional<std::string_view> subcategory = std::nullopt);

	/**
	 * @brief Get SubcategoriedSkillData objects
	 *
	 * SubcategoriedSkillData cannot be created with ID only as the ID is derived from optional arguments. In order to
	 * allow these to be safely created and cached a custom factory method has been created
	 * @param skill_id Unique identifier of the skill that is being wrapped
	 * @param subcategory optional subcategory of @a skill_data
	 * @return Reference to the SubcategoriedSkillData object with the matching skill and subcategory. If the object does not exist it will be created and added to the cache before being returned.
	 * @throws out_of_range if a SubcategoriedSkillData with the given id has been flagged as deleted
	 */
	rm::rule::SubcategoriedSkillData& subcategoriedSkillData(std::string& skill_id, std::optional<std::string_view> subcategory = std::nullopt) {
		rm::rule::SkillData& skill = get<rm::rule::SkillData>(skill_id);
		return (subcategory ? subcategoriedSkillData(skill, subcategory) : subcategoriedSkillData(skill));
	}

	/**
	 * @brief Get all prefixes of GameRuleData objects in the cache
	 *
	 * This is useful for deserialisation when we want to get all objects with a specific prefix in their ID, e.g. all skills that have IDs starting with "SKILL_". This method will return all the prefixes, without the underscore, as
	 * lowercase values.
	 *
	 * @return Set of strings containing the unique prefixes of the GameRuleData objects in the cache
	 */
	const std::set<std::string> getAllPrefixes() const;

	/**
	 * @brief Get whether an object has been flagged as deleted
	 *
	 * This is used to check if an object has been flagged as deleted without invalidating existing references. This is necessary as the cache does not return references to the objects it stores, but instead returns references to copies of
	 * the objects, so if an object is deleted from the cache then any existing references to that object will still be valid, which can lead to bugs if those references are used after the object has been deleted.
	 *
	 * @param id Unique identifier of the object to check
	 */
	bool isDeleted(std::string id) const {
		return deleted_objects_.find(id) != deleted_objects_.end();
	}

	/**
	 * @brief Flag an object as deleted
	 *
	 * This is used to flag an object as deleted without invalidating existing references. This is necessary as the cache does not return references to the objects it stores, but instead returns references to copies of the objects, so if an
	 * object is deleted from the cache then any existing references to that object will still be valid, which can lead to bugs if those references are used after the object has been deleted.
	 *
	 * @param id Unique identifier of the object to flag as deleted
	 */
	void deleteObject(std::string id) {
		deleted_objects_.insert(id);
	}

	/**
	 * @brief Unflag an object as deleted
	 *
	 * This is used to unflag an object as deleted without invalidating existing references. This is necessary as the cache does not return references to the objects it stores, but instead returns references to copies of the objects, so if
	 * an object is deleted from the cache then any existing references to that object will still be valid, which can lead to bugs if those references are used after the object has been deleted.
	 *
	 * @param id Unique identifier of the object to unflag as deleted
	 */
	void undeleteObject(std::string id) {
		deleted_objects_.erase(id);
	}

	/**
	 * @brief Get whether an object is non-serialized and should not be serialised
	 *
	 * If this flag is set then the object will not be serialised, but it will still be retrievable from the cache and all existing references to the object will still be valid. This is useful for objects that are not created in a valid
	 * state to be serialised and need to be completed before they can be serialised, e.g. a character object where the end user has to select things such as the race and profession, plus make multiple selections, and only once these have
	 * been completed is the object 'saved' at which point this flag should be removed using the #unflagNonSerialized method.
	 *
	 * @param id Unique identifier of the object to check
	 * @see flagNonSerialized
	 * @see unflagNonSerialized
	 */
	bool isNonSerialized(std::string id) const {
		return non_serialized_objects_.find(id) != non_serialized_objects_.end();
	}

	/**
	 * @brief Flag an object as non-serialized
	 *
	 * This is used to flag an object as non-serialized without invalidating existing references. It is typically used when creating an object that is not fully contructed when on creation. An example would be a character object where the
	 * end user has to select things such as teh race  and professionn, plus make multiple selections and only once these ave been completd is the objects 'saved' at which point this flag should be removed using the #unflagNonSerialized
	 * method.
	 *
	 * @param id Unique identifier of the object to flag as non-serialized
	 * @see isNonSerialized
	 * @see unflagNonSerialized
	 */
	void flagNonSerialized(std::string id) {
		non_serialized_objects_.insert(id);
	}

	/**
	 * @brief Unflag an object as non-serialized
	 *
	 * This is used to unflag an object as non-serialized without invalidating existing references. It is typically used when an object is not created in a valid state to be serialised and needs to be completed. An example would be a
	 * character object where the end user has to select things such as the race and profession, plus make multiple selections, and only once these have been completed is the object 'saved' at which point this flag should be removed using
	 * the #unflagNonSerialized method.
	 *
	 * @param id Unique identifier of the object to unflag as non-serialized
	 */
	void unflagNonSerialized(std::string id) {
		non_serialized_objects_.erase(id);
	}

private:
	PersistentCache& cache_;                     /**< Reference to a cache to store the objects. */
	std::set<std::string> deleted_objects_{};    /**< Set of IDs of objects that have been deleted. This is used to flag an object as delted without invalidating existing references. */
	std::set<std::string> non_serialized_objects_{}; /**< Set of IDs of objects that should not be serialised. This is used to flag an object as non-serialised without invalidating existing references. */
};

template<persistent_object T>
inline void PersistentObjectManager::keys(std::set<std::string>& keys) {
	// First populate the set with the keys from the cache and then remove those that have been flagged as deleted
	cache_.keys<T>(keys);
	for (auto it = keys.begin(); it != keys.end();) {
		if (isDeleted(*it))
			it = keys.erase(it);
		else
			++it;
	}
}

template<default_persistent_object T>
inline T& PersistentObjectManager::get() {
	// Create a new object and add it to the cache.
	std::unique_ptr<T> obj(new T());
	std::string id = obj->id(); // We need to store this as adding the object to the cache moves it, invalidating the object we have created
	cache_.add<T>(std::move(obj));

	// If an object with his id has been flagged as deleted then we need to unflag it as it will not be retrievable otherwise.
	if (isDeleted(id)) {
		undeleteObject(id);
	}

	return cache_.get<T>(id);
}

template<persistent_object T>
inline T& PersistentObjectManager::get(std::string id) {
	// If the object has been deleted then throw an exception as it should not be retrievable without first undeleting it
	if (isDeleted(id))
		throw std::out_of_range("Object with id " + id + " has been deleted and cannot be retrieved.");

	// If the object already exists in the cache then we can return it without creating a new one
	if (cache_.exists<T>(id))
		return cache_.get<T>(id);

	if constexpr (id_persistent_object<T>) {
		// Create a new object and add it to the cache.
		//
		// We need to check if the id parameter already contains the prefix or not as the constructor will add the prefix to the id if it is not already present, but if the id parameter already contains the prefix then we do not want to add
		// it again as this will result in an incorrect id being generated and the object not being retrievable from the cache.
		std::string prefix = T::prefix_ + "_";
		if (id.starts_with(prefix)) {
			id = id.substr(prefix.size()); // Remove the prefix from the id as the constructor will add it back in
		}

		std::unique_ptr<T> obj(new T(id));
		std::string new_id =
		    std::string(obj.get()->generateId()); // We need to generate the id as the constructor only sets the base id and does not generate the full id in the standard format, which is required for the object to be usable
		cache_.add<T>(std::move(obj));
		return cache_.get<T>(new_id);
	} else {
		throw std::out_of_range("Object with id " + id + " does not exist in the cache and cannot be created as it is not an ID persistent object.");
	}
}

template<persistent_object T>
inline std::vector<std::reference_wrapper<T>> PersistentObjectManager::getAll() {
	std::vector<std::reference_wrapper<T>> objects;

	std::set<std::string> keySet;
	cache_.keys<T>(keySet); // Populate the set
	for (const auto& id : keySet) {
		if (!isDeleted(id) && !isNonSerialized(id))
			objects.push_back(std::ref(cache_.get<T>(id)));
	}
	return objects;
}

} // namespace rm