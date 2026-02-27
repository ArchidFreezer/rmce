#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <DiseaseData.h>

/**
 * @class DiseaseDatafileParserJson
 * @brief Process DiseaseData objects for json files
 *
 * This class can read a well-formed json file creating DiseaseData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see DiseaseData
 * @see #GameRuleDataCache
 */
class DiseaseDatafileParserJson : public DatafileParserJson {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParserBoost::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for DiseaseData objects
	 * @param filename Path to the datafile to parse
	 */
	DiseaseDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Disease", filename) {
		setRootNode("diseases");
	}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for DiseaseData objects
	 */
	DiseaseDatafileParserJson(GameRuleDataCache& cache) : DiseaseDatafileParserJson(cache, "") {}

	/**
	 * @brief Write disease game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<DiseaseData>(filename); }

	/**
	 * @brief Populate the given boost tree with the data from a disease
	 * @param id Id of the disease to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into DiseaseData objects
	 *
	 * Parse a boost::ptree containing the disease rule datas, convert to DiseaseData objects and store in a data cache
	 */
	void parse() override;
};