#pragma once

#include <DatafileParserJson.h>
#include <PoisonData.h>

namespace rm::rule::parser {

	/**
	 * @class PoisonDatafileParserJson
	 * @brief Process PoisonData objects for json files
	 *
	 * This class can read a well-formed json file creating PoisonData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see PoisonData
	 */
	class PoisonDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		PoisonDatafileParserJson(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "Poison", filename) {
			setRootNode("poisons");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		PoisonDatafileParserJson(rm::GameRuleDataFactory& object_manager) : PoisonDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write poison game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<PoisonData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a poison
		 * @param id Id of the poison to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into PoisonData objects
		 *
		 * Parse a boost::ptree containing the poison rule datas, convert to PoisonData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser