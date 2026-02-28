#include <RaceDatafileParserJson.h>

using namespace rm::rule::enums;

namespace rm::rule::parser {

	void RaceDatafileParserJson::parse() {
		std::cout << "Loading Race data ... ";

		// Get the lists to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", factory().generateId(ruleDatatype(), name));

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
			CreatureSizeType::Type size{};
			fromString(v.second.get<std::string>("creature-size"), size);
			ref.setSize(size);

			// get the critical type
			CriticalTableType::Type crit_table_type{};
			fromString(v.second.get<std::string>("critical-table"), crit_table_type);
			ref.setCriticalTableType(crit_table_type);

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
			ref.setStartingLanguageAbilities(parseLanguageAbilityMapTree(v.second.get_child_optional("starting-languages")));

			// Get the adolescent languages
			ref.setAdolescentLanguageAbilities(parseLanguageAbilityMapTree(v.second.get_child_optional("adolescent-languages")));

			// Get Stat bonuses
			ref.setStatBonuses(parseEnumPairTree<StatType::Type, int>(v.second.get_child_optional("stat-bonuses")));

			// Get everyman skills
			ref.setEverymanSkills(parseSkillSetTree(v.second.get_child_optional("everyman-skills")));

			// Get restricted skills
			ref.setRestrictedSkills(parseSkillSetTree(v.second.get_child_optional("restricted-skills")));

			// Everyman skill categories
			ref.setEverymanSkillCategories(parseGameDataSetTree<SkillCategoryData>(v.second.get_child_optional("everyman-categories")));

			// Restricted skill categories
			ref.setRestrictedSkillCategories(parseGameDataSetTree<SkillCategoryData>(v.second.get_child_optional("restricted-categories")));

			// Get skill bonuses
			ref.setSkillBonuses(parseSkillPairTree<int>(v.second.get_child_optional("skill-bonuses")));

			// Get the skill category everyman choices
			ref.setCategoryEverymanSkillChoices(parseGameDataChoiceSetTree<SkillCategoryData>(v.second.get_child_optional("skill-category-choices-everyman")));

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
		{
			pt::ptree tree{ getLanguageAbilityMapTree(game_data.startingLanguageAbilities()) };
			if (tree.size()) datum.push_back(std::make_pair("starting-languages", tree));
		}

		// Adolescent languages
		{
			pt::ptree tree{ getLanguageAbilityMapTree(game_data.adolescentLanguageAbilities()) };
			if (tree.size()) datum.push_back(std::make_pair("adolescent-languages", tree));
		}

		// Stat bonus
		{
			pt::ptree tree{ getEnumPairTree<StatType::Type, int>(game_data.statBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("stat-bonuses", tree));
		}

		// Everyman skills
		{
			pt::ptree tree{ getSkillSetTree(game_data.everymanSkills()) };
			if (tree.size()) datum.push_back(std::make_pair("everyman-skills", tree));
		}

		// Restricted skills
		{
			pt::ptree tree{ getSkillSetTree(game_data.restrictedSkills()) };
			if (tree.size()) datum.push_back(std::make_pair("restricted-skills", tree));
		}

		// Everyman categories
		{
			pt::ptree tree{ getGameDataSetTree<SkillCategoryData>(game_data.everymanSkillCategories()) };
			if (tree.size()) datum.push_back(std::make_pair("everyman-categories", tree));
		}

		// Restricted categories
		{
			pt::ptree tree{ getGameDataSetTree<SkillCategoryData>(game_data.restrictedSkillCategories()) };
			if (tree.size()) datum.push_back(std::make_pair("restricted-categories", tree));
		}

		// Skill bonuses
		{
			pt::ptree tree{ getSkillPairTree<int>(game_data.skillBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-bonuses", tree));
		}

		// Everyman skill category choices
		{
			pt::ptree tree{ getGameDataChoiceSetTree<SkillCategoryData>(game_data.categoryEverymanSkillChoices()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-category-choices-everyman", tree));
		}

	}

} // namespace rm::rule::parser