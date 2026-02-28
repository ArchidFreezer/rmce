#pragma once

#include <DatafileParserJson.h>
#include <LanguageCategoryData.h>

namespace rm::rule::parser {

	/**
	 * @class LanguageCategoryDatafileParserJson
	 * @brief Process LanguageCategoryData objects for json files
	 *
	 * This class can read a well-formed json file creating LanguageCategoryData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see LanguageCategoryData
	 */
	class LanguageCategoryDatafileParserJson : public DatafileParserJson {
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

	public:
		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		LanguageCategoryDatafileParserJson(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "LanguageCategory", filename) {
			setRootNode("languageCategories");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		LanguageCategoryDatafileParserJson(rm::GameRuleDataFactory& object_manager) : LanguageCategoryDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write language category game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<LanguageCategoryData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a language category
		 * @param id Id of the language to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into LanguageCategoryData objects
		 *
		 * Parse a boost::ptree containing the language category rule datas, convert to LanguageCategoryData objects and store in
		 * a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser