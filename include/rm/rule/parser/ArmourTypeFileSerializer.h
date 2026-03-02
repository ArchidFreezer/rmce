#pragma once

#include <string>
#include <ArmourTypeData.h>
#include <ArmourTypeSerializer.h>

namespace rm::rule::parser {

/**
 * @class ArmourTypeFileSerializer
 * @brief Class to parse files containing armour type data
 *
 * This class implements the PersistentObjectSerializer read and save functions to (de)serialize the armour type data to and from a file.
 *
 * @see PersistentObjectSerializer
 */
class ArmourTypeFileSerializer : public ArmourTypeSerializer {
public:
	/**
	 * @brief Deleted default constructor to ensure initialisation of the base class.
	 */
	ArmourTypeFileSerializer() = delete;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 * @param filename Path to the datafile to parse
	 */
	ArmourTypeFileSerializer(rm::PersistentObjectManager& object_manager, const std::string_view filename) : ArmourTypeSerializer(object_manager), filename_{filename} {
	}

	~ArmourTypeFileSerializer() = default;                                              /** Default destructor */
	ArmourTypeFileSerializer(const ArmourTypeFileSerializer&) = default;                      /** Default copy constructor */
	ArmourTypeFileSerializer& operator=(const ArmourTypeFileSerializer&) noexcept = default;  /** Default copy assignment operator */
	ArmourTypeFileSerializer(ArmourTypeFileSerializer&&) = default;                           /** Default move constructor */
	ArmourTypeFileSerializer& operator=(ArmourTypeFileSerializer&& other) noexcept = default; /** Default move assignment operator */

	/**
	 * @brief Read armourtype data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 */
	void read(const std::string& filename) {
		std::ifstream is{filename};
		if (!is) {
			std::cerr << "Error opening file " << filename << " for reading.\n";
			return;
		}
		ArmourTypeSerializer::read(is);
	}

	/**
	 * @brief Read armour type data from file, convert to objects and store in the game rule data cache
	 *
	 * This function uses the filename provided in the constructor.
	 */
	void read() override {
		read(filename_);
	}

	/**
	 * @brief Write armour type data from the cache to a file
	 * @param filename Path to the file to write the output to
	 */
	void save(const std::string& filename) {
		std::ofstream os{filename};
		if (!os) {
			std::cerr << "Error opening file " << filename << " for writing.\n";
			return;
		}
		ArmourTypeSerializer::save<ArmourTypeData>(os);
	}

	/**
	 * @brief Write armour type data from the cache to a file
	 *
	 * This function uses the filename provided in the constructor.
	 */
	void save() override {
		save(filename_);
	}

private:
	const std::string filename_{}; /**< Path to the datafile to parse */
};

} // namespace rm::rule::parser