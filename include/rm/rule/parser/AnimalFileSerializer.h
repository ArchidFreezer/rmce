#pragma once

#include <string>
#include <AnimalData.h>
#include <AnimalSerializer.h>

namespace rm::rule::parser {

/**
 * @class AnimalFileSerializer
 * @brief Class to parse files containing animal data
 *
 * This class implements the PersistentObjectSerializer read and save functions to (de)serialize the animal data to and from a file.
 *
 * @see PersistentObjectSerializer
 */
class AnimalFileSerializer : public AnimalSerializer {
public:
	/**
	 * @brief Deleted default constructor to ensure initialisation of the base class.
	 */
	AnimalFileSerializer() = delete;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 * @param filename Path to the datafile to parse
	 */
	AnimalFileSerializer(rm::PersistentObjectManager& object_manager, const std::string_view filename) : AnimalSerializer(object_manager), filename_{filename} {
	}

	~AnimalFileSerializer() = default;                                                /** Default destructor */
	AnimalFileSerializer(const AnimalFileSerializer&) = default;                      /** Default copy constructor */
	AnimalFileSerializer& operator=(const AnimalFileSerializer&) noexcept = default;  /** Default copy assignment operator */
	AnimalFileSerializer(AnimalFileSerializer&&) = default;                           /** Default move constructor */
	AnimalFileSerializer& operator=(AnimalFileSerializer&& other) noexcept = default; /** Default move assignment operator */

	/**
	 * @brief Read animal data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 */
	void read(const std::string& filename) {
		std::ifstream is{filename};
		if (!is) {
			std::cerr << "Error opening file " << filename << " for reading.\n";
			return;
		}
		AnimalSerializer::read(is);
	}

	/**
	 * @brief Read animal data from file, convert to objects and store in the game rule data cache
	 *
	 * This function uses the filename provided in the constructor.
	 */
	void read() override {
		read(filename_);
	}

	/**
	 * @brief Write animal data from the cache to a file
	 * @param filename Path to the file to write the output to
	 */
	void save(const std::string& filename) {
		std::ofstream os{filename};
		if (!os) {
			std::cerr << "Error opening file " << filename << " for writing.\n";
			return;
		}
		AnimalSerializer::save<AnimalData>(os);
	}

	/**
	 * @brief Write animal data from the cache to a file
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