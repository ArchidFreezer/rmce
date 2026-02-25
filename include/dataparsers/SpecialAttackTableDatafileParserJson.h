#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <table/SpecialAttackTable.h>

/**
 * @class SpecialAttackTableDatafileParserJson
 * @brief Process SpecialAttackTable objects for json files
 *
 * This class can read a well-formed json file creating SpecialAttackTable objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see SpecialAttackTable
 * @see GameRuleDataCache
 */

class SpecialAttackTableDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParserBoost::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SpecialAttackTable objects
	 * @param filename Path to the datafile to parse
	 */
	SpecialAttackTableDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "SpecialAttackTable", filename) {
		setRootNode("attack-tables");
	}


	/**
	 * @brief Constructor
	 * @param cache Cache to use for SpecialAttackTable objects
	 */
	SpecialAttackTableDatafileParserJson(GameRuleDataCache& cache) : SpecialAttackTableDatafileParserJson(cache, "") {}

	/**
	 * @brief Write attack tables from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<SpecialAttackTable>(filename); }

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