#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <CreaturePaceData.h>

namespace rm {

	/**
	 * @class CreaturePaceDatafileParserJson
	 * @brief Process CreaturePaceData objects for json files
	 *
	 * This class can read a well-formed json file creating CreaturePaceData objects from the contents, adding them to a
	 * #GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see CreaturePaceData
	 * @see #GameRuleDataCache
	 */
	class CreaturePaceDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for CreaturePaceData objects
		 * @param filename Path to the datafile to parse
		 */
		CreaturePaceDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "CreaturePace", filename) {
			setRootNode("creature-paces");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for CreaturePaceData objects
		 */
		CreaturePaceDatafileParserJson(GameRuleDataCache& cache) : CreaturePaceDatafileParserJson(cache, "") {}

		/**
		 * @brief Write creaturepace game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<CreaturePaceData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a creaturepace
		 * @param id Id of the creaturepace to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into CreaturePaceData objects
		 *
		 * Parse a boost::ptree containing the creaturepace rule datas, convert to CreaturePaceData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm