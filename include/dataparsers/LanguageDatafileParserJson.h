#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <LanguageData.h>

/**
 * @class LanguageDatafileParserJson
 * @brief Process LanguageData objects for json files
 *
 * This class can read a well-formed json file creating LanguageData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see LanguageData
 * @see #GameRuleDataCache
 */
class LanguageDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param filename Path to the datafile to parse
	 */
	LanguageDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Language", filename) {
		setRootNode("languages");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 */
	LanguageDatafileParserJson(GameRuleDataCache& cache) : LanguageDatafileParserJson(cache, "") {}

	/**
	 * @brief Write language game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<LanguageData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a language
	 * @param id Id of the language to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into LanguageData objects
	 *
	 * Parse a boost::ptree containing the language rule datas, convert to LanguageData objects and store in a data cache
	 */
	void parse() override;
};