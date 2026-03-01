#pragma once

#include <DatafileParserJson.h>
#include <WeaponTypeData.h>

namespace rm::rule::parser {

	/**
	 * @class WeaponTypeDatafileParserJson
	 * @brief Process WeaponTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating WeaponTypeData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see WeaponTypeData
	 */

	class WeaponTypeDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
	//	using DatafileParser::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		WeaponTypeDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "WeaponType", filename) {
			setRootNode("weapon-types");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		WeaponTypeDatafileParserJson(rm::PersistentObjectManager& object_manager) : WeaponTypeDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write weapon type game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<WeaponTypeData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a weapon type
		 * @param id Id of the weapon type to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into WeaponTypeData objects
		 *
		 * Parse a boost::ptree containing the weapon type rule data, convert to WeaponTypeData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser