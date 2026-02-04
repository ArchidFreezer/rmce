#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>

/**
 * @class SkillDatafileParserXml
 * @brief Process SkillData objects from xml files
 *
 * This class can read a well-formed xml file creating SkillData objects from the contents adding them to a GameRuleDataCache cache.
 *
 * @see SkillData
 * @see GameRuleDataCache
 */

class SkillDatafileParserXml : public DatafileParserXml {
public:

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillData objects
	 * @param filename Path to the datafile to parse
	 */
	SkillDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "Skill", filename) {
		setRootNode("SkillData.skills");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for SkillData objects
	 */
	SkillDatafileParserXml(GameRuleDataCache& cache) : DatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into SkillData objects
	 *
	 * Parse a boost::ptree containing the book rule data, convert to SkillData objects and store in a data cache.
	 *
	 */
	void parse() override;

	/**
	 * @brief Gets the skill category from the xml value
	 *
	 * The tag value in the xml has the skill group and skill category names separated by a pipe '|' symbol
	 *
	 * @param category std::string Tag value containing both group and category names
	 * @return std::string containing the category name
	 */
	std::string parseCategory(const std::string& category);

};