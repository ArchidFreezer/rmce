#pragma once

#include <DatafileParserJson.h>
#include <SkillCategoryData.h>

namespace rm::rule::parser {

	/**
	 * @class SkillCategoryDatafileParserJson
	 * @brief Process SkillCategoryData objects for json files
	 *
	 * This class can read a well-formed json file creating SkillCategoryData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see SkillCategoryData
	 */

	class SkillCategoryDatafileParserJson : public DatafileParserJson {
	public:

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		SkillCategoryDatafileParserJson(rm::PersistentObjectManager& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "SkillCategory", filename) {
			setRootNode("skill-categories");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		SkillCategoryDatafileParserJson(rm::PersistentObjectManager& object_manager) : SkillCategoryDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write skill category game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<SkillCategoryData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a skill category
		 * @param id Id of the skill category to populate from
		 * @param datum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& datum) override;

	private:
		/**
		 * @brief Parse a ptree into SkillCategoryData objects
		 *
		 * Parse a boost::ptree containing the skill category rule datas, convert to SkillCategoryData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser