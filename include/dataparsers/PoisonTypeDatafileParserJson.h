#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <PoisonTypeData.h>

/**
 * @class PoisonTypeDatafileParserJson
 * @brief Process PoisonTypeData objects for json files
 *
 * This class can read a well-formed json file creating PoisonTypeData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see PoisonTypeData
 * @see #GameRuleDataCache
 */
class PoisonTypeDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for PoisonTypeData objects
	 * @param filename Path to the datafile to parse
	 */
	PoisonTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "PoisonType", filename) {
		setRootNode("poison-types");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for PoisonTypeData objects
	 */
	PoisonTypeDatafileParserJson(GameRuleDataCache& cache) : PoisonTypeDatafileParserJson(cache, "") {}

	/**
	 * @brief Write poisontype game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<PoisonTypeData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a poisontype
	 * @param id Id of the poisontype to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into PoisonTypeData objects
	 *
	 * Parse a boost::ptree containing the poisontype rule datas, convert to PoisonTypeData objects and store in a data cache
	 */
	void parse() override;
};