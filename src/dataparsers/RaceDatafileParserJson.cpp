#include <RaceDatafileParserJson.h>

void RaceDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading Race data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		if (id_only) {
			// We create a Data object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<RaceData> datum = std::make_unique<RaceData>(id);
			cache().add<RaceData>(std::move(datum), id);
		} else {
			RaceData& ref = cache().get<RaceData>(id);
			ref.setName(name);

			std::cout << "\tRace name: " << ref.name() << std::endl;
		}
	}

}

void RaceDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	RaceData& game_data = cache().get<RaceData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	datum.put("book", game_data.book().value()->id());
	datum.put("hight-culture", game_data.highCulture());
	datum.put("creature-size", CreatureSizeType::toString(game_data.size()));
	datum.put("critical-table", CriticalTableType::toString(game_data.criticalTableType()));
	datum.put("recovery-multiplier", game_data.recoveryMultipler());
	datum.put("exhaustion-bonus", game_data.exhaustionBonus());
	datum.put("stat-loss-racial-type", game_data.statLossRacialType());
	datum.put("required-sleep", game_data.requiredSleep());
	datum.put("required-sleep-frequency", game_data.requiredSleepFrequency());
	datum.put("soul-departure", game_data.soulDeparture());
	datum.put("build-modifier", game_data.buildModifier());
	datum.put("average-male-height", game_data.averageMaleHeight());
	datum.put("average-female-height", game_data.averageFemaleHeight());
	datum.put("average-lifespan", game_data.averageLifespan());
	datum.put("male-weight-modifier", game_data.maleWeightModifier());
	datum.put("male-feweight-modifier", game_data.femaleWeightModifier());
	datum.put("arcane-progression", game_data.arcaneProgression().id());
	datum.put("arms-progression", game_data.armsProgression().id());
	datum.put("channeling-progression", game_data.channelingProgression().id());
	datum.put("essence-progression", game_data.essenceProgression().id());
	datum.put("mentalism-progression", game_data.mentalismProgression().id());

	// Starting languages
	pt::ptree starting_languages_tree{};
	for (auto& language_ability : game_data.startingLanguages()) {
		pt::ptree language_ability_tree{};
		language_ability_tree.put("language", language_ability.languageId());
		if (language_ability.somantic()) language_ability_tree.put("somantic", language_ability.somantic());
		if (language_ability.spoken()) language_ability_tree.put("spoken", language_ability.spoken());
		if (language_ability.written()) language_ability_tree.put("written", language_ability.written());
		starting_languages_tree.push_back(std::make_pair("", language_ability_tree));
	}
	datum.push_back(std::make_pair("starting-languages", starting_languages_tree));

	// Adolescent languages
	pt::ptree adolescent_languages_tree{};
	for (auto& language_ability : game_data.adolescentLanguages()) {
		pt::ptree language_ability_tree{};
		language_ability_tree.put("language", language_ability.languageId());
		if (language_ability.somantic()) language_ability_tree.put("somantic", language_ability.somantic());
		if (language_ability.spoken()) language_ability_tree.put("spoken", language_ability.spoken());
		if (language_ability.written()) language_ability_tree.put("written", language_ability.written());
		adolescent_languages_tree.push_back(std::make_pair("", language_ability_tree));
	}
	datum.push_back(std::make_pair("adolescent-languages", adolescent_languages_tree));

	// Stat bonus
	pt::ptree stat_bonuses_tree{};
	for (auto& stat_with_bonus : game_data.statsWithBonus()) {
		pt::ptree stat_bonus_tree{};
		StatType::Type stat{ stat_with_bonus };
		stat_bonus_tree.put("stat", StatType::toString(stat));
		stat_bonus_tree.put("bonus", game_data.statBonus(stat));
		stat_bonuses_tree.push_back(std::make_pair("", stat_bonus_tree));
	}
	if (stat_bonuses_tree.size()) datum.push_back(std::make_pair("stat-bonuses", stat_bonuses_tree));

	// Everyman skills
	pt::ptree everyman_skills_tree{};
	for (auto& skill : game_data.everymanSkills()) {
		pt::ptree skill_tree{};
		skill_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_tree.put("subcategory", skill.subcategory().value());
		everyman_skills_tree.push_back(std::make_pair("", skill_tree));
	}
	if (everyman_skills_tree.size()) datum.push_back(std::make_pair("everyman-skills", everyman_skills_tree));

	// Restricted skills
	pt::ptree restricted_skills_tree{};
	for (auto& skill : game_data.restrictedSkills()) {
		pt::ptree skill_tree{};
		skill_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_tree.put("subcategory", skill.subcategory().value());
		restricted_skills_tree.push_back(std::make_pair("", skill_tree));
	}
	if (restricted_skills_tree.size()) datum.push_back(std::make_pair("restricted-skills", restricted_skills_tree));

	// Everyman categories
	pt::ptree everyman_categories_tree{};
	for (auto& category : game_data.everymanSkillCategories()) {
		pt::ptree everyman_category_tree{};
		everyman_category_tree.put("", category->id());
		everyman_categories_tree.push_back(std::make_pair("", everyman_category_tree));
	}
	if (everyman_categories_tree.size()) datum.push_back(std::make_pair("everyman-categories", everyman_categories_tree));

	// Restricted categories
	pt::ptree restricted_categories_tree{};
	for (auto& category : game_data.restrictedSkillCategories()) {
		pt::ptree restricted_category_tree{};
		restricted_category_tree.put("", category->id());
		restricted_categories_tree.push_back(std::make_pair("", restricted_category_tree));
	}
	if (restricted_categories_tree.size()) datum.push_back(std::make_pair("restricted-categories", restricted_categories_tree));

	// Skill bonuses
	pt::ptree skill_bonuses_tree{};
	for (auto& skill : game_data.skillsWithBonus()) {
		pt::ptree skill_bonus_tree{};
		skill_bonus_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_bonus_tree.put("subcategory", skill.subcategory().value());
		skill_bonus_tree.put("bonus", game_data.skillBonus(skill));
		skill_bonuses_tree.push_back(std::make_pair("", skill_bonus_tree));
	}
	if (skill_bonuses_tree.size()) datum.push_back(std::make_pair("skill-bonuses", skill_bonuses_tree));

	// Everyman skill category choices
	pt::ptree everyman_category_choices_tree{};
	for (const auto& category_choices : game_data.categoryEverymanSkillChoices()) {
		pt::ptree category_choice_tree{};
		populateGameRuleDataChoice<SkillCategoryData>(category_choices.get(), category_choice_tree);
		everyman_category_choices_tree.push_back(std::make_pair("", category_choice_tree));
	}
	
	if (everyman_category_choices_tree.size()) datum.push_back(std::make_pair("skill-category-choices-everyman", everyman_category_choices_tree));

}

