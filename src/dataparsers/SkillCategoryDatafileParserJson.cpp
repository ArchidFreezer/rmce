#include <SkillCategoryDatafileParserJson.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>

void SkillCategoryDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SkillCategoryData& game_data = cache().get<SkillCategoryData>(id);

	datum.put("id", game_data.id());
	datum.put("group", game_data.group().id());
	datum.put("name", game_data.name());
	datum.put("use-realm-stats", game_data.useRealmStats());
	datum.put("skill-progression", game_data.defaultSkillProgression().name());
	datum.put("category-progression", game_data.skillCategoryProgression().name());

	pt::ptree pstats;
	for (auto& stat : game_data.stats()) {
		pt::ptree pstat;
		pstat.put("", toString(stat));
		pstats.push_back(std::make_pair("", pstat));
	}
	datum.push_back(std::make_pair("stats", pstats));
}

void SkillCategoryDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading SkillCategory data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string category_name = v.second.get<std::string>("name");
		std::string group_name{ v.second.get<std::string>("group") };
		// Some skill groups only have s single category and for thoise we dont need the concatenation for the id
		std::string id_name{ (category_name == group_name ? category_name : group_name + "_" + category_name) };
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), id_name));

		if (id_only) {
			// We create a LanguageData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<SkillCategoryData> datum = std::make_unique<SkillCategoryData>(id);
			cache().add<SkillCategoryData>(std::move(datum), id);
		} else {
			SkillCategoryData& ref = cache().get<SkillCategoryData>(id);
			ref.setName(category_name);

			// Attempt to find the SkillGroup
			std::string group_id = v.second.get<std::string>("group");
			ref.setGroup(cache().get<SkillGroupData>(group_id));

			// Get the skill and category progressions
			std::string skill_progression_json = v.second.get<std::string>("skill-progression");
			std::string skill_progression_id = GameRuleData::generateId("SkillProgressionType", skill_progression_json);
			SkillProgressionTypeData& skill_progression{ cache().get<SkillProgressionTypeData>(skill_progression_id) };
			std::string category_progression_json = v.second.get<std::string>("category-progression");
			std::string category_progression_id = GameRuleData::generateId("SkillProgressionType", category_progression_json);
			SkillProgressionTypeData& category_progression{ cache().get<SkillProgressionTypeData>(category_progression_id) };
			ref.setSkillProgressions(skill_progression, category_progression);

			bool use_realm_stats{ v.second.get<bool>("use-realm-stats") };
			ref.setUseRealmStats(use_realm_stats);

			// Check for any stats
			if (!use_realm_stats) {
				if (boost::optional<const pt::ptree&> stat_tree = v.second.get_child_optional("stats")) {
					for (const auto& stat : stat_tree.get()) {
						if (StatType::fromString(stat.second.data())) {
							ref.addStat(StatType::fromString(stat.second.data()).value());
						}
					}
				}
			}

			std::cout << "\tSkillCategory name: " << ref.name() << std::endl;

		}
	}
}