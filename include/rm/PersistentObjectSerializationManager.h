#pragma once

#include <PersistentObjectManager.h>
#include <JsonFileSerializer.h>
#include <TsvFileSerializer.h>
#include <FileSerializers.h>
#include <memory>
#include <type_traits>

namespace rm {

namespace json = boost::json;

/**
 * @brief Class to manage the (de)serialization of PersistentObject objects to and from JSON data
 *
 * This class is intended to be used as a utility class to enable the (de)serialization of PersistentObject objects to and from JSON data. It is not intended to be used as a base class for serializers, but rather as a utility class that can
 * be used by serializers to manage the creation of PersistentObject objects from JSON data and the conversion of PersistentObject objects to JSON data and loading and saving to file.
 */
class PersistentObjectSerializationManager {
public:
	/**
	 * @brief Constructor to initiate the object manager reference
	 * @param object_manager PersistentObjectManager reference to use for creating objects
	 */
	PersistentObjectSerializationManager(PersistentObjectManager& object_manager) : object_manager_{object_manager} {
	}

	/**
	 * @brief Set the data directory path
	 *
	 * This is the default location where the JSON data files are located. It will be used during the load/save routines.
	 * @param data_directory Data directory path
	 */
	void setDataDirectory(std::string data_directory) {
		data_directory_ = data_directory;
	}

	/**
	 * @brief Load all objects of a specific persistent type from a JSON file
	 *
	 * This template function loads all objects of type T from the specified JSON file using the appropriate
	 * serializer for that type. The function automatically selects the correct serializer and root key based on
	 * the type T.
	 *
	 * @tparam T The persistent object type to load (must satisfy persistent_object concept)
	 * @param filename Name of the JSON file to load (will be appended to data_directory_)
	 * @param root_key The key in the JSON file that contains the array of objects
	 *
	 * @throws std::runtime_error If the file cannot be opened or parsed
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.deserializeAllObjects<rm::rule::BookData>("books.json", "books");
	 * @endcode
	 */
	template<persistent_object T>
	void deserializeAllObjects(const std::string& filename, const std::string& root_key);

	/**
	 * @brief Load all objects of a specific persistent type from a TSV file
	 *
	 * This template function loads all objects of type T from the specified TSV file using the appropriate
	 * serializer for that type.
	 *
	 * @tparam T The persistent object type to load (must satisfy persistent_object concept)
	 * @param filename Name of the TSV file to load (will be appended to data_directory_)
	 *
	 * @throws std::runtime_error If the file cannot be opened or parsed
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.deserializeTsv<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
	 * @endcode
	 */
	template<persistent_object T>
	void deserializeTsv(const std::string& filename);

	/**
	 * @brief Save all objects of a specific persistent type to a JSON file
	 *
	 * This template function saves all objects of type T to the specified JSON file using the appropriate
	 * serializer for that type. The function automatically selects the correct serializer and root key based on
	 * the type T.
	 *
	 * @tparam T The persistent object type to save (must satisfy persistent_object concept)
	 * @param filename Name of the JSON file to save to (will be appended to data_directory_)
	 * @param root_key The key in the JSON file that contains the array of objects
	 *
	 * @throws std::runtime_error If the file cannot be opened for writing
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.serializeAllObjects<rm::rule::BookData>("books.json", "books");
	 * @endcode
	 */
	template<persistent_object T>
	void serializeAllObjects(const std::string& filename, const std::string& root_key);

	/**
	 * @brief Save all objects of a specific persistent type to a TSV file
	 *
	 * This template function saves all objects of type T to the specified TSV file using the appropriate
	 * serializer for that type.
	 *
	 * @tparam T The persistent object type to save (must satisfy persistent_object concept)
	 * @param filename Name of the TSV file to save to (will be appended to data_directory_)
	 *
	 * @throws std::runtime_error If the file cannot be opened for writing
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.serializeTsv<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
	 * @endcode
	 */
	template<persistent_object T>
	void serializeTsv(const std::string& filename);

