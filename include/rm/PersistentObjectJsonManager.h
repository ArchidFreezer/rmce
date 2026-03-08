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
class PersistentObjectJsonManager {
public:
	/**
	 * @brief Constructor to initiate the object manager reference
	 * @param object_manager PersistentObjectManager reference to use for creating objects
	 */
	PersistentObjectJsonManager(PersistentObjectManager& object_manager) : object_manager_{object_manager} {
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
	 * PersistentObjectJsonManager manager(object_manager);
	 * manager.deserializeAllObjects<rm::rule::BookData>("books.json", "books");
	 * @endcode
	 */
	template<persistent_object T>
	void deserializeAllObjects(const std::string& filename, const std::string& root_key);

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
	 * PersistentObjectJsonManager manager(object_manager);
	 * manager.serializeAllObjects<rm::rule::BookData>("books.json", "books");
	 * @endcode
	 */
	template<persistent_object T>
	void serializeAllObjects(const std::string& filename, const std::string& root_key);

	/**
	 * @brief Deserialize all known data objects from file to preload the cache
	 *
	 * This function is a convenience wrapper around the template deserializeAllObjects function that uses the default filename and root key for each type.
	 *
	 * @throws std::runtime_error If the file cannot be opened or parsed
	 *
	 * @code
	 * PersistentObjectJsonManager manager(object_manager);
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
	 * PersistentObjectJsonManager manager(object_manager);
	 * manager.save();
	 * @endcode
	 */
	void save();

private:
	PersistentObjectManager& object_manager_;
	std::string data_directory_{"../../../../data/"};

	/**
	 * @brief Helper function to create the appropriate serializer for a given type
	 * @tparam T The persistent object type
	 * @return A unique pointer to the appropriate PersistentJsonSerializer
	 */
	template<persistent_object T>
	std::unique_ptr<rule::serial::PersistentJsonSerializer<T>> createSerializer();
};

// Template implementation - must be in header for template instantiation

template<persistent_object T>
void PersistentObjectJsonManager::deserializeAllObjects(const std::string& filename, const std::string& root_key) {
	using namespace rm::rule::serial;

	// Create the appropriate serializer for type T
	auto serializer = createSerializer<T>();

	// Construct the full file path
	std::string full_path = data_directory_ + filename;

	// Create the file serializer and load the data
	JsonFileSerializer<T> file_serializer(*serializer, root_key, full_path);
	file_serializer.load();
}

template<persistent_object T>
void PersistentObjectJsonManager::serializeAllObjects(const std::string& filename, const std::string& root_key) {
	using namespace rm::rule::serial;

	// Create the appropriate serializer for type T
	auto serializer = createSerializer<T>();

	// Construct the full file path
	std::string full_path = data_directory_ + filename;

	// Create the file serializer and save the data
	JsonFileSerializer<T> file_serializer(*serializer, root_key, full_path);
	file_serializer.save();
}

template<persistent_object T>
std::unique_ptr<rule::serial::PersistentJsonSerializer<T>> PersistentObjectJsonManager::createSerializer() {
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
		// This is a special case as it is not a JSON file, so we need to use a different serializer and file serializer, so we will handle this separately in the load/save functions
		//} else if constexpr (std::is_same_v<T, table::TrainingPackageCostTable>) {
		//	return std::make_unique<TrainingPackageCostTableSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, TreasureCodeData>) {
		return std::make_unique<TreasureCodeSerializer>(object_manager_);
	} else if constexpr (std::is_same_v<T, WeaponTypeData>) {
		return std::make_unique<WeaponTypeSerializer>(object_manager_);
	} else {
		static_assert(sizeof(T) == 0, "No serializer defined for this persistent object type");
	}
}

} // namespace rm