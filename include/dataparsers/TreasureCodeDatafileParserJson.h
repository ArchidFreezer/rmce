#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <TreasureCodeData.h>

/**
 * @class TreasureCodeDatafileParserJson
 * @brief Process TreasureCodeData objects for json files
 *
 * This class can read a well-formed json file creating TreasureCodeData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see TreasureCodeData
 * @see GameRuleDataCache
 */

class TreasureCodeDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for TreasureCodeData objects
	 * @param filename Path to the datafile to parse
	 */
	TreasureCodeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "TreasureCode", filename) {
		setRootNode("treasure-codes");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for TreasureCodeData objects
	 */
	TreasureCodeDatafileParserJson(GameRuleDataCache& cache) : TreasureCodeDatafileParserJson(cache, "") {}

	/**
	 * @brief Write skill category game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<TreasureCodeData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a skill category
	 * @param id Id of the skill category to populate from
	 * @param datum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& datum) override;

private:
	/**
	 * @brief Parse a ptree into TreasureCodeData objects
	 *
	 * Parse a boost::ptree containing the skill category rule datas, convert to TreasureCodeData objects and store in a data cache
	 */
	void parse() override;
};