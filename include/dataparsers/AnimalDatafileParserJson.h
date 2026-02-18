#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <AnimalData.h>

/**
 * @class AnimalDatafileParserJson
 * @brief Process AnimalData objects for json files
 *
 * This class can read a well-formed json file creating AnimalData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see AnimalData
 * @see #GameRuleDataCache
 */
class AnimalDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AnimalData objects
	 * @param filename Path to the datafile to parse
	 */
	AnimalDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Animal", filename) {
		setRootNode("animals");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AnimalData objects
	 */
	AnimalDatafileParserJson(GameRuleDataCache& cache) : AnimalDatafileParserJson(cache, "") {}

	/**
	 * @brief Write animal game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<AnimalData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a animal
	 * @param id Id of the animal to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into AnimalData objects
	 *
	 * Parse a boost::ptree containing the animal rule datas, convert to AnimalData objects and store in a data cache
	 */
	void parse() override;

	/**
	 * @brief Build the lookup table that maps the bonus XP code to a bonus XP value
	 *
	 * This is used to determine how much bonus XP is awarded for killing an animal based on its bonus XP code.
	 */
	void buildCreatureBonusXpTable();
};