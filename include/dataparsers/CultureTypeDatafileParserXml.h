#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>
#include <CultureTypeData.h>

/**
 * @class CultureTypeDatafileParserXml
 * @brief Process CultureTypeData objects for xml files
 *
 * This class can read a well-formed xml file creating CultureTypeData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * @see CultureTypeData
 * @see GameRuleDataCache
 */

class CultureTypeDatafileParserXml : public DatafileParserXml {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for CultureTypeData objects
	 * @param filename Path to the datafile to parse
	 */
	CultureTypeDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "CultureType", filename) {
		setRootNode("CultureTypeData.cultureTypes");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for CultureTypeData objects
	 */
	CultureTypeDatafileParserXml(GameRuleDataCache& cache) : CultureTypeDatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into CultureTypeData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to CultureTypeData objects and store in a data cache
	 */
	void parse() override;
};