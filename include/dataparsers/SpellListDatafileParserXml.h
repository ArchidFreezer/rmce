#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>

/**
 * @class SpellListDatafileParserXml
 * @brief Process SpellListData objects from xml files
 *
 * This class can read a well-formed xml file creating SpellListData objects from the contents adding them to a GameRuleDataCache cache.
 *
 * @see SpellListData
 * @see GameRuleDataCache
 */

class SpellListDatafileParserXml : public DatafileParserXml {
public:

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SpellListData objects
	 * @param filename Path to the datafile to parse
	 */
	SpellListDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "SpellList", filename) {
		setRootNode("SpellListData.spellLists");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SpellList and SpecialSpellList objects
	 */
	SpellListDatafileParserXml(GameRuleDataCache& cache) : DatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into SpellListData objects
	 *
	 * Parse a boost::ptree containing the book rule data, convert to SpellListData objects and store in a data cache.
	 *
	 */
	void parse() override;
};