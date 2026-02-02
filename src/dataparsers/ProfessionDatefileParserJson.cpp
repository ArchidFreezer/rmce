#include <ProfessionDatefileParserJson.h>

void ProfessionDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading Profession data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		if (id_only) {
			// We create a Data object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<ProfessionData> datum = std::make_unique<ProfessionData>(id);
			cache().add<ProfessionData>(std::move(datum), id);
		} else {
			ProfessionData& ref = cache().get<ProfessionData>(id);
			ref.setName(name);
			ref.setDescription(v.second.get<std::string>("description"));

			// Get the book from the cache
			std::string book_id = v.second.get<std::string>("book");
			ref.setBook(cache().get<BookData>(book_id));

			std::cout << "\tProfession name: " << ref.name() << std::endl;
		}
	}

}

void ProfessionDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	ProfessionData& game_data = cache().get<ProfessionData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	datum.put("book", game_data.book().value()->id());

	// Get the container tree for the realms
	pt::ptree realms_tree{};
	for (RealmType::Type realm : game_data.realms()) {
		// Get the realm container
		pt::ptree realm_tree{};
		realm_tree.put("", RealmType::toString(realm));
		realms_tree.push_back(std::make_pair("", realm_tree));
	}
	datum.push_back(std::make_pair("realms", realms_tree));

	// Get the container tree for the stats
	pt::ptree stats_tree{};
	for (StatType::Type stat : game_data.stats()) {
		// Get the stat container
		pt::ptree stat_tree{};
		stat_tree.put("", StatType::toString(stat));
		stats_tree.push_back(std::make_pair("", stat_tree));
	}
	datum.push_back(std::make_pair("stats", stats_tree));

	// Base spell list choices
	pt::ptree base_spell_list_choices_tree{};
	for (const auto& base_spell_list_choice : game_data.baseSpellListChoices()) {
		pt::ptree base_spell_list_choice_tree{};
		populateGameRuleDataChoice<SpellListData>(&base_spell_list_choice, base_spell_list_choice_tree);
		base_spell_list_choices_tree.push_back(std::make_pair("", base_spell_list_choice_tree));
	}
	if (base_spell_list_choices_tree.size()) datum.push_back(std::make_pair("base-spell-list-choices", base_spell_list_choices_tree));

	// Occupational skills
	pt::ptree occupational_skills_tree{};
	for (auto& skill : game_data.occupationalSkills()) {
		pt::ptree skill_tree{};
		skill_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_tree.put("subcategory", skill.subcategory().value());
		occupational_skills_tree.push_back(std::make_pair("", skill_tree));
	}
	if (occupational_skills_tree.size()) datum.push_back(std::make_pair("occupational-skills", occupational_skills_tree));

	// Everyman skills
	pt::ptree everyman_skills_tree{};
	for (auto& skill : game_data.everymanSkills()) {
		pt::ptree skill_tree{};
		skill_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_tree.put("subcategory", skill.subcategory().value());
		everyman_skills_tree.push_back(std::make_pair("", skill_tree));
	}
	if (everyman_skills_tree.size()) datum.push_back(std::make_pair("everyman-skills", everyman_skills_tree));

	// Standard skills
	pt::ptree standard_skills_tree{};
	for (auto& skill : game_data.standardSkills()) {
		pt::ptree skill_tree{};
		skill_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_tree.put("subcategory", skill.subcategory().value());
		standard_skills_tree.push_back(std::make_pair("", skill_tree));
	}
	if (standard_skills_tree.size()) datum.push_back(std::make_pair("standard-skills", standard_skills_tree));

	// Restricted skills
	pt::ptree restricted_skills_tree{};
	for (auto& skill : game_data.restrictedSkills()) {
		pt::ptree skill_tree{};
		skill_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_tree.put("subcategory", skill.subcategory().value());
		restricted_skills_tree.push_back(std::make_pair("", skill_tree));
	}
	if (restricted_skills_tree.size()) datum.push_back(std::make_pair("restricted-skills", restricted_skills_tree));

	// Skill group bonus
	pt::ptree skill_group_bonuses_tree{};
	for (auto& skill_group : game_data.skillGroupsWithBonus()) {
		pt::ptree skill_group_bonus_tree{};
		skill_group_bonus_tree.put("skill_group", skill_group);
		skill_group_bonus_tree.put("bonus", game_data.skillGroupBonus(skill_group));
		skill_group_bonuses_tree.push_back(std::make_pair("", skill_group_bonus_tree));
	}
	if (skill_group_bonuses_tree.size()) datum.push_back(std::make_pair("skill_group-bonuses", skill_group_bonuses_tree));

	// Skill category bonus
	pt::ptree skill_category_bonuses_tree{};
	for (auto& skill_category : game_data.skillCategoriesWithBonus()) {
		pt::ptree skill_category_bonus_tree{};
		skill_category_bonus_tree.put("skill_category", skill_category->id());
		skill_category_bonus_tree.put("bonus", game_data.skillCategoryBonus(*skill_category));
		skill_category_bonuses_tree.push_back(std::make_pair("", skill_category_bonus_tree));
	}
	if (skill_category_bonuses_tree.size()) datum.push_back(std::make_pair("skill_category-bonuses", skill_category_bonuses_tree));

}