	/**
	 * @brief Get the JSON representation of all objects of a specific persistent type
	 *
	 * This template function gets the JSON representation of all objects of type T using the appropriate serializer for that type. The function automatically selects the correct serializer and root key based on the type T.
	 *
	 * @tparam T The persistent object type to serialize (must satisfy persistent_object concept)
	 * @return A string containing the JSON representation of all objects of type T
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * std::string json = manager.serializeAllObjects<rm::rule::BookData>();
	 * @endcode
	 */
	template<persistent_object T>
	std::string serializeAllObjects_Impl(std::string_view root_key);

	/**
	 * @brief Get the JSON representation of all objects of any game rule type with a specific prefix in their ID
	 *
	 * This function gets the JSON representation of all objects of any game rule type that have IDs starting with the specified prefix using the appropriate serializer for each type.
	 *
	 * @param prefix Lowercase string of the prefix to match in the IDs of the objects to serialize
	 * @return A string containing the JSON representation of all matching objects
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * std::string json = manager.serializeAllObject("SKILL_");
	 * @endcode
	 */
	std::string serializeAllObjects(std::string_view prefix);

	/**
	 * @brief Deserialize all known data objects from file to preload the cache
	 *
	 * This function is a convenience wrapper around the template deserializeAllObjects function that uses the default filename and root key for each type.
	 *
	 * @throws std::runtime_error If the file cannot be opened or parsed
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.load();
	 * @endcode
	 */
	void load();

	/**
	 * @brief Reset the cache, clearing all stored game objects and mutexes
	 *
	 * This is used to reset the cache to a clean state, typically when starting a new game or loading a saved game. It will clear all stored game objects and mutexes, and clear the deleted flags.
	 */
	void resetCache() {
		object_manager_.resetCache();
	}

	/**
	 * @brief Serialize all known data objects to file
	 *
	 * This function is a convenience wrapper around the template serializeAllObjects function that uses the default filename and root key for each type.
	 *
	 * @throws std::runtime_error If the file cannot be opened for writing
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.save();
	 * @endcode
	 */
	void save();

	/**
	 * @brief Serialize all objects of any game rule type with a specific prefix in their ID to file
	 *
	 * This function serializes all objects of any game rule type that have IDs starting with the specified prefix to the appropriate JSON files using the appropriate serializer for each type. The function automatically selects the correct
	 * serializer and root key based on the prefix.
	 *
	 * @param prefix Lowercase string of the prefix to match in the IDs of the objects to serialize
	 *
	 * @throws std::runtime_error If any of the files cannot be opened or parsed
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * manager.save("skill");
	 * @endcode
	 */
	void save(std::string_view prefix);

	/**
	 * @brief Get the JSON representation of a single object of a specific persistent type
	 *
	 * This template function gets the JSON representation of a single object of type T using the appropriate serializer for that type.
	 *
	 * @tparam T The persistent object type to serialize (must satisfy persistent_object concept)
	 * @param obj The object to serialize
	 * @return A string containing the JSON representation of the object
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * auto& book = object_manager.get<rm::rule::BookData>("BOOK_MAGIC");
	 * std::string json = manager.serializeObject(book);
	 * @endcode
	 */
	template<persistent_object T>
	std::string serializeObject(const T& obj);

	/**
	 * @brief Get the JSON representation of a single object of a specific persistent type by ID
	 *
	 * This template function gets the JSON representation of a single object of type T using the appropriate serializer for that type.
	 *
	 * @tparam T The persistent object type to serialize (must satisfy persistent_object concept)
	 * @param id The ID of the object to serialize
	 * @return A string containing the JSON representation of the object
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * std::string json = manager.serializeObject<rm::rule::BookData>("BOOK_MAGIC");
	 * @endcode
	 */
	template<persistent_object T>
	std::string serializeObject(std::string_view id);

	/**
	 * @brief Get the JSON representation of a single object of any game data rule type by ID
	 *
	 * This function gets the JSON representation of a single object of any game data rule type using the appropriate serializer for that type. The type of the object is determined by its ID prefix.
	 *
	 * @param id The ID of the object to serialize
	 * @return A string containing the JSON representation of the object
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * std::string json = manager.serializeAnyDataObject("BOOK_MAGIC");
	 * @endcode
	 */
	std::string serializeAnyDataObject(const std::string& id);

