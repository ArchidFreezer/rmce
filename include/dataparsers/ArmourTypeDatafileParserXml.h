#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>
#include <ArmourTypeData.h>

/**
 * @class ArmourTypeDatafileParserXml
 * @brief Process ArmourTypeData objects for xml files
 *
 * This class can read a well-formed xml file creating ArmourTypeData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * @see ArmourTypeData
 * @see GameRuleDataCache
 */

class ArmourTypeDatafileParserXml : public DatafileParserXml {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for ArmourTypeData objects
	 * @param filename Path to the datafile to parse
	 */
	ArmourTypeDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "ArmourType", filename) {
		setRootNode("ArmourTypeData.armourTypes");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for ArmourTypeData objects
	 */
	ArmourTypeDatafileParserXml(GameRuleDataCache& cache) : ArmourTypeDatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into ArmourTypeData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to ArmourTypeData objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;
};