#pragma once

#include "DatafileParserXml.h"
#include "GameRuleDataCache.h"

/**
 * @class AttackTableDatafileParserXml
 * @brief Process AttackTable and SpecialAttackTable objects for xml files
 *
 * This class can read a well-formed json file creating AttackTable and SpecialAttackTable objects from the contents,
 * adding them to a GameRuleDataCache cache.
 *
 * @see AttackTable
 * @see SpecialAttackTable
 * @see GameRuleDataCache
 */

class AttackTableDatafileParserXml : public DatafileParserXml {
public:

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AttackTable and SpecialAttackTable objects
	 * @param filename Path to the datafile to parse
	 */
	AttackTableDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "AttackTable", filename) {
		setRootNode("AttackTableData.attackTables");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AttackTable and SpecialAttackTable objects
	 */
	AttackTableDatafileParserXml(GameRuleDataCache& cache) : DatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into AttackTable and SpecialAttackTable objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to AttackTable and SpecialAttackTable objects and store
	 * in a data cache.
	 * 
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;

};