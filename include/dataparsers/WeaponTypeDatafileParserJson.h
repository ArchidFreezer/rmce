#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <WeaponTypeData.h>

/**
 * @class WeaponTypeDatafileParserJson
 * @brief Process WeaponTypeData objects for json files
 *
 * This class can read a well-formed json file creating WeaponTypeData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see WeaponTypeData
 * @see GameRuleDataCache
 */

class WeaponTypeDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
//	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for WeaponTypeData objects
	 * @param filename Path to the datafile to parse
	 */
	WeaponTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "WeaponType", filename) {
		setRootNode("weapon-types");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for WeaponTypeData objects
	 */
	WeaponTypeDatafileParserJson(GameRuleDataCache& cache) : WeaponTypeDatafileParserJson(cache, "") {}

	/**
	 * @brief Write weapon type game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<WeaponTypeData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a weapon type
	 * @param id Id of the weapon type to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into WeaponTypeData objects
	 *
	 * Parse a boost::ptree containing the weapon type rule data, convert to WeaponTypeData objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;
};