#pragma once

#include <DatafileParserJson.h>
#include <RaceData.h>

namespace rm::rule::parser {

	/**
	 * @class RaceDatafileParserJson
	 * @brief Process RaceData objects for json files
	 *
	 * This class can read a well-formed json file creating RaceData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see RaceData
	 */

	class RaceDatafileParserJson : public DatafileParserJson {
	public:

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		RaceDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "Race", filename) {
			setRootNode("races");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		RaceDatafileParserJson(rm::PersistentObjectManager& object_manager) : RaceDatafileParserJson(object_manager, "") {}

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

} // namespace rm::rule::parser