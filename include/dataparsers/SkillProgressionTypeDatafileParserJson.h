#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

/**
 * @class SkillProgressionTypeDatafileParserJson
 * @brief Process SkillProgressionTypeData objects from json files
 *
 * This class can read a well-formed json file creating SkillProgressionTypeData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see SkillProgressionTypeData
 * @see GameRuleDataCache
 */

class SkillProgressionTypeDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillProgressionTypeData objects
	 * @param filename Path to the datafile to parse
	 */
	SkillProgressionTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename);

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillProgressionTypeData objects
	 */
	SkillProgressionTypeDatafileParserJson(GameRuleDataCache& cache);

	/**
	 * @brief Write book game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename);

private:
	/**
	 * @brief Parse a ptree into SkillProgressionTypeData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to SkillProgressionTypeData objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only);
};