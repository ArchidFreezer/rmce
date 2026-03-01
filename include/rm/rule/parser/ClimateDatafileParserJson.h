#pragma once

#include <DatafileParserJson.h>
#include <ClimateData.h>

namespace rm::rule::parser {

	/**
	 * @class ClimateDatafileParserJson
	 * @brief Process ClimateData objects for json files
	 *
	 * This class can read a well-formed json file creating ClimateData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see ClimateData
	 */

	class ClimateDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		ClimateDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "Climate", filename) {
			setRootNode("climates");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		ClimateDatafileParserJson(rm::PersistentObjectManager& object_manager) : ClimateDatafileParserJson(object_manager, "") {}

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