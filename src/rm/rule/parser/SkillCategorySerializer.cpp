#include <GameRuleBoostConverter.h>
#include <SkillCategorySerializer.h>

namespace rm::rule::parser {

void SkillCategorySerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading SkillCategory data ..." << std::endl;

	// Get the skillcategorys to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string category_name = v.second.get<std::string>("name");
		std::string group_name{v.second.get<std::string>("group")};
		// Some skill groups only have s single category and for thoise we dont need the concatenation for the id
		std::string id_name{(category_name == group_name ? category_name : group_name + "_" + category_name)};
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), id_name));

		SkillCategoryData& ref = manager().get<SkillCategoryData>(id);
		ref.setName(category_name);

		// Attempt to find the SkillGroup
		std::string group_id = v.second.get<std::string>("group");
		ref.setGroup(manager().get<SkillGroupData>(group_id));

		// Get the skill and category progressions
		std::string skill_progression_id = v.second.get<std::string>("skill-progression");
		SkillProgressionTypeData& skill_progression{manager().get<SkillProgressionTypeData>(skill_progression_id)};
		std::string category_progression_id = v.second.get<std::string>("category-progression");
		SkillProgressionTypeData& category_progression{manager().get<SkillProgressionTypeData>(category_progression_id)};
		ref.setSkillProgressions(skill_progression, category_progression);

		ref.setUseRealmStats(v.second.get<bool>("use-realm-stats"));

		GameRuleBoostConverter converter{manager()};
		// Check for any stats
		ref.setStats(converter.parseEnumVectorTree<StatType::Type>(v.second.get_child_optional("stats")));

		std::cout << "\tSkillCategory name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void SkillCategorySerializer::populateDatum(std::string key, pt::ptree& datum) {
	SkillCategoryData& game_data = manager().get<SkillCategoryData>(key);
	datum.put("id", game_data.id());
	datum.put("group", game_data.group().id());
	datum.put("name", game_data.name());
	datum.put("use-realm-stats", game_data.useRealmStats());
	datum.put("skill-progression", game_data.defaultSkillProgression().id());
	datum.put("category-progression", game_data.skillCategoryProgression().id());

	GameRuleBoostConverter converter{manager()};

	// Stats
	{
		pt::ptree tree{converter.getEnumVectorTree<StatType::Type>(game_data.stats())};
		if (!tree.empty()) {
			datum.push_back(std::make_pair("stats", tree));
		}
	}
}

std::string SkillCategorySerializer::rootNode() const {
	return "skill-categories";
}

} // namespace rm::rule::parser