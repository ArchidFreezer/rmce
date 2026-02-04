#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <SkillProgressionTypeData.h>

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
	SkillProgressionTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "SkillProgressionType", filename) {
		setRootNode("skillProgressions");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillProgressionTypeData objects
	 */
	SkillProgressionTypeDatafileParserJson(GameRuleDataCache& cache) : SkillProgressionTypeDatafileParserJson(cache, "") {}

	/**
	 * @brief Write book game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<SkillProgressionTypeData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a skill progression type
	 * @param id Id of the skill progression type to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into SkillProgressionTypeData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to SkillProgressionTypeData objects and store in a data cache
	 */
	void parse() override;
};