#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>
#include <RaceData.h>

/**
 * @class RaceDatafileParserXml
 * @brief Process RaceData objects for xml files
 *
 * This class can read a well-formed xml file creating RaceData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * @see RaceData
 * @see GameRuleDataCache
 */

class RaceDatafileParserXml : public DatafileParserXml {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for RaceData objects
	 * @param filename Path to the datafile to parse
	 */
	RaceDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "Race", filename) {
		setRootNode("RaceData.races");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for RaceData objects
	 */
	RaceDatafileParserXml(GameRuleDataCache& cache) : RaceDatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into RaceData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to RaceData objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;
};