#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <CultureTypeData.h>

namespace rm::rule::parser {

	/**
	 * @class CultureTypeDatafileParserJson
	 * @brief Process CultureTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating CultureTypeData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see CultureTypeData
	 * @see GameRuleDataCache
	 */

	class CultureTypeDatafileParserJson : public DatafileParserJson {
	public:
		/**
		 * @brief Constructor
		 * @param cache Cache to use for CultureTypeData objects
		 * @param filename Path to the datafile to parse
		 */
		CultureTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "CultureType", filename) {
			setRootNode("culture-types");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for CultureTypeData objects
		 */
		CultureTypeDatafileParserJson(GameRuleDataCache& cache) : CultureTypeDatafileParserJson(cache, "") {}

		/**
		 * @brief Write culturetype game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<CultureTypeData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a culturetype
		 * @param id Id of the culturetype to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into CultureTypeData objects
		 *
		 * Parse a boost::ptree containing the culturetype rule data, convert to CultureTypeData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser