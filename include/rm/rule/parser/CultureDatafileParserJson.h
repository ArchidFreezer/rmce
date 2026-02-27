#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <CultureData.h>

namespace rm {

	/**
	 * @class CultureDatafileParserJson
	 * @brief Process CultureData objects for json files
	 *
	 * This class can read a well-formed json file creating CultureData objects from the contents, adding them to a
	 * #GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see CultureData
	 * @see #GameRuleDataCache
	 */
	class CultureDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for CultureData objects
		 * @param filename Path to the datafile to parse
		 */
		CultureDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Culture", filename) {
			setRootNode("cultures");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for CultureData objects
		 */
		CultureDatafileParserJson(GameRuleDataCache& cache) : CultureDatafileParserJson(cache, "") {}

		/**
		 * @brief Write culture game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<CultureData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a culture
		 * @param id Id of the culture to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into CultureData objects
		 *
		 * Parse a boost::ptree containing the culture rule datas, convert to CultureData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm