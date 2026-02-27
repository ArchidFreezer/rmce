#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <ArmourTypeData.h>

namespace rm {

	/**
	 * @class ArmourTypeDatafileParserJson
	 * @brief Process ArmourTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating ArmourTypeData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see ArmourTypeData
	 * @see GameRuleDataCache
	 */

	class ArmourTypeDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for ArmourTypeData objects
		 * @param filename Path to the datafile to parse
		 */
		ArmourTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "ArmourType", filename) {
			setRootNode("armour-types");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for ArmourTypeData objects
		 */
		ArmourTypeDatafileParserJson(GameRuleDataCache& cache) : ArmourTypeDatafileParserJson(cache, "") {}

		/**
		 * @brief Write the game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<ArmourTypeData>(filename); }

		/**
		 * @brief Populate the given boost tree with the game rule data
		 * @param id Id of the data to populate from
		 * @param datum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& datum) override;

	private:
		/**
		 * @brief Parse a ptree into ArmourTypeData objects
		 *
		 * Parse a boost::ptree containing the rule data, convert to ArmourTypeData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm