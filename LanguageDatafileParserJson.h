#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

/**
 * @class LanguageDatafileParserJson
 * @brief Process #LanguageData objects for json files
 *
 * This class can read a well-formed json file creating #LanguageData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see #LanguageData
 * @see #GameRuleDataCache
 */
class LanguageDatafileParserJson : public DatafileParserJson {
public:
	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 */
	LanguageDatafileParserJson(GameRuleDataCache& cache);

	/**
	 * @brief Write language game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename);

private:
	/**
	 * @brief Parse a ptree into #LanguageData objects
	 *
	 * Parse a boost::ptree containing the language rule datas, convert to #LanguageData objects and store in a data cache
	 */
	void parse();
};