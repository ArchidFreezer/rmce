#pragma once

#include <DatafileParserJson.h>
#include <CultureData.h>

namespace rm::rule::parser {

	/**
	 * @class CultureDatafileParserJson
	 * @brief Process CultureData objects for json files
	 *
	 * This class can read a well-formed json file creating CultureData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see CultureData
	 */
	class CultureDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		CultureDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "Culture", filename) {
			setRootNode("cultures");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		CultureDatafileParserJson(rm::PersistentObjectManager& object_manager) : CultureDatafileParserJson(object_manager, "") {}

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

} // namespace rm::rule::parser