#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <SkillData.h>

namespace rm::rule::parser {

	/**
	 * @class SkillDatafileParserJson
	 * @brief Process SkillData objects for json files
	 *
	 * This class can read a well-formed json file creating SkillData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see rm::rule::SkillData
	 * @see GameRuleDataCache
	 */

	class SkillDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		//using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for SkillData objects
		 * @param filename Path to the datafile to parse
		 */
		SkillDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Skill", filename) {
			setRootNode("skills");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for SkillData objects
		 */
		SkillDatafileParserJson(GameRuleDataCache& cache) : SkillDatafileParserJson(cache, "") {}

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