	/**
	 * @brief Get the JSON representation of a single game object by ID
	 *
	 * This function gets the JSON representation of a single game object using the appropriate serializer for that type. The type of the object is determined by the label parameter.
	 *
	 * @param id The ID of the object to serialize
	 * @param label The label of the object type
	 * @return A string containing the JSON representation of the object
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * std::string json = manager.serializeAnyGameObject("2200490a-16f7-49b4-bc4a-5550d947ef13", "character");
	 * @endcode
	 */
	std::string serializeAnyGameObject(const std::string_view id, const std::string_view label);

	/**
	 * @brief Deserialize a single object of a specific persistent type from a JSON string
	 *
	 * This template function deserializes a single object of type T from the specified JSON string using the appropriate serializer for that type. The deserialized object is added to the cache and a reference to it is returned.
	 *
	 * @tparam T The persistent object type to deserialize (must satisfy persistent_object concept)
	 * @param json_str A string containing the JSON representation of the object
	 * @return A reference to the deserialized object in the cache
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * std::string json = R"({"id": "BOOK_MAGIC", "name": "Magic Book", "description": "A book of magic spells."})";
	 * const auto& book = manager.deserializeObject<rm::rule::BookData>(json);
	 * @endcode
	 */
	template<persistent_object T>
	const T& deserializeObject(const std::string& json_str);

	/**
	 * @brief Deserialize a single object of a specific persistent type from a JSON object
	 *
	 * This template function deserializes a single object of type T from the specified JSON object using the appropriate serializer for that type. The deserialized object is added to the cache and a reference to it is returned.
	 *
	 * @tparam T The persistent object type to deserialize (must satisfy persistent_object concept)
	 * @param obj A JSON object containing the representation of the object
	 * @return A reference to the deserialized object in the cache
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * json::object obj = {{"id", "BOOK_MAGIC"}, {"name", "Magic Book"}, {"description", "A book of magic spells."}};
	 * const auto& book = manager.deserializeObject<rm::rule::BookData>(obj);
	 * @endcode
	 */
	template<persistent_object T>
	const T& deserializeObject(json::object& obj);

	/**
	 * @brief Get the JSON representation of a container of objects
	 *
	 * This template function gets the JSON representation of a container of objects, typically std::strings representing the IDs of the objects to serialize.
	 *
	 * @tparam Container The type of the container (e.g. std::vector<std::string>, std::set<std::string>, etc.)
	 * @param container The container of objects to serialize
	 * @param key The key to use in the JSON output for each object (e.g. "books" for a container of BookData object IDs)
	 * @return A string containing the JSON representation of the container of objects
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * const std::set<std::string> ids = manager->objectManager().getAllIds("book");
	 * std::string json = manager.serializeContainer(books, "books");
	 * @endcode
	 */
	template<typename Container>
	std::string serializeContainer(const Container& container, std::string_view key);

	/**
	 * @brief Get a reference to the PersistentObjectManager used by this serialization manager
	 *
	 * @return Reference to the PersistentObjectManager
	 */
	PersistentObjectManager& objectManager() const {
		return object_manager_;
	}

	/**
	 * @brief Get the root key to use in the JSON file for a given type
	 *
	 * This function returns the root key to use in the JSON file for a given type. The root key is used to identify the array of objects in the JSON file that corresponds to the given type.
	 * The value returned is the natural plural of the type name, e.g. "books" for "book", "skillcategories" for "skillcategory", etc.
	 *
	 * @param prefix The type prefix to get the root key for (e.g. "book", "skillcategory", etc.)
	 * @return The root key to use in the JSON file for the given type
	 */
	const std::string getRootKeyForType(std::string_view prefix);

	/**
	 * @brief Get the JSON representation of a single object of any game rule type by ID
	 *
	 * This function gets the JSON representation of a single object of any game rule type using the appropriate serializer for that type. The type of the object is determined by its ID prefix.
	 *
	 * @param obj A JSON object containing the representation of the object
	 * @param prefix The type prefix to determine the type of the object (e.g. "book", "skillcategory", etc.)
	 * @return The ID of the deserialized object in the cache
	 *
	 * @code
	 * PersistentObjectSerializationManager manager(object_manager);
	 * const std::string book_id = manager.deserializeAnyObject(json_obj, "book");
	 * @endcode
	 */
	const std::string deserializeObject(json::object& obj, std::string_view prefix);

private:
	PersistentObjectManager& object_manager_;
	std::string data_directory_{"../../../../data/"};

