#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <ProfessionData.h>

/**
 * @class ProfessionDatafileParserJson
 * @brief Process ProfessionData objects for json files
 *
 * This class can read a well-formed json file creating ProfessionData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see ProfessionData
 * @see GameRuleDataCache
 */

class ProfessionDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
//	using DatafileParserBoost::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for ProfessionData objects
	 * @param filename Path to the datafile to parse
	 */
	ProfessionDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Profession", filename) {
		setRootNode("professions");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for ProfessionData objects
	 */
	ProfessionDatafileParserJson(GameRuleDataCache& cache) : ProfessionDatafileParserJson(cache, "") {}

	/**
	 * @brief Write profession game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<ProfessionData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a profession
	 * @param id Id of the profession to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into ProfessionData objects
	 *
	 * Parse a boost::ptree containing the profession rule data, convert to ProfessionData objects and store in a data cache
	 */
	void parse() override;
};