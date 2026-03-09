#pragma once

#include <PersistentObjectManager.h>
#include <JsonFileSerializer.h>
#include <TsvFileSerializer.h>
#include <FileSerializers.h>
#include <memory>
#include <type_traits>

namespace rm {

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
	 * manager.deserializeAllObjects<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
	 * @endcode
	 */
	template<persistent_object T>
	void deserializeAllObjects(const std::string& filename);

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
	 * manager.serializeAllObjects<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
	 * @endcode
	 */
	template<persistent_object T>
	void serializeAllObjects(const std::string& filename);

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

private:
	PersistentObjectManager& object_manager_;
	std::string data_directory_{"../../../../data/"};

	/**
	 * @brief Helper function to create the appropriate JSON serializer for a given type
	 * @tparam T The persistent object type
	 * @return A unique pointer to the appropriate PersistentJsonSerializer
	 */
	template<persistent_object T>
	std::unique_ptr<rule::serial::PersistentJsonSerializer<T>> createJsonSerializer();

	/**
	 * @brief Helper function to create the appropriate TSV serializer for a given type
	 * @tparam T The persistent object type
	 * @return A unique pointer to the appropriate PersistentTsvSerializer
	 */
	template<persistent_object T>
	std::unique_ptr<rule::serial::PersistentTsvSerializer<T>> createTsvSerializer();
};

// Template implementation - must be in header for template instantiation

template<persistent_object T>
void PersistentObjectSerializationManager::deserializeAllObjects(const std::string& filename, const std::string& root_key) {
	using namespace rm::rule::serial;

	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();

	// Construct the full file path and create the file serializer, then load the data
	JsonFileSerializer<T> file_serializer(*serializer, root_key, data_directory_ + filename);
	file_serializer.load();
}

template<persistent_object T>
void PersistentObjectSerializationManager::deserializeAllObjects(const std::string& filename) {
	using namespace rm::rule::serial;

	// Create the appropriate TSV serializer for type T
	auto serializer = createTsvSerializer<T>();

	// Construct the full file path and create the file serializer, then load the data
	TsvFileSerializer<T> file_serializer(*serializer, data_directory_ + filename);
	file_serializer.load();
}

template<persistent_object T>
void PersistentObjectSerializationManager::serializeAllObjects(const std::string& filename, const std::string& root_key) {
	using namespace rm::rule::serial;

	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();

	// Construct the full file path and create the file serializer, then save the data
	JsonFileSerializer<T> file_serializer(*serializer, root_key, data_directory_ + filename);
	file_serializer.save();
}

template<persistent_object T>
void PersistentObjectSerializationManager::serializeAllObjects(const std::string& filename) {
	using namespace rm::rule::serial;

	// Create the appropriate TSV serializer for type T
	auto serializer = createTsvSerializer<T>();

	// Construct the full file path and create the file serializer, then save the data
	TsvFileSerializer<T> file_serializer(*serializer, data_directory_ + filename);
	file_serializer.save();
}

template<persistent_object T>
std::unique_ptr<rule::serial::PersistentJsonSerializer<T>> PersistentObjectSerializationManager::createJsonSerializer() {
	using namespace rm::rule;
	using namespace rm::rule::serial;

	// Use if constexpr (C++17) to select the appropriate serializer at compile time
	if constexpr (std::is_same_v<T, AnimalData>) {
		return std::make_unique<AnimalSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, ArmourTypeData>) {
		return std::make_unique<ArmourTypeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, table::AttackTable>) {
		return std::make_unique<AttackTableSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, BookData>) {
		return std::make_unique<BookSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, ClimateData>) {
		return std::make_unique<ClimateSerializer>(object_manager_);
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
std::unique_ptr<rule::serial::PersistentTsvSerializer<T>> PersistentObjectSerializationManager::createTsvSerializer() {
	using namespace rm::rule;
	using namespace rm::rule::serial;

	// Use if constexpr (C++17) to select the appropriate serializer at compile time
	if constexpr (std::is_same_v<T, table::TrainingPackageCostTable>) {
		return std::make_unique<TrainingPackageCostTableSerializer>(object_manager_);
	} else {
		static_assert(sizeof(T) == 0, "No TSV serializer defined for this persistent object type");
	}
}

template<persistent_object T>
std::string PersistentObjectSerializationManager::serializeObject(const T& obj) {
	using namespace rm::rule::serial;
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
	using namespace rm::rule::serial;
	// Create the appropriate JSON serializer for type T
	auto serializer = createJsonSerializer<T>();
	// Parse the JSON string into a JSON value
	json::value json_value = json::parse(json_str);
	// Deserialize the object from the JSON value
	return serializer->deserializeObject(json_value.as_object());
}

} // namespace rm