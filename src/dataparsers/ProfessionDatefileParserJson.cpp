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
		ref.setSpellUserType(SpellUserType::fromString(v.second.get<std::string>("spell-user-type")).value());

		// Get the book from the cache
		std::string book_id = v.second.get<std::string>("book");
		ref.setBook(factory().get<BookData>(book_id));

		// Get the spell power realms
		for (const auto& realm_tree : v.second.get_child("realms")) {
			std::optional<RealmType::Type> realm = RealmType::fromString(realm_tree.second.get_value<std::string>());
			if (realm) ref.addRealm(realm.value());
		}

		// Get the primary stats
		for (const auto& stat_tree : v.second.get_child("stats")) {
			std::optional<StatType::Type> stat = StatType::fromString(stat_tree.second.get_value<std::string>());
			if (stat) ref.addStat(stat.value());
		}

		// Get the base spell list choices
		if (boost::optional<const pt::ptree&> spell_list_choices = v.second.get_child_optional("base-spell-list-choices")) {
			for (const auto& choice_tree : spell_list_choices.get()) {

				GameRuleDataChoice<SpellListData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));

				for (const auto& list_tree : choice_tree.second.get_child("options")) {
					std::string list_id{ list_tree.second.get_value<std::string>() };
					choice_data.addOption(factory().get<SpellListData>(list_id));
				}
				ref.addBaseSpellListChoice(std::move(choice_data));
			}
		}

		// Get skill bonuses
		ref.setSkillBonuses(parseSkillPairTree<int>(v.second.get_child_optional("skill-bonuses")));

		// Skill development types
		ref.setSkillDevelopmentTypes(parseSkillPairEnumTree<SkillDevelopmentType::Type>(v.second.get_child_optional("skill-development-types")));

		// Skill development type choices
		if (boost::optional<const pt::ptree&> skill_development_type_choices = v.second.get_child_optional("skill-development-type-choices")) {
			for (const auto& choice_tree : skill_development_type_choices.get()) {

				GameRuleDataChoice<SubcategoriedSkillData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
				std::string type_id{ choice_tree.second.get<std::string>("development-type") };

				for (const auto& skill_tree : choice_tree.second.get_child("skills")) {
					std::string skill_id{ skill_tree.second.get<std::string>("skill")};
					boost::optional<std::string> subcategory = skill_tree.second.get_optional<std::string>("subcategory");
					if (subcategory) {
						choice_data.addOption(factory().subcategoriedSkillData(skill_id, subcategory.get()));
					} else {
						choice_data.addOption(factory().subcategoriedSkillData(skill_id));
					}
				}
				ref.addSkillDevelopmentTypeChoice(std::move(choice_data), SkillDevelopmentType::fromString(type_id).value());
			}
		}

		// Skill subcategory development type choices
		if (boost::optional<const pt::ptree&> skill_development_type_choices = v.second.get_child_optional("skill-subcategory-development-type-choices")) {
			for (const auto& choice_tree : skill_development_type_choices.get()) {

				GameRuleDataChoice<SkillData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
				std::string type_id{ choice_tree.second.get<std::string>("development-type") };
				for (const auto& skill_tree : choice_tree.second.get_child("skills")) {
					std::string skill_id{ skill_tree.second.get_value<std::string>() };
					choice_data.addOption(factory().get<SkillData>(skill_id));
				}
				ref.addSkillSubcategoryDevelopmentTypeChoice(std::move(choice_data), SkillDevelopmentType::fromString(type_id).value());
			}
		}

		// Skill category profession bonus
		ref.setSkillCategoryProfessionBonuses(parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-profession-bonuses")));

		// Skill category special bonus
		ref.setSkillCategorySpecialBonuses(parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-special-bonuses")));

		// Skill category development types
		ref.setSkillCategorySkillDevelopmentTypes(parseGameDataPairEnumTree<SkillCategoryData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-category-skill-development-types")));

		// Skill category development type choices
		if (boost::optional<const pt::ptree&> skill_category_development_type_choices = v.second.get_child_optional("skill-category-skill-development-type-choices")) {
			for (const auto& choice_tree : skill_category_development_type_choices.get()) {

				GameRuleDataChoice<SkillCategoryData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
				std::string type_id{ choice_tree.second.get<std::string>("development-type") };
				for (const auto& category_tree : choice_tree.second.get_child("categories")) {
					std::string skill_id{ category_tree.second.get_value<std::string>() };
					choice_data.addOption(factory().get<SkillCategoryData>(skill_id));
				}
				ref.addSkillCategorySkillDevelopmentTypeChoice(std::move(choice_data), SkillDevelopmentType::fromString(type_id).value());
			}
		}

		// Skill category costs
		if (boost::optional<const pt::ptree&> category_costs = v.second.get_child_optional("skill-category-costs")) {
			for (const auto& category_cost : category_costs.get()) {
				std::string category_id{ category_cost.second.get<std::string>("category") };
				std::string cost{ category_cost.second.get<std::string>("cost") };
				ref.addSkillCategoryDevelopmentCost(factory().get<SkillCategoryData>(category_id), std::move(SkillDevelopmentCost(cost)));
			}
		}

		// Skill group profession bonus
		ref.setSkillGroupProfessionBonuses(parseGameDataPairTree<SkillGroupData, int>(v.second.get_child_optional("skill-group-profession-bonuses")));

		// Skill group special bonus
		ref.setSkillGroupSpecialBonuses(parseGameDataPairTree<SkillGroupData, int>(v.second.get_child_optional("skill-group-special-bonuses")));

		// Skill group skill development types
		ref.setSkillGroupSkillDevelopmentTypes(parseGameDataPairEnumTree<SkillGroupData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-group-skill-development-types")));

		// Skill group development type choices
		if (boost::optional<const pt::ptree&> skill_group_development_type_choices = v.second.get_child_optional("skill-group-skill-development-type-choices")) {
			for (const auto& choice_tree : skill_group_development_type_choices.get()) {

				GameRuleDataChoice<SkillGroupData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
				std::string type_id{ choice_tree.second.get<std::string>("development-type") };
				for (const auto& group_tree : choice_tree.second.get_child("groups")) {
					std::string skill_id{ group_tree.second.get_value<std::string>() };
					choice_data.addOption(factory().get<SkillGroupData>(skill_id));
				}
				ref.addSkillGroupSkillDevelopmentTypeChoice(std::move(choice_data), SkillDevelopmentType::fromString(type_id).value());
			}
		}

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
	{
		pt::ptree tree{ getSkillPairTree<int>(game_data.skillBonuses()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-bonuses", tree));
	}

	// Skill development types
	{
		pt::ptree tree{ getSkillPairEnumTree<SkillDevelopmentType::Type>(game_data.skillDevelopmentTypes()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-development-types", tree));
	}

	// Skill development type choices
	pt::ptree skill_development_type_choices_tree{};
	for (auto& skill_choice : game_data.skillDevelopmentTypeChoices()) {
		pt::ptree skill_development_type_choice_tree{};
		skill_development_type_choice_tree.put("num-choices", skill_choice.first.numChoices());
		skill_development_type_choice_tree.put("development-type", SkillDevelopmentType::toString(skill_choice.second));

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
			skill_subcategory_development_type_choice_tree.put("development-type", SkillDevelopmentType::toString(skill_choice.second));

			pt::ptree skills_tree{};
			for (auto& skill : skill_choice.first.options()) {
				pt::ptree skill_tree{};
				skill_tree.put("", skill->id());
				skills_tree.push_back(std::make_pair("", skill_tree));
			}
			skill_subcategory_development_type_choice_tree.push_back(std::make_pair("skills", skills_tree));

			skill_subcategory_development_type_choices_tree.push_back(std::make_pair("", skill_subcategory_development_type_choice_tree));
		}
		if (skill_subcategory_development_type_choices_tree.size()) { datum.push_back(std::make_pair("skill-subcategory-development-type-choices", skill_subcategory_development_type_choices_tree)); }
	}

	// Skill category profession bonus
	{
		pt::ptree tree{ getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategoryProfessionBonuses()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-category-profession-bonuses", tree));
	}

	// Skill category special bonus
	{
		pt::ptree tree{ getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategorySpecialBonuses()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-category-special-bonuses", tree));
	}

	// Skill category development types
	{
		pt::ptree tree{ getGameDataPairEnumTree<SkillCategoryData,SkillDevelopmentType::Type>(game_data.skillCategorySkillDevelopmentTypes()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-category-skill-development-types", tree));
	}

	// Skill category development type choices
	pt::ptree skill_category_skill_development_type_choices_tree{};
	for (auto& skill_category_choice : game_data.skillCategorySkillDevelopmentTypeChoices()) {
		pt::ptree skill_category_skill_development_type_choice_tree{};
		skill_category_skill_development_type_choice_tree.put("num-choices", skill_category_choice.first.numChoices());
		skill_category_skill_development_type_choice_tree.put("development-type", SkillDevelopmentType::toString(skill_category_choice.second));

		pt::ptree categories_tree{};
		for (auto& category : skill_category_choice.first.options()) {
			pt::ptree category_tree{};
			category_tree.put("", category->id());
			categories_tree.push_back(std::make_pair("", category_tree));
		}
		skill_category_skill_development_type_choice_tree.push_back(std::make_pair("categories", categories_tree));
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
		pt::ptree tree{ getGameDataPairTree<SkillGroupData, int>(game_data.skillGroupProfessionBonuses()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-group-profession-bonuses", tree));
	}

	// Skill group special bonus
	{
		pt::ptree tree{ getGameDataPairTree<SkillGroupData, int>(game_data.skillGroupSpecialBonuses()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-group-special-bonuses", tree));
	}

	// Skill group skill development types
	{
		pt::ptree tree{ getGameDataPairEnumTree<SkillGroupData,SkillDevelopmentType::Type>(game_data.skillGroupSkillDevelopmentTypes()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-group-skill-development-types", tree));
	}

	// Skill group development type choices
	pt::ptree skill_group_skill_development_type_choices_tree{};
	for (auto& skill_group_choice : game_data.skillGroupSkillDevelopmentTypeChoices()) {
		pt::ptree skill_group_skill_development_type_choice_tree{};
		skill_group_skill_development_type_choice_tree.put("num-choices", skill_group_choice.first.numChoices());
		skill_group_skill_development_type_choice_tree.put("development-type", SkillDevelopmentType::toString(skill_group_choice.second));

		pt::ptree groups_tree{};
		for (auto& group : skill_group_choice.first.options()) {
			pt::ptree group_tree{};
			group_tree.put("", group->id());
			groups_tree.push_back(std::make_pair("", group_tree));
		}
		skill_group_skill_development_type_choice_tree.push_back(std::make_pair("groups", groups_tree));
		skill_group_skill_development_type_choices_tree.push_back(std::make_pair("", skill_group_skill_development_type_choice_tree));
	}
	if (skill_group_skill_development_type_choices_tree.size()) { datum.push_back(std::make_pair("skill-group-skill-development-type-choices", skill_group_skill_development_type_choices_tree)); }

}

