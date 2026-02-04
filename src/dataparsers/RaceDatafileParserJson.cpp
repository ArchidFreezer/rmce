#include <RaceDatafileParserJson.h>

void RaceDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading Race data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		RaceData& ref = factory().get<RaceData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));
		ref.setHighCulture(v.second.get<bool>("high-culture"));
		ref.setRecoveryMultiplier(v.second.get<float>("recovery-multiplier"));
		ref.setNumBackgroundOptions(v.second.get<int>("background-options"));
		ref.setExhaustionBonus(v.second.get<int>("exhaustion-bonus"));
		ref.setStatLossRacialType(v.second.get<int>("stat-loss-racial-type"));
		ref.setRequiredSleep(v.second.get<int>("required-sleep"));
		ref.setRequiredSleepFrequency(v.second.get<int>("required-sleep-frequency"));
		ref.setSoulDeparture(v.second.get<int>("soul-departure"));
		ref.setBuildModifier(v.second.get<int>("build-modifier"));
		ref.setAverageMaleHeight(v.second.get<int>("average-male-height"));
		ref.setAverageFemaleHeight(v.second.get<int>("average-female-height"));
		ref.setAverageLifespan(v.second.get<int>("average-lifespan"));
		ref.setMaleWeightModifier(v.second.get<int>("male-weight-modifier"));
		ref.setFemaleWeightModifier(v.second.get<int>("female-weight-modifier"));

		// Get the book from the cache
		std::string book_id = v.second.get<std::string>("book");
		ref.setBook(factory().get<BookData>(book_id));

		// Get the creature size
		std::string creature_size = v.second.get<std::string>("creature-size");
		if (CreatureSizeType::fromString(creature_size)) {
			ref.setSize(CreatureSizeType::fromString(creature_size).value());
		}

		// get the critical type
		std::string critical_table = v.second.get<std::string>("critical-table");
		if (CriticalTableType::fromString(critical_table)) {
			ref.setCriticalTableType(CriticalTableType::fromString(critical_table).value());
		}

		// Get the realm progressions
		std::string arcane_progression = v.second.get<std::string>("arcane-progression");
		ref.setArcaneProgression(factory().get<SkillProgressionTypeData>(arcane_progression));
		std::string arms_progression = v.second.get<std::string>("arms-progression");
		ref.setArmsProgression(factory().get<SkillProgressionTypeData>(arms_progression));
		std::string channeling_progression = v.second.get<std::string>("channeling-progression");
		ref.setChannelingProgression(factory().get<SkillProgressionTypeData>(channeling_progression));
		std::string essence_progression = v.second.get<std::string>("essence-progression");
		ref.setEssenceProgression(factory().get<SkillProgressionTypeData>(essence_progression));
		std::string mentalism_progression = v.second.get<std::string>("mentalism-progression");
		ref.setMentalismProgression(factory().get<SkillProgressionTypeData>(mentalism_progression));

		// Get the starting languages
		for (const auto& language : v.second.get_child("starting-languages")) {
			std::string language_name{ language.second.get<std::string>("language") };
			LanguageAbility ability(factory().get<LanguageData>(language_name));
			boost::optional<int> somantic = language.second.get_optional<int>("somantic");
			if (somantic) {	ability.updateSomanticRanks(somantic.get()); }
			boost::optional<int> spoken = language.second.get_optional<int>("spoken");
			if (spoken) { ability.updateSpokenRanks(spoken.get()); }				
			boost::optional<int> written = language.second.get_optional<int>("written");
			if (written) { ability.updateWrittenRanks(written.get()); }				
			ref.setStartingLanguageAbility(ability);
		}

		// Get the adolescent languages
		for (const auto& language : v.second.get_child("adolescent-languages")) {
			std::string language_name{ language.second.get<std::string>("language") };
			LanguageAbility ability(factory().get<LanguageData>(language_name));
			boost::optional<int> somantic = language.second.get_optional<int>("somantic");
			if (somantic) { ability.updateSomanticRanks(somantic.get()); }
			boost::optional<int> spoken = language.second.get_optional<int>("spoken");
			if (spoken) { ability.updateSpokenRanks(spoken.get()); }
			boost::optional<int> written = language.second.get_optional<int>("written");
			if (written) { ability.updateWrittenRanks(written.get()); }
			ref.setAdolescentLanguageAbility(ability);
		}

		// Get Stat bonuses
		if (boost::optional<const pt::ptree&> stat_bonuses = v.second.get_child_optional("stat-bonuses")) {
			for (const auto& stat_bonus : stat_bonuses.get()) {
				StatType::Type stat{ StatType::fromString(stat_bonus.second.get<std::string>("stat")).value() };
				int bonus{ stat_bonus.second.get<int>("bonus") };
				if (bonus) ref.setStatBonus(stat, bonus);
			}
		}

		// Get everyman skills
		if (boost::optional<const pt::ptree&> everyman_skills = v.second.get_child_optional("everyman-skills")) {
			for (const auto& everyman_skill : everyman_skills.get()) {
				std::string skill_name{ everyman_skill.second.get<std::string>("skill")};
				boost::optional<std::string> subcategory = everyman_skill.second.get_optional<std::string>("subcategory");

				if (subcategory) {
					ref.addEverymanSkill(factory().subcategoriedSkillData(skill_name, subcategory.get()));
				} else {
					ref.addEverymanSkill(factory().subcategoriedSkillData(skill_name));
				}
			}
		}

		// Get restricted skills
		if (boost::optional<const pt::ptree&> restricted_skills = v.second.get_child_optional("restricted-skills")) {
			for (const auto& restricted_skill : restricted_skills.get()) {
				std::string skill_name{ restricted_skill.second.get<std::string>("skill") };
				boost::optional<std::string> subcategory = restricted_skill.second.get_optional<std::string>("subcategory");

				if (subcategory) {
					ref.addRestrictedSkill(factory().subcategoriedSkillData(skill_name, subcategory.get()));
				} else {
					ref.addRestrictedSkill(factory().subcategoriedSkillData(skill_name));
				}
			}
		}

		// Everyman skill categories
		if (boost::optional<const pt::ptree&> everyman_categories = v.second.get_child_optional("everyman-categories")) {
			for (const auto& everyman_category : everyman_categories.get()) {
				std::string category_name{ everyman_category.second.get_value<std::string>()};
				ref.addEverymanSkillCategory(factory().get<SkillCategoryData>(category_name));
			}
		}

		// Restricted skill categories
		if (boost::optional<const pt::ptree&> restricted_categories = v.second.get_child_optional("restricted-categories")) {
			for (const auto& restricted_category : restricted_categories.get()) {
				std::string category_name{ restricted_category.second.get_value<std::string>() };
				ref.addRestrictedSkillCategory(factory().get<SkillCategoryData>(category_name));
			}
		}

		// Get skill bonuses
		if (boost::optional<const pt::ptree&> skill_bonuses = v.second.get_child_optional("skill-bonuses")) {
			for (const auto& skill_bonus_tree : skill_bonuses.get()) {
				std::string skill_name{ skill_bonus_tree.second.get<std::string>("skill") };
				boost::optional<std::string> subcategory = skill_bonus_tree.second.get_optional<std::string>("subcategory");
				int bonus{ skill_bonus_tree.second.get<int>("bonus") };
				if (subcategory) {
					ref.setSkillBonus(factory().subcategoriedSkillData(skill_name, subcategory.get()), bonus);
				} else {
					ref.setSkillBonus(factory().subcategoriedSkillData(skill_name), bonus);
				}
			}
		}

		// Get the skill category everyman choices
		if (boost::optional<const pt::ptree&> category_choices = v.second.get_child_optional("skill-category-choices-everyman")) {
			for (const auto& choice_tree : category_choices.get()) {

				GameRuleDataChoice<SkillCategoryData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));

				for (const auto& category_tree : choice_tree.second.get_child("options")) {
					std::string category_id{ category_tree.second.get_value<std::string>() };
					choice_data.addOption(factory().get<SkillCategoryData>(category_id));
				}
				ref.addCategoryEverymanSkillChoice(std::move(choice_data));
			}
		}

		std::cout << "\tRace name: " << ref.name() << std::endl;
	}

}

void RaceDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	RaceData& game_data = factory().get<RaceData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	datum.put("book", game_data.book().value()->id());
	datum.put("high-culture", game_data.highCulture());
	datum.put("creature-size", CreatureSizeType::toString(game_data.size()));
	datum.put("critical-table", CriticalTableType::toString(game_data.criticalTableType()));
	datum.put("recovery-multiplier", game_data.recoveryMultipler());
	datum.put("background-options", game_data.numBackgroundOption());
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
	datum.put("female-weight-modifier", game_data.femaleWeightModifier());
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
	// The categories are stored as a pointer so are not alpha sorted by name so we do that first to ensure consistent data file order
	std::set<std::string> everyman_category_set{};
	for (auto& category : game_data.everymanSkillCategories()) {
		everyman_category_set.insert(category->id());
	}
	for (auto& category : everyman_category_set) {
		pt::ptree everyman_category_tree{};
		everyman_category_tree.put("", category);
		everyman_categories_tree.push_back(std::make_pair("", everyman_category_tree));
	}
	if (everyman_categories_tree.size()) datum.push_back(std::make_pair("everyman-categories", everyman_categories_tree));

	// Restricted categories
	pt::ptree restricted_categories_tree{};
	// The categories are stored as a pointer so are not alpha sorted by name so we do that first to ensure consistent data file order
	std::set<std::string> restricted_category_set{};
	for (auto& category : game_data.restrictedSkillCategories()) {
		restricted_category_set.insert(category->id());
	}
	for (auto& category : restricted_category_set) {
		pt::ptree restricted_category_tree{};
		restricted_category_tree.put("", category);
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
		populateGameRuleDataChoice<SkillCategoryData>(&category_choices, category_choice_tree);
		everyman_category_choices_tree.push_back(std::make_pair("", category_choice_tree));
	}
	
	if (everyman_category_choices_tree.size()) datum.push_back(std::make_pair("skill-category-choices-everyman", everyman_category_choices_tree));

}

