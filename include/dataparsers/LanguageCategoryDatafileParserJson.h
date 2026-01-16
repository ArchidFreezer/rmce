#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

/**
 * @class LanguageCategoryDatafileParserJson
 * @brief Process #LanguageCategoryData objects for json files
 * 
 * This class can read a well-formed json file creating #LanguageCategoryData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 * 
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 * 
 * @see #LanguageCategoryData
 * @see #GameRuleDataCache
 */
class LanguageCategoryDatafileParserJson : public DatafileParserJson {
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

public:
	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param filename Path to the datafile to parse
	 */
	LanguageCategoryDatafileParserJson(GameRuleDataCache& cache, std::string_view filename);

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 */
	LanguageCategoryDatafileParserJson(GameRuleDataCache& cache);

	/**
	 * @brief Write language category game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override;

private:
	/**
	 * @brief Parse a ptree into #LanguageCategoryData objects
	 * 
	 * Parse a boost::ptree containing the language category rule datas, convert to #LanguageCategoryData objects and store in
	 * a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;
};