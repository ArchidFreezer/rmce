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
		return std::unique_ptr<T>(new T(id));
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
	T& get() {
		// Create a new object and add it to the cache.
		std::unique_ptr<T> obj(new T());
		std::string id = obj->id(); // We need to store this as adding the object to the cache moves it, invalidating the object we have created
		cache_.add<T>(std::move(obj));
		return cache_.get<T>(id);
	}

	/**
	 * @brief Get a PersistentObject object that may be created with an ID only
	 *
	 * Objects created with this method will be serialised.
	 *
	 * @tparam T type of PersistentObject object to create
	 * @param id Unique ID of the object
	 * @return PersistentObject object from the cache of type @a T
	 */
	template<id_persistent_object T>
	T& get(std::string id) {
		// If the object already exists in the cache then we can return it without creating a new one
		if (cache_.exists<T>(id))
			return cache_.get<T>(id);

		// Create a new object and add it to the cache.
		std::unique_ptr<T> obj(new T(id));
		cache_.add<T>(std::move(obj));
		return cache_.get<T>(id);
	}

	/**
	 * @brief Get all PersistentObject objects of a specific type from the cache
	 *
	 * This is useful for iterating over all objects of a specific type, e.g. when serialising all objects of a specific type to a JSON file.
	 *
	 * @tparam T type of PersistentObject objects to retrieve
	 * @return Vector of references to PersistentObject objects of type @a T from the cache
	 */
	template<persistent_object T>
	std::vector<std::reference_wrapper<T>> getAll() {
		std::vector<std::reference_wrapper<T>> objects;

		std::set<std::string> keySet;
		cache_.keys<T>(keySet); // Populate the set
		for (const auto& id : keySet) {
			objects.push_back(std::ref(cache_.get<T>(id)));
		}
		return objects;
	}

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
	 * @brief Get SubcategoriedSkillData objects
	 *
	 * SubcategoriedSkillData cannot be created with ID only as the ID is derived from optional arguments. In order to
	 * allow these to be safely created and cached a custom factory method has been created
	 * @param skill_data SkillData that is being wrapped
	 * @param subcategory optional subcategory of @a skill_data
	 * @return
	 */
	rm::rule::SubcategoriedSkillData& subcategoriedSkillData(const rm::rule::SkillData& skill_data, std::optional<std::string_view> subcategory = std::nullopt) {
		std::string id{skill_data.id() + (subcategory ? "_" + std::string(subcategory.value()) : "")};
		if (cache_.exists<rm::rule::SubcategoriedSkillData>(id))
			return cache_.get<rm::rule::SubcategoriedSkillData>(id);
		if (subcategory)
			cache_.add<rm::rule::SubcategoriedSkillData>(std::move(std::make_unique<rm::rule::SubcategoriedSkillData>(skill_data, subcategory)));
		else
			cache_.add<rm::rule::SubcategoriedSkillData>(std::move(std::make_unique<rm::rule::SubcategoriedSkillData>(skill_data)));
		return cache_.get<rm::rule::SubcategoriedSkillData>(id);
	}

	/**
	 * @brief Get SubcategoriedSkillData objects
	 *
	 * SubcategoriedSkillData cannot be created with ID only as the ID is derived from optional arguments. In order to
	 * allow these to be safely created and cached a custom factory method has been created
	 * @param skill_id Unique identifier of the skill that is being wrapped
	 * @param subcategory optional subcategory of @a skill_data
	 * @return
	 */
	rm::rule::SubcategoriedSkillData& subcategoriedSkillData(std::string& skill_id, std::optional<std::string_view> subcategory = std::nullopt) {
		rm::rule::SkillData& skill = get<rm::rule::SkillData>(skill_id);
		return (subcategory ? subcategoriedSkillData(skill, subcategory) : subcategoriedSkillData(skill));
	}

	/**
	 * @brief Creates a string that can be used as a unique identifier for an object
	 *
	 * Each object requires a unique identifier and this is a helper function that can create one in a standard format. The
	 * output is a string that is a combination of the type and val parameters that only contain ucase letters, numbers and
	 * the underscore character. Any character that is not an alphanum will be converted to an underscore with consecutive
	 * underscores being discarded.
	 *
	 * @param type The type of data object
	 * @param val The unique name of the data object
	 * @return String containing an id in standard format
	 */
	const std::string generateId(std::string_view type, std::string_view val);

private:
	PersistentCache& cache_; /**< Reference to a cache to store the objects. */
};

} // namespace rm