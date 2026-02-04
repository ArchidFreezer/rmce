#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>

/**
 * @class WeaponTypeDatafileParserXml
 * @brief Process WeaponTypeData objects from xml files
 *
 * This class can read a well-formed xml file creating WeaponTypeData objects from the contents adding them to a GameRuleDataCache cache.
 *
 * @see WeaponTypeData
 * @see GameRuleDataCache
 */

class WeaponTypeDatafileParserXml : public DatafileParserXml {
public:

	/**
	 * @brief Constructor
	 * @param cache Cache to use for WeaponTypeData objects
	 * @param filename Path to the datafile to parse
	 */
	WeaponTypeDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "WeaponType", filename) {
		setRootNode("WeaponTypeData.weaponTypes");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for WeaponType and SpecialWeaponType objects
	 */
	WeaponTypeDatafileParserXml(GameRuleDataCache& cache) : DatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into WeaponTypeData objects
	 *
	 * Parse a boost::ptree containing the book rule data, convert to WeaponTypeData objects and store in a data cache.
	 *
	 */
	void parse() override;
};