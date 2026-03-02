#pragma once

#include <string>
#include <TrainingPackageCostTableDataParser.h>

namespace rm::rule::parser {

	/**
	 * @class TrainingPackageCostTableDatafileParser
	 * @brief Class to parse files containing training package cost table data
	 *
	 * This class implements the DatafileParser::read function to read the file into the lookup table in the data cache.
	 *
	 * @see DatafileParser
	 */
	class TrainingPackageCostTableDatafileParser : public TrainingPackageCostTableDataParser {
	public:

		/**
		 * @brief Deleted default constructor to ensure initialisation of the base class.
		 */
		TrainingPackageCostTableDatafileParser() = delete;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		TrainingPackageCostTableDatafileParser(rm::PersistentObjectManager& object_manager, const std::string_view filename) : TrainingPackageCostTableDataParser(object_manager), filename_{ filename } {}

		~TrainingPackageCostTableDatafileParser() = default; /** Default destructor for polymorphism */
		TrainingPackageCostTableDatafileParser(const TrainingPackageCostTableDatafileParser&) = default; /** Default copy constructor */
		TrainingPackageCostTableDatafileParser& operator=(const TrainingPackageCostTableDatafileParser&) noexcept = default; /** Default copy assignment operator */
		TrainingPackageCostTableDatafileParser(TrainingPackageCostTableDatafileParser&&) = default; /** Default move constructor */
		TrainingPackageCostTableDatafileParser& operator=(TrainingPackageCostTableDatafileParser&& other) noexcept = default; /** Default move assignment operator */

		/**
		 * @brief Read training package cost table data from file, convert to objects and store in the game rule data cache
		 * @param filename Path to the file to read the output from
		 */
		void read(const std::string& filename) {
			std::ifstream is{ filename };
			if (!is) {
				std::cerr << "Error opening file " << filename << " for reading.\n";
				return;
			}
			TrainingPackageCostTableDataParser::read(is);
		}

		/**
		 * @brief Read training package cost table data from file, convert to objects and store in the game rule data cache
		 *
		 * This function uses the filename provided in the constructor.
		 */
		void read() override { read(filename_); }

		/**
		 * @brief Write training package cost table data from the cache to a file
		 * @param filename Path to the file to write the output to
		 */
		void save(const std::string& filename) {
			std::ofstream os{ filename };
			if (!os) {
				std::cerr << "Error opening file " << filename << " for writing.\n";
				return;
			}
			TrainingPackageCostTableDataParser::save(os);
		}

		/**
		 * @brief Write training package cost table data from the cache to a file
		 *
		 * This function uses the filename provided in the constructor.
		 */
		void save() override { save(filename_); }

	private:
		const std::string filename_{}; /**< Path to the datafile to parse */

	};

} // namespace rm::rule::parser