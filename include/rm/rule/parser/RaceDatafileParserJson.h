#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <RaceData.h>

namespace rm {

	/**
	 * @class RaceDatafileParserJson
	 * @brief Process RaceData objects for json files
	 *
	 * This class can read a well-formed json file creating RaceData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see RaceData
	 * @see GameRuleDataCache
	 */

	class RaceDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
	//	using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for RaceData objects
		 * @param filename Path to the datafile to parse
		 */
		RaceDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Race", filename) {
			setRootNode("races");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for RaceData objects
		 */
		RaceDatafileParserJson(GameRuleDataCache& cache) : RaceDatafileParserJson(cache, "") {}

		/**
		 * @brief Write race game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<RaceData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a race
		 * @param id Id of the race to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into RaceData objects
		 *
		 * Parse a boost::ptree containing the race rule data, convert to RaceData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm