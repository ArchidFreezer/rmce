#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <SpellListData.h>

namespace rm {

	/**
	 * @class SpellListDatafileParserJson
	 * @brief Process SpellListData objects for json files
	 *
	 * This class can read a well-formed json file creating SpellListData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see SpellListData
	 * @see GameRuleDataCache
	 */

	class SpellListDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
	//	using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for SpellListData objects
		 * @param filename Path to the datafile to parse
		 */
		SpellListDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "SpellList", filename) {
			setRootNode("spell-lists");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for SpellListData objects
		 */
		SpellListDatafileParserJson(GameRuleDataCache& cache) : SpellListDatafileParserJson(cache, "") {}

		/**
		 * @brief Write spell list game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<SpellListData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a spell list
		 * @param id Id of the spell list to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into SpellListData objects
		 *
		 * Parse a boost::ptree containing the spell list rule data, convert to SpellListData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm