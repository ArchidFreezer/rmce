#pragma once

#include <string_view>
#include <DatafileParser.h>

namespace rm::rule::parser {

	/**
	 * @class TrainingPackageCostTableDatafileParser
	 * @brief Class to parse files containing training package cost table data
	 *
	 * This class implements the DatafileParser::read function to read the file into the lookup table in the data cache.
	 *
	 * @see DatafileParser
	 */
	class TrainingPackageCostTableDatafileParser : public DatafileParser {
	public:
		using DatafileParser::read;
		/**
		 * @brief Deleted default constructor to ensure initialisation of the base class.
		 */
		TrainingPackageCostTableDatafileParser() = delete;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		TrainingPackageCostTableDatafileParser(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParser(object_manager, "TrainingPackageCostTable", filename) {}

		/**
		 * @brief Read training package cost table data from file, convert to objects and store in the game rule data cache
		 * @param filename Path to the file to read the output from
		 */
		void read(const std::string& filename) override;

		/**
		 * @brief Write training package cost table data from the cache to a file
		 * @param filename Path to the file to write the output to
		 */
		void save(const std::string& filename) override;

	};

} // namespace rm::rule::parser