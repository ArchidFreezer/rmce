#pragma once

#include <DatafileParserJson.h>
#include <SkillData.h>

namespace rm::rule::parser {

	/**
	 * @class SkillDatafileParserJson
	 * @brief Process SkillData objects for json files
	 *
	 * This class can read a well-formed json file creating SkillData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see rm::rule::SkillData
	 */

	class SkillDatafileParserJson : public DatafileParserJson {
	public:

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		SkillDatafileParserJson(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "Skill", filename) {
			setRootNode("skills");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		SkillDatafileParserJson(rm::GameRuleDataFactory& object_manager) : SkillDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write skill game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<SkillData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a skill
		 * @param id Id of the skill to populate from
		 * @param datum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& datum) override;

	private:
		/**
		 * @brief Parse a ptree into SkillData objects
		 *
		 * Parse a boost::ptree containing the skill rule datas, convert to SkillData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser