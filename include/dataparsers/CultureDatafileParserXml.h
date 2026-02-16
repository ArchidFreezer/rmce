#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>
#include <CultureData.h>

/**
 * @class CultureDatafileParserXml
 * @brief Process CultureData objects for xml files
 *
 * This class can read a well-formed xml file creating CultureData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * @see CultureData
 * @see GameRuleDataCache
 */

class CultureDatafileParserXml : public DatafileParserXml {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for CultureData objects
	 * @param filename Path to the datafile to parse
	 */
	CultureDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "Culture", filename) {
		setRootNode("CultureData.cultures");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for CultureData objects
	 */
	CultureDatafileParserXml(GameRuleDataCache& cache) : CultureDatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into CultureData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to CultureData objects and store in a data cache
	 */
	void parse() override;

	const SubcategoriedSkillData& getSubcategory(const std::string& val);
};