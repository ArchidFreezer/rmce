#include <ProfessionDatefileParserJson.h>


void ProfessionDatafileParserJson::parse() {
	std::cout << "Loading Profession data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		ProfessionData& ref = factory().get<ProfessionData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));

		// Get the book from the cache
		std::string book_id = v.second.get<std::string>("book");
		ref.setBook(factory().get<BookData>(book_id));

		std::cout << "\tProfession name: " << ref.name() << std::endl;
	}

}

void ProfessionDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	ProfessionData& game_data = factory().get<ProfessionData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	datum.put("book", game_data.book().value()->id());
	datum.put("spell-user-type", SpellUserType::toString(game_data.spellUserType()));

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

	// Skill development type choices
	pt::ptree skill_development_type_choices_tree{};
	for (auto& skill_choice : game_data.skillDevelopmentTypeChoices()) {
		pt::ptree skill_development_type_choice_tree{};
		skill_development_type_choice_tree.put("num-choices", skill_choice.first.numChoices());
		skill_development_type_choice_tree.put("skill-type", SkillDevelopmentType::toString(skill_choice.second));

		pt::ptree skills_tree{};
		for (auto& skill : skill_choice.first.options()) {
			pt::ptree skill_tree{};
			skill_tree.put("skill", skill->skillData().id());
			if (skill->subcategory()) skill_tree.put("subcategory", skill->subcategory().value());
			skills_tree.push_back(std::make_pair("", skill_tree));
		}
		skill_development_type_choice_tree.push_back(std::make_pair("skills", skills_tree));

		skill_development_type_choices_tree.push_back(std::make_pair("", skill_development_type_choice_tree));
	}
	if (skill_development_type_choices_tree.size()) { datum.push_back(std::make_pair("skill-development-type-choices", skill_development_type_choices_tree)); }

	// Skill subcategory development type choices
	{
		pt::ptree skill_subcategory_development_type_choices_tree{};
		for (auto& skill_choice : game_data.skillSubcategoryDevelopmentTypeChoices()) {
			pt::ptree skill_subcategory_development_type_choice_tree{};
			skill_subcategory_development_type_choice_tree.put("num-choices", skill_choice.first.numChoices());
			skill_subcategory_development_type_choice_tree.put("skill-type", SkillDevelopmentType::toString(skill_choice.second));

			for (auto& skill : skill_choice.first.options()) {
				pt::ptree skill_tree{};
				skill_tree.put("skill", skill->id());
				skill_subcategory_development_type_choice_tree.push_back(std::make_pair("skills", skill_tree));
			}

			skill_subcategory_development_type_choices_tree.push_back(std::make_pair("", skill_subcategory_development_type_choice_tree));
		}
		if (skill_subcategory_development_type_choices_tree.size()) { datum.push_back(std::make_pair("skill-subcategory-development-type-choices", skill_subcategory_development_type_choices_tree)); }
	}

	// Skill category profession bonus
	{
		pt::ptree skill_category_bonuses_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithProfessionBonus()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_bonus_tree{};
			skill_category_bonus_tree.put("skill_category", skill_category.first);
			skill_category_bonus_tree.put("bonus", game_data.skillCategoryProfessionBonus(*skill_category.second));
			skill_category_bonuses_tree.push_back(std::make_pair("", skill_category_bonus_tree));
		}
		if (skill_category_bonuses_tree.size()) datum.push_back(std::make_pair("skill-category-profession-bonus", skill_category_bonuses_tree));
	}

	// Skill category special bonus
	{
		pt::ptree skill_category_bonuses_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithSpecialBonus()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_bonus_tree{};
			skill_category_bonus_tree.put("skill_category", skill_category.first);
			skill_category_bonus_tree.put("bonus", game_data.skillCategorySpecialBonus(*skill_category.second));
			skill_category_bonuses_tree.push_back(std::make_pair("", skill_category_bonus_tree));
		}
		if (skill_category_bonuses_tree.size()) datum.push_back(std::make_pair("skill-category-special-bonuses", skill_category_bonuses_tree));
	}

	// Skill category development types
	{
		pt::ptree skill_category_skill_development_types_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithSkillDevelopmentType()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_skill_development_type_tree{};
			skill_category_skill_development_type_tree.put("category", skill_category.first);
			skill_category_skill_development_type_tree.put("development-type", SkillDevelopmentType::toString(game_data.skillCategorySkillDevelopmentType(*skill_category.second)));
			skill_category_skill_development_types_tree.push_back(std::make_pair("", skill_category_skill_development_type_tree));
		}
		if (skill_category_skill_development_types_tree.size()) datum.push_back(std::make_pair("skill-category-skill-development-types", skill_category_skill_development_types_tree));
	}

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

	// Skill category costs
	{
		pt::ptree skill_category_costs_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithCost()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_cost_tree{};
			skill_category_cost_tree.put("category", skill_category.first);
			skill_category_cost_tree.put("cost", game_data.skillCategoryDevelopmentCost(*skill_category.second).toString());
			skill_category_costs_tree.push_back(std::make_pair("", skill_category_cost_tree));
		}
		if (skill_category_costs_tree.size()) datum.push_back(std::make_pair("skill-category-costs", skill_category_costs_tree));
	}

	// Skill group profession bonus
	{
		pt::ptree skill_group_bonuses_tree{};
		std::map<std::string, const SkillGroupData*> groups{};
		for (auto& skill_group : game_data.skillGroupsWithProfessionBonus()) {
			groups.emplace(skill_group->id(), skill_group);
		}
		for (const auto& skill_group : groups) {
			pt::ptree skill_group_bonus_tree{};
			skill_group_bonus_tree.put("skill_group", skill_group.first);
			skill_group_bonus_tree.put("bonus", game_data.skillGroupProfessionBonus(*skill_group.second));
			skill_group_bonuses_tree.push_back(std::make_pair("", skill_group_bonus_tree));
		}
		if (skill_group_bonuses_tree.size()) datum.push_back(std::make_pair("skill-group-profession-bonuses", skill_group_bonuses_tree));
	}

	// Skill group special bonus
	{
		pt::ptree skill_group_bonuses_tree{};
		std::map<std::string, const SkillGroupData*> groups{};
		for (auto& skill_group : game_data.skillGroupsWithSpecialBonus()) {
			groups.emplace(skill_group->id(), skill_group);
		}
		for (const auto& skill_group : groups) {
			pt::ptree skill_group_bonus_tree{};
			skill_group_bonus_tree.put("skill_group", skill_group.first);
			skill_group_bonus_tree.put("bonus", game_data.skillGroupSpecialBonus(*skill_group.second));
			skill_group_bonuses_tree.push_back(std::make_pair("", skill_group_bonus_tree));
		}
		if (skill_group_bonuses_tree.size()) datum.push_back(std::make_pair("skill-group-special-bonuses", skill_group_bonuses_tree));
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

