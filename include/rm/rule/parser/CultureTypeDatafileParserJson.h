#pragma once

#include <DatafileParserJson.h>
#include <CultureTypeData.h>

namespace rm::rule::parser {

	/**
	 * @class CultureTypeDatafileParserJson
	 * @brief Process CultureTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating CultureTypeData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see CultureTypeData
	 */

	class CultureTypeDatafileParserJson : public DatafileParserJson {
	public:
		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		CultureTypeDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "CultureType", filename) {
			setRootNode("culture-types");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		CultureTypeDatafileParserJson(rm::PersistentObjectManager& object_manager) : CultureTypeDatafileParserJson(object_manager, "") {}

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