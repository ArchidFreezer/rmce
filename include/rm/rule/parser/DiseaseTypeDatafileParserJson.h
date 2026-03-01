#pragma once

#include <DatafileParserJson.h>
#include <DiseaseTypeData.h>

namespace rm::rule::parser {

	/**
	 * @class DiseaseTypeDatafileParserJson
	 * @brief Process DiseaseTypeData objects for json files
	 *
	 * This class can read a well-formed json file creating DiseaseTypeData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see DiseaseTypeData
	 */
	class DiseaseTypeDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		DiseaseTypeDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "DiseaseType", filename) {
			setRootNode("disease-types");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		DiseaseTypeDatafileParserJson(rm::PersistentObjectManager& object_manager) : DiseaseTypeDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write diseasetype game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<DiseaseTypeData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a diseasetype
		 * @param id Id of the diseasetype to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into DiseaseTypeData objects
		 *
		 * Parse a boost::ptree containing the diseasetype rule datas, convert to DiseaseTypeData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser