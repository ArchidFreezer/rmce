#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <TrainingPackageData.h>

/**
 * @class TrainingPackageDatafileParserJson
 * @brief Process TrainingPackageData objects for json files
 *
 * This class can read a well-formed json file creating TrainingPackageData objects from the contents, adding them to a
 * GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see TrainingPackageData
 * @see GameRuleDataCache
 */

class TrainingPackageDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
//	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for TrainingPackageData objects
	 * @param filename Path to the datafile to parse
	 */
	TrainingPackageDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Trainingpackage", filename) {
		setRootNode("training-packages");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for TrainingPackageData objects
	 */
	TrainingPackageDatafileParserJson(GameRuleDataCache& cache) : TrainingPackageDatafileParserJson(cache, "") {}

	/**
	 * @brief Write trainingpackage game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<TrainingPackageData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a trainingpackage
	 * @param id Id of the trainingpackage to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into TrainingPackageData objects
	 *
	 * Parse a boost::ptree containing the trainingpackage rule data, convert to TrainingPackageData objects and store in a data cache
	 */
	void parse() override;

	/**
	 * @brief Parse a boost ptree containing a map of qualifiers and reductions into a std::map of the same
	 *
	 * The boost ptree expected by this function should be derived from the following json format:
	 * @code{.json}
	 * "qualifiers": [
	 *   {
	 *     "qualifier": "description of the qualifier",
	 *     "reduction": reduction in package cost if the qualifier is met
	 *   }
	 * ]
	 * @endcode
	 *
	 * @param qualifiers Boost ptree containing the map of qualifiers and reductions
	 * @return Map of qualifiers and reductions
	 */
	std::map<std::string, int> parseQualifiers(boost::optional<const pt::ptree&> qualifiers);

	/**
	 * @brief Parse a std::map of qualifiers and reductions into a boost ptree
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "qualifiers": [
	 *   {
	 *     "qualifier": "description of the qualifier",
	 *     "reduction": reduction in package cost if the qualifier is met
	 *   }
	 * ]
	 * @endcode
	 *
	 * @param map Map of qualifiers and reductions
	 * @return Boost ptree containing the map of qualifiers and reductions
	 */
	const pt::ptree getQualifiersTree(TrainingPackageData& game_data);

	std::vector<std::pair<std::string, int>> parseSpecials(boost::optional<const pt::ptree&> specials);

	const pt::ptree getSpecialsTree(TrainingPackageData& game_data);
};