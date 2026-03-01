#pragma once

#include <DatafileParserJson.h>
#include <ArmourTypeData.h>

namespace rm::rule::parser {

	/**
	 * @class ArmourTypeDatafileParserJson
	 * @brief Process ArmourTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating ArmourTypeData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see ArmourTypeData
	 */

	class ArmourTypeDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		ArmourTypeDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "ArmourType", filename) {
			setRootNode("armour-types");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		ArmourTypeDatafileParserJson(rm::PersistentObjectManager& object_manager) : ArmourTypeDatafileParserJson(object_manager, "") {}

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

} // namespace rm::rule::parser