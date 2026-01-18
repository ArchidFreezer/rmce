#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

/**
 * @class AttackTableDatafileParserJson
 * @brief Process AttackTable objects for json files
 *
 * This class can read a well-formed json file creating AttackTable objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see AttackTable
 * @see GameRuleDataCache
 */

class AttackTableDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AttackTable objects
	 * @param filename Path to the datafile to parse
	 */
	AttackTableDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "AttackTable", filename) {
		setRootNode("attack_tables");
	}


	/**
	 * @brief Constructor
	 * @param cache Cache to use for AttackTable objects
	 */
	AttackTableDatafileParserJson(GameRuleDataCache& cache) : AttackTableDatafileParserJson(cache, "") {}

	/**
	 * @brief Write attack tables from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override;

private:
	/**
	 * @brief Parse a ptree into AttackTable objects
	 *
	 * Parse a boost::ptree containing the attack table data, convert to AttackTable objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;
};