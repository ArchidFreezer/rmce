#pragma once

#include <string>

namespace rm::rule::parser {

/**
 * @class PersistentObjectFileSerializer
 * @brief Class to parse files containing data for a specific rule type
 *
 * This class implements the PersistentObjectSerializer read and save functions to (de)serialize the data to and from a file.
 *
 * @tparam BaseSerializer Class of the base serializer to use for the (de)serialization of the data. Must be derived from PersistentObjectSerializer.
 *
 * @see PersistentObjectSerializer
 */
template<persistent_object_serializer BaseSerializer>
class PersistentObjectFileSerializer : public BaseSerializer {
public:
	/**
	 * @brief Deleted default constructor to ensure initialisation of the base class.
	 */
	PersistentObjectFileSerializer() = delete;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 * @param filename Path to the datafile to parse
	 */
	PersistentObjectFileSerializer(rm::PersistentObjectManager& object_manager, const std::string_view filename)
	    : BaseSerializer(object_manager), filename_{filename} {
	}

	~PersistentObjectFileSerializer() = default;                                                          /** Default destructor */
	PersistentObjectFileSerializer(const PersistentObjectFileSerializer&) = default;                      /** Default copy constructor */
	PersistentObjectFileSerializer& operator=(const PersistentObjectFileSerializer&) noexcept = default;  /** Default copy assignment operator */
	PersistentObjectFileSerializer(PersistentObjectFileSerializer&&) = default;                           /** Default move constructor */
	PersistentObjectFileSerializer& operator=(PersistentObjectFileSerializer&& other) noexcept = default; /** Default move assignment operator */

	/**
	 * @brief Read book data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 */
	void read(const std::string& filename) {
		std::ifstream is{filename};
		if (!is) {
			std::cerr << "Error opening file " << filename << " for reading.\n";
			return;
		}
		BaseSerializer::read(is);
	}

	/**
	 * @brief Read book data from file, convert to objects and store in the game rule data cache
	 *
	 * This function uses the filename provided in the constructor.
	 */
	void read() override {
		read(filename_);
	}

	/**
	 * @brief Write book data from the cache to a file
	 * @param filename Path to the file to write the output to
	 */
	void save(const std::string& filename) {
		std::ofstream os{filename};
		if (!os) {
			std::cerr << "Error opening file " << filename << " for writing.\n";
			return;
		}
		BaseSerializer::save<BaseSerializer::DataClass>(os);
	}

	/**
	 * @brief Write book data from the cache to a file
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
