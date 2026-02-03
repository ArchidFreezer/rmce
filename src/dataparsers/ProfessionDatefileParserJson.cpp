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

	// Skill bonuses
	pt::ptree skill_bonuses_tree{};
	for (auto& skill : game_data.skillsWithBonus()) {
		pt::ptree skill_bonus_tree{};
		skill_bonus_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_bonus_tree.put("subcategory", skill.subcategory().value());
		skill_bonus_tree.put("bonus", game_data.skillBonus(skill));
		skill_bonuses_tree.push_back(std::make_pair("", skill_bonus_tree));
	}
	if (skill_bonuses_tree.size()) datum.push_back(std::make_pair("skill-bonus", skill_bonuses_tree));

	// Skill development types
	pt::ptree skill_development_types_tree{};
	for (auto& skill : game_data.skillsWithSkillDevelopmentType()) {
		pt::ptree skill_development_type_tree{};
		skill_development_type_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_development_type_tree.put("subcategory", skill.subcategory().value());
		skill_development_type_tree.put("development-type", SkillDevelopmentType::toString(game_data.skillDevelopmentType(skill)));
		skill_development_types_tree.push_back(std::make_pair("", skill_development_type_tree));
	}
	if (skill_development_types_tree.size()) datum.push_back(std::make_pair("skill-development-types", skill_development_types_tree));

	// Skill category bonus
	{
		pt::ptree skill_category_bonuses_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithBonus()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_bonus_tree{};
			skill_category_bonus_tree.put("skill_category", skill_category.first);
			skill_category_bonus_tree.put("bonus", game_data.skillCategoryBonus(*categories.at(skill_category.first)));
			skill_category_bonuses_tree.push_back(std::make_pair("", skill_category_bonus_tree));
		}
		if (skill_category_bonuses_tree.size()) datum.push_back(std::make_pair("skill-category-bonus", skill_category_bonuses_tree));
	}

	// Skill category development types
	pt::ptree skill_category_skill_development_types_tree{};
	for (auto& skill_category : game_data.skillCategoriesWithSkillDevelopmentType()) {
		pt::ptree skill_category_skill_development_type_tree{};
		skill_category_skill_development_type_tree.put("category", skill_category->id());
		skill_category_skill_development_type_tree.put("development-type", SkillDevelopmentType::toString(game_data.skillCategorySkillDevelopmentType(*skill_category)));
		skill_category_skill_development_types_tree.push_back(std::make_pair("", skill_category_skill_development_type_tree));
	}
	if (skill_category_skill_development_types_tree.size()) datum.push_back(std::make_pair("skill-category-skill-development-types", skill_category_skill_development_types_tree));

	// Skill category development type choices
	pt::ptree skill_category_skill_development_type_choices_tree{};
	for (auto& skill_category_choice : game_data.skillCategorySkillDevelopmentTypeChoices()) {
		pt::ptree skill_category_skill_development_type_choice_tree{};
		skill_category_skill_development_type_choice_tree.put("num-choices", skill_category_choice.first.numChoices());
		skill_category_skill_development_type_choice_tree.put("skill-type", SkillDevelopmentType::toString(skill_category_choice.second));

		for (auto& category : skill_category_choice.first.options()) {
			pt::ptree category_tree{};
			category_tree.put("", category->id());
			skill_category_skill_development_type_choice_tree.push_back(std::make_pair("categories", category_tree));
		}
		skill_category_skill_development_type_choices_tree.push_back(std::make_pair("", skill_category_skill_development_type_choice_tree));
	}
	if (skill_category_skill_development_type_choices_tree.size()) { datum.push_back(std::make_pair("skill-category-skill-development-type-choices", skill_category_skill_development_type_choices_tree)); }

	// Skill group bonus
	{
		pt::ptree skill_group_bonuses_tree{};
		std::map<std::string, const SkillGroupData*> groups{};
		for (auto& skill_group : game_data.skillGroupsWithBonus()) {
			groups.emplace(skill_group->id(), skill_group);
		}
		for (const auto& skill_group : groups) {
			pt::ptree skill_group_bonus_tree{};
			skill_group_bonus_tree.put("skill_group", skill_group.first);
			skill_group_bonus_tree.put("bonus", game_data.skillGroupBonus(*skill_group.second));
			skill_group_bonuses_tree.push_back(std::make_pair("", skill_group_bonus_tree));
		}
		if (skill_group_bonuses_tree.size()) datum.push_back(std::make_pair("skill-group-bonuses", skill_group_bonuses_tree));
	}

	// Skill group skill development types
	{
		pt::ptree skill_group_skill_developments_tree{};
		std::map<std::string, const SkillGroupData*> groups{};
		for (auto& skill_group : game_data.skillGroupsWithSkillDevelopmentType()) {
			groups.emplace(skill_group->id(), skill_group);
		}
		for (const auto& skill_group : groups) {
			pt::ptree skill_group_skill_development_tree{};
			skill_group_skill_development_tree.put("skill_group", skill_group.first);
			skill_group_skill_development_tree.put("skill_development", SkillDevelopmentType::toString(game_data.skillGroupSkillDevelopmentType(*skill_group.second)));
			skill_group_skill_developments_tree.push_back(std::make_pair("", skill_group_skill_development_tree));
		}
		if (skill_group_skill_developments_tree.size()) datum.push_back(std::make_pair("skill-group-skill-development-types", skill_group_skill_developments_tree));
	}

	// Skill group development type choices
	pt::ptree skill_group_skill_development_type_choices_tree{};
	for (auto& skill_group_choice : game_data.skillGroupSkillDevelopmentTypeChoices()) {
		pt::ptree skill_group_skill_development_type_choice_tree{};
		skill_group_skill_development_type_choice_tree.put("num-choices", skill_group_choice.first.numChoices());
		skill_group_skill_development_type_choice_tree.put("skill-type", SkillDevelopmentType::toString(skill_group_choice.second));

		for (auto& group : skill_group_choice.first.options()) {
			pt::ptree group_tree{};
			group_tree.put("", group->id());
			skill_group_skill_development_type_choice_tree.push_back(std::make_pair("groups", group_tree));
		}
		skill_group_skill_development_type_choices_tree.push_back(std::make_pair("", skill_group_skill_development_type_choice_tree));
	}
	if (skill_group_skill_development_type_choices_tree.size()) { datum.push_back(std::make_pair("skill-group-skill-development-type-choices", skill_group_skill_development_type_choices_tree)); }

}

