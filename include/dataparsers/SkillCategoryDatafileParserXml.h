#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>

/**
 * @class SkillCategoryDatafileParserXml
 * @brief Process SkillCategory and SpecialSkillCategory objects for xml files
 *
 * This class can read a well-formed json file creating SkillCategory and SpecialSkillCategory objects from the contents,
 * adding them to a GameRuleDataCache cache.
 *
 * @see SkillCategoryData
 * @see GameRuleDataCache
 */

class SkillCategoryDatafileParserXml : public DatafileParserXml {
public:

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillCategoryData objects
	 * @param filename Path to the datafile to parse
	 */
	SkillCategoryDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "SkillCategory", filename) {
		setRootNode("SkillCategoryData.skillCategories");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillCategoryData objects
	 */
	SkillCategoryDatafileParserXml(GameRuleDataCache& cache) : DatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into SkillCategoryData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to SkillCategoryData objects and store
	 * in a data cache.
	 *
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;

	/**
	 * @brief Gets the skill category form the xml name tag value
	 * 
	 * The tag value in the xml has the skill group and skill category names separated by a pipe '|' symbol
	 * 
	 * @param name std::string Name tag value containing both group and category names
	 * @return std::string containing the category name
	 */
	std::string parseName(const std::string& name);

};