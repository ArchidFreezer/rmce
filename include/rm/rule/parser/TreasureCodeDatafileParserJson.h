#pragma once

#include <DatafileParserJson.h>
#include <TreasureCodeData.h>

namespace rm::rule::parser {

	/**
	 * @class TreasureCodeDatafileParserJson
	 * @brief Process TreasureCodeData objects for json files
	 *
	 * This class can read a well-formed json file creating TreasureCodeData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see TreasureCodeData
	 */

	class TreasureCodeDatafileParserJson : public DatafileParserJson {
	public:

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		TreasureCodeDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "TreasureCode", filename) {
			setRootNode("treasure-codes");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		TreasureCodeDatafileParserJson(rm::PersistentObjectManager& object_manager) : TreasureCodeDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write skill category game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<TreasureCodeData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a skill category
		 * @param id Id of the skill category to populate from
		 * @param datum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& datum) override;

	private:
		/**
		 * @brief Parse a ptree into TreasureCodeData objects
		 *
		 * Parse a boost::ptree containing the skill category rule datas, convert to TreasureCodeData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser