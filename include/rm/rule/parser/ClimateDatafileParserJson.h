#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <ClimateData.h>

namespace rm::rule::parser {

	/**
	 * @class ClimateDatafileParserJson
	 * @brief Process ClimateData objects for json files
	 *
	 * This class can read a well-formed json file creating ClimateData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see ClimateData
	 * @see GameRuleDataCache
	 */

	class ClimateDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for ClimateData objects
		 * @param filename Path to the datafile to parse
		 */
		ClimateDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Climate", filename) {
			setRootNode("climates");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for ClimateData objects
		 */
		ClimateDatafileParserJson(GameRuleDataCache& cache) : ClimateDatafileParserJson(cache, "") {}

		/**
		 * @brief Write climate game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<ClimateData>(filename); }


		/**
		 * @brief Populate the given boost tree with the data from a climate
		 * @param id Id of the climate to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into ClimateData objects
		 *
		 * Parse a boost::ptree containing the climate rule datas, convert to ClimateData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser