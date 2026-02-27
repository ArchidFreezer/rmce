#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <SkillGroupData.h>

namespace rm {

	/**
	 * @class SkillGroupDatafileParserJson
	 * @brief Process SkillGroupData objects for json files
	 *
	 * This class can read a well-formed json file creating SkillGroupData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see SkillGroupData
	 * @see GameRuleDataCache
	 */

	class SkillGroupDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for SkillGroupData objects
		 * @param filename Path to the datafile to parse
		 */
		SkillGroupDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "SkillGroup", filename) {
			setRootNode("skill-groups");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for SkillGroupData objects
		 */
		SkillGroupDatafileParserJson(GameRuleDataCache& cache) : SkillGroupDatafileParserJson(cache, "") {}

		/**
		 * @brief Write skill group game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<SkillGroupData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a skill group
		 * @param id Id of the skill group to populate from
		 * @param datum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& datum) override;

	private:
		/**
		 * @brief Parse a ptree into SkillGroupData objects
		 *
		 * Parse a boost::ptree containing the skill group rule datas, convert to SkillGroupData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm