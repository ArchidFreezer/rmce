#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>
#include <AnimalData.h>

/**
 * @class AnimalDatafileParserXml
 * @brief Process AnimalData objects for xml files
 *
 * This class can read a well-formed xml file creating AnimalData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * @see AnimalData
 * @see GameRuleDataCache
 */

class AnimalDatafileParserXml : public DatafileParserXml {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AnimalData objects
	 * @param filename Path to the datafile to parse
	 */
	AnimalDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "Animal", filename) {
		setRootNode("AnimalTypeData.animalTypes");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for AnimalData objects
	 */
	AnimalDatafileParserXml(GameRuleDataCache& cache) : AnimalDatafileParserXml(cache, "") {}

private:
	/**
	 * @brief Parse a ptree into AnimalData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to AnimalData objects and store in a data cache
	 */
	void parse() override;

	void buildCreatureBonusXpTable();

	void buildCreatureLevelVarianceTable();

};