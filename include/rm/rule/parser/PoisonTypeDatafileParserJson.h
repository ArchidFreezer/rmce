#pragma once

#include <DatafileParserJson.h>
#include <PoisonTypeData.h>

namespace rm::rule::parser {

	/**
	 * @class PoisonTypeDatafileParserJson
	 * @brief Process PoisonTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating PoisonTypeData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see PoisonTypeData
	 */
	class PoisonTypeDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		PoisonTypeDatafileParserJson(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "PoisonType", filename) {
			setRootNode("poison-types");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		PoisonTypeDatafileParserJson(rm::GameRuleDataFactory& object_manager) : PoisonTypeDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write poisontype game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<PoisonTypeData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a poisontype
		 * @param id Id of the poisontype to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into PoisonTypeData objects
		 *
		 * Parse a boost::ptree containing the poisontype rule datas, convert to PoisonTypeData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser