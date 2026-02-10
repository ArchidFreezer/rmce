#include <SkillCategoryDatafileParserJson.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>

void SkillCategoryDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SkillCategoryData& game_data = factory().get<SkillCategoryData>(id);

	datum.put("id", game_data.id());
	datum.put("group", game_data.group().id());
	datum.put("name", game_data.name());
	datum.put("use-realm-stats", game_data.useRealmStats());
	datum.put("skill-progression", game_data.defaultSkillProgression().id());
	datum.put("category-progression", game_data.skillCategoryProgression().id());

	// Stats
	{
		pt::ptree tree{ getEnumVectorTree<StatType::Type>(game_data.stats()) };
		if (!tree.empty()) { datum.push_back(std::make_pair("stats", tree));	}
	}
}

void SkillCategoryDatafileParserJson::parse() {
	std::cout << "Loading SkillCategory data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string category_name = v.second.get<std::string>("name");
		std::string group_name{ v.second.get<std::string>("group") };
		// Some skill groups only have s single category and for thoise we dont need the concatenation for the id
		std::string id_name{ (category_name == group_name ? category_name : group_name + "_" + category_name) };
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), id_name));

		SkillCategoryData& ref = factory().get<SkillCategoryData>(id);
		ref.setName(category_name);

		// Attempt to find the SkillGroup
		std::string group_id = v.second.get<std::string>("group");
		ref.setGroup(factory().get<SkillGroupData>(group_id));

		// Get the skill and category progressions
		std::string skill_progression_id = v.second.get<std::string>("skill-progression");
		SkillProgressionTypeData& skill_progression{ factory().get<SkillProgressionTypeData>(skill_progression_id) };
		std::string category_progression_id = v.second.get<std::string>("category-progression");
		SkillProgressionTypeData& category_progression{ factory().get<SkillProgressionTypeData>(category_progression_id) };
		ref.setSkillProgressions(skill_progression, category_progression);

		ref.setUseRealmStats(v.second.get<bool>("use-realm-stats"));

		// Check for any stats
		ref.setStats(parseEnumVectorTree<StatType::Type>(v.second.get_child_optional("stats")));

		std::cout << "\tSkillCategory name: " << ref.name() << std::endl;

	}
}