	/**
	 * @brief Helper function to create the appropriate JSON serializer for a given type
	 * @tparam T The persistent object type
	 * @return A unique pointer to the appropriate PersistentJsonSerializer
	 */
	template<persistent_object T>
	std::unique_ptr<serial::PersistentJsonSerializer<T>> createJsonSerializer();

	/**
	 * @brief Helper function to create the appropriate TSV serializer for a given type
	 * @tparam T The persistent object type
	 * @return A unique pointer to the appropriate PersistentTsvSerializer
	 */
	template<persistent_object T>
	std::unique_ptr<serial::PersistentTsvSerializer<T>> createTsvSerializer();
};

// Template implementation - must be in header for template instantiation

template<persistent_object T>
void PersistentObjectSerializationManager::deserializeAllObjects(const std::string& filename, const std::string& root_key) {
	using namespace rm::serial;

	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();

	// Construct the full file path and create the file serializer, then load the data
	JsonFileSerializer<T> file_serializer(*serializer, root_key, data_directory_ + filename);
	try {
		file_serializer.load();
	} catch (const std::exception& e) {
		throw std::runtime_error("Failed to load JSON file: " + data_directory_ + filename + " - Error: " + e.what());
	}
}

template<persistent_object T>
void PersistentObjectSerializationManager::deserializeTsv(const std::string& filename) {
	using namespace rm::serial;

	// Create the appropriate TSV serializer for type T
	auto serializer = createTsvSerializer<T>();

	// Construct the full file path and create the file serializer, then load the data
	TsvFileSerializer<T> file_serializer(*serializer, data_directory_ + filename);
	file_serializer.load();
}

template<persistent_object T>
void PersistentObjectSerializationManager::serializeAllObjects(const std::string& filename, const std::string& root_key) {
	using namespace rm::serial;

	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();

	// Construct the full file path and create the file serializer, then save the data
	JsonFileSerializer<T> file_serializer(*serializer, root_key, data_directory_ + filename);
	file_serializer.save();
}

template<persistent_object T>
void PersistentObjectSerializationManager::serializeTsv(const std::string& filename) {
	using namespace rm::serial;

	// Create the appropriate TSV serializer for type T
	auto serializer = createTsvSerializer<T>();

	// Construct the full file path and create the file serializer, then save the data
	TsvFileSerializer<T> file_serializer(*serializer, data_directory_ + filename);
	file_serializer.save();
}

template<persistent_object T>
std::unique_ptr<serial::PersistentJsonSerializer<T>> PersistentObjectSerializationManager::createJsonSerializer() {
	using namespace rm::rule;
	using namespace rm::serial;

	// Use if constexpr (C++17) to select the appropriate serializer at compile time
	if constexpr (std::is_same_v<T, AnimalData>) {
		return std::make_unique<AnimalSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, ArmourTypeData>) {
		return std::make_unique<ArmourTypeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, table::AttackTable>) {
		return std::make_unique<AttackTableSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, AutoCharacterBuilder>) {
		return std::make_unique<AutoCharacterBuilderSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, BookData>) {
		return std::make_unique<BookSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, Character>) {
		return std::make_unique<CharacterSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, CharacterBuilder>) {
		return std::make_unique<CharacterBuilderSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, CharacterLeveller>) {
		return std::make_unique<CharacterLevellerSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, CreaturePaceData>) {
		return std::make_unique<CreaturePaceSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, CultureData>) {
		return std::make_unique<CultureSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, CultureTypeData>) {
		return std::make_unique<CultureTypeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, DiseaseData>) {
		return std::make_unique<DiseaseSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, DiseaseTypeData>) {
		return std::make_unique<DiseaseTypeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, ForagableData>) {
		return std::make_unique<ForagableSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, LanguageCategoryData>) {
		return std::make_unique<LanguageCategorySerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, LanguageData>) {
		return std::make_unique<LanguageSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, PoisonData>) {
		return std::make_unique<PoisonSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, PoisonTypeData>) {
		return std::make_unique<PoisonTypeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, ProfessionData>) {
		return std::make_unique<ProfessionSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, RaceData>) {
		return std::make_unique<RaceSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, SkillData>) {
		return std::make_unique<SkillSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, SkillCategoryData>) {
		return std::make_unique<SkillCategorySerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, SkillGroupData>) {
		return std::make_unique<SkillGroupSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, SkillProgressionTypeData>) {
		return std::make_unique<SkillProgressionTypeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, table::SpecialAttackTable>) {
		return std::make_unique<SpecialAttackTableSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, SpellListData>) {
		return std::make_unique<SpellListSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, TrainingPackageData>) {
		return std::make_unique<TrainingPackageSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, TreasureCodeData>) {
		return std::make_unique<TreasureCodeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, WeaponTypeData>) {
		return std::make_unique<WeaponTypeSerializer>(object_manager_);
	} else {
		static_assert(sizeof(T) == 0, "No JSON serializer defined for this persistent object type");
	}
}

template<persistent_object T>
std::unique_ptr<serial::PersistentTsvSerializer<T>> PersistentObjectSerializationManager::createTsvSerializer() {
	using namespace rm::rule;
	using namespace rm::serial;

	// Use if constexpr (C++17) to select the appropriate serializer at compile time
	if constexpr (std::is_same_v<T, table::TrainingPackageCostTable>) {
		return std::make_unique<TrainingPackageCostTableSerializer>(object_manager_);
	} else {
		static_assert(sizeof(T) == 0, "No TSV serializer defined for this persistent object type");
	}
}

template<persistent_object T>
std::string PersistentObjectSerializationManager::serializeObject(const T& obj) {
	using namespace rm::serial;
	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();
	// Serialize the object to a JSON value
	json::value json_value = serializer->serializeObject(obj);
	// use the json library to convert the JSON value to a string
	return json::serialize(json_value);
}

template<persistent_object T>
std::string PersistentObjectSerializationManager::serializeObject(std::string_view id) {
	auto& obj = object_manager_.get<T>(std::string{id});
	return serializeObject(obj);
}

template<persistent_object T>
const T& PersistentObjectSerializationManager::deserializeObject(const std::string& json_str) {
	json::value json_value = json::parse(json_str);
	// Deserialize the object from the JSON value
	return deserializeObject(json_value.as_object());
}

template<persistent_object T>
const T& PersistentObjectSerializationManager::deserializeObject(json::object& obj) {
	using namespace rm::serial;
	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();
	// Deserialize the object from the JSON value
	return serializer->deserializeObject(obj);
}

template<persistent_object T>
std::string PersistentObjectSerializationManager::serializeAllObjects_Impl(std::string_view root_key) {
	using namespace rm::serial;
	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();

	std::ostringstream json_stream;
	std::vector<std::reference_wrapper<T>>{object_manager_.getAll<T>()}; // We need to create a vector of reference wrappers to avoid copying the objects when we iterate over them, as they may not be copyable

	json_stream << "{ \"" << root_key << "\": [";
	if (!object_manager_.getAll<T>().empty()) {
		for (const auto& obj : object_manager_.getAll<T>()) {
			json_stream << json::serialize(serializer->serializeObject(obj)) + ",";
		}
		// Remove the trailing comma if there was at least one object, we can do this as we know that the stream is not empty and we will be appeneding a closing bracket after this
		json_stream.seekp(-1, std::ios_base::end);
	}
	json_stream << "]}";

	return json_stream.str();
}

template<typename Container>
std::string PersistentObjectSerializationManager::serializeContainer(const Container& container, std::string_view key) {
	std::ostringstream json_stream;
	json_stream << "{ \"" << key << "\": [";
	if (!container.empty()) {
		for (const auto& obj : container) {
			json_stream << json::serialize(json::value(obj)) + ",";
		}
		// Remove the trailing comma if there was at least one object, we can do this as we know that the stream is not empty and we will be appeneding a closing bracket after this
		json_stream.seekp(-1, std::ios_base::end);
	}
	json_stream << "]}";
	return json_stream.str();
}

} // namespace rm