#pragma once

#include <DatafileParserJson.h>
#include <table/SpecialAttackTable.h>

namespace rm::rule::parser {

	/**
	 * @class SpecialAttackTableDatafileParserJson
	 * @brief Process SpecialAttackTable objects for json files
	 *
	 * This class can read a well-formed json file creating SpecialAttackTable objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see SpecialAttackTable
	 */

	class SpecialAttackTableDatafileParserJson : public DatafileParserJson {
	public:

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		SpecialAttackTableDatafileParserJson(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "SpecialAttackTable", filename) {
			setRootNode("attack-tables");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		SpecialAttackTableDatafileParserJson(rm::GameRuleDataFactory& object_manager) : SpecialAttackTableDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write attack tables from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<table::SpecialAttackTable>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a special attack table
		 * @param id Id of the special attack table to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into SpecialAttackTable objects
		 *
		 * Parse a boost::ptree containing the attack table data, convert to SpecialAttackTable objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser