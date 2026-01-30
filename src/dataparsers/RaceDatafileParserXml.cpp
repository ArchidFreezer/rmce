#include <RaceData.h>
#include <RaceDatafileParserXml.h>
#include <RealmType.h>

void RaceDatafileParserXml::parse(bool id_only) {
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
			ref.setDescription(v.second.get<std::string>("description"));
			ref.setHighCulture(v.second.get<bool>("high-culture"));
			ref.setRecoveryMultiplier(v.second.get<float>("recovery-multiplier"));
			ref.setNumBackgroundOptions(v.second.get<int>("background-options"));
			ref.setExhaustionBonus(v.second.get<int>("exhaustion-bonus"));
			ref.setStatLossRacialType(v.second.get<int>("racial-type"));
			ref.setRequiredSleep(v.second.get<int>("required-sleep"));
			ref.setRequiredSleepFrequency(v.second.get<int>("required-sleep-frequency"));
			ref.setSoulDeparture(v.second.get<int>("soul-departure"));
			ref.setBuildModifier(v.second.get<int>("physique-constructor.male-build-modifier"));
			ref.setAverageMaleHeight(v.second.get<int>("physique-constructor.male-height"));
			ref.setAverageFemaleHeight(v.second.get<int>("physique-constructor.female-height"));
			ref.setAverageLifespan(v.second.get<int>("physique-constructor.lifespan"));
			ref.setMaleWeightModifier(v.second.get<float>("physique-constructor.male-weight-modifier"));
			ref.setFemaleWeightModifier(v.second.get<float>("physique-constructor.female-weight-modifier"));

			// Get the book from the cache
			std::string book_id = GameRuleData::generateId("Book", v.second.get<std::string>("book"));
			ref.setBook(cache().get<BookData>(book_id));

			// Get the CreatureSizeType
			std::string size_name = v.second.get<std::string>("size");
			if (CreatureSizeType::fromString(size_name)) {
				ref.setSize(CreatureSizeType::fromString(size_name).value());
			} else {
				throw CreatureSizeType::CreatureSizeTypeNotFoundException("Could not find a creature size for: " + size_name);
			}

			// Get the critical table
			std::string crit_table = v.second.get<std::string>("critical-table");
			if (CriticalTableType::fromString(crit_table)) {
				ref.setCriticalTableType(CriticalTableType::fromString(crit_table).value());
			} else {
				throw CriticalTableType::CriticalTableTypeNotFoundException("Could not find a critical table for: " + crit_table);
			}

			// Get realm modifiers
			for (const auto& realm_progression : v.second.get_child("realm-progressions")) {
				std::string realm = realm_progression.second.get<std::string>("<xmlattr>.realm");
				std::string progression = realm_progression.second.get_value<std::string>();
				switch (RealmType::fromString(realm).value()) {
				case (RealmType::kArcane): ref.setArcaneProgression(cache().get<SkillProgressionTypeData>(progression)); break;
				case (RealmType::kArms): ref.setArmsProgression(cache().get<SkillProgressionTypeData>(progression)); break;
				case (RealmType::kChanneling): ref.setChannelingProgression(cache().get<SkillProgressionTypeData>(progression)); break;
				case (RealmType::kEssence): ref.setEssenceProgression(cache().get<SkillProgressionTypeData>(progression)); break;
				case (RealmType::kMentalism): ref.setMentalismProgression(cache().get<SkillProgressionTypeData>(progression)); break;
				}
			}

			// Get starting languages
			for (const auto& starting_language : v.second.get_child("starting-languages")) {
				std::string language_name{ GameRuleData::generateId("Language", starting_language.second.get<std::string>("<xmlattr>.language")) };
				LanguageAbility ability(cache().get<LanguageData>(language_name));
				for (const auto& form_tree : starting_language.second) {
					int ranks{ form_tree.second.get<int>("<xmlattr>.ranks", 0) };
					std::string form{ form_tree.second.get_value<std::string>() };
					if (form == "Somantic") ability.updateSomanticRanks(ranks);
					else if (form == "Spoken") ability.updateSpokenRanks(ranks);
					else if (form == "Written") ability.updateWrittenRanks(ranks);
				}
				ref.setStartingLanguageAbility(ability);
			}

			// Get adolescent languages
			for (const auto& starting_language : v.second.get_child("adolescent-languages")) {
				std::string language_name{ GameRuleData::generateId("Language", starting_language.second.get<std::string>("<xmlattr>.language")) };
				LanguageAbility ability(cache().get<LanguageData>(language_name));
				for (const auto& form_tree : starting_language.second) {
					int ranks{ form_tree.second.get<int>("<xmlattr>.ranks", 0) };
					std::string form{ form_tree.second.get_value<std::string>() };
					if (form == "Somantic") ability.updateSomanticRanks(ranks);
					else if (form == "Spoken") ability.updateSpokenRanks(ranks);
					else if (form == "Written") ability.updateWrittenRanks(ranks);
				}
				ref.setAdolescentLanguageAbility(ability);
			}

			// Get Stat bonuses
			if (boost::optional<const pt::ptree&> stat_bonuses = v.second.get_child_optional("stat-bonuses")) {
				for (const auto& stat_bonus : stat_bonuses.get()) {
					StatType::Type stat{ StatType::fromString(stat_bonus.second.get<std::string>("<xmlattr>.stat")).value() };
					int bonus = stat_bonus.second.get_value<int>();
					if (bonus) ref.setStatBonus(stat, bonus);
				}
			}

			// Everyman skills
			if (boost::optional<const pt::ptree&> everyman_skills = v.second.get_child_optional("skills-everyman")) {
				for (const auto& everyman_skill : everyman_skills.get()) {
					std::string skill_name{ GameRuleData::generateId("Skill", everyman_skill.second.get_value<std::string>()) };
					std::string attr = everyman_skill.second.get<std::string>("<xmlattr>.subcategory", "");
					std::unique_ptr<SubcategoriedSkillData> everyman_data;
					if (!attr.empty()) everyman_data = std::make_unique<SubcategoriedSkillData>(cache().get<SkillData>(skill_name), attr);
					else everyman_data = std::make_unique<SubcategoriedSkillData>(cache().get<SkillData>(skill_name));
					ref.addEverymanSkill(std::move(everyman_data));
				}
			}

			// Restricted skills
			if (boost::optional<const pt::ptree&> restricted_skills = v.second.get_child_optional("skills-restricted")) {
				for (const auto& restricted_skill : restricted_skills.get()) {
					std::string skill_name{ GameRuleData::generateId("Skill", restricted_skill.second.get_value<std::string>()) };
					std::string attr = restricted_skill.second.get<std::string>("<xmlattr>.subcategory", "");
					std::unique_ptr<SubcategoriedSkillData> everyman_data;
					if (!attr.empty()) everyman_data = std::make_unique<SubcategoriedSkillData>(cache().get<SkillData>(skill_name), attr);
					else everyman_data = std::make_unique<SubcategoriedSkillData>(cache().get<SkillData>(skill_name));
					ref.addRestrictedSkill(std::move(everyman_data));
				}
			}

			// Everyman skill categories
			if (boost::optional<const pt::ptree&> everyman_categories = v.second.get_child_optional("skill-categories-everyman")) {
				for (const auto& everyman_category : everyman_categories.get()) {
					std::string category_name{ GameRuleData::generateId("SkillCategory", everyman_category.second.get_value<std::string>()) };
					ref.addEverymanSkillCategory(cache().get<SkillCategoryData>(category_name));
				}
			}

			// Restricted skill categories
			if (boost::optional<const pt::ptree&> restricted_categories = v.second.get_child_optional("skill-categories-restricted")) {
				for (const auto& restricted_category : restricted_categories.get()) {
					std::string category_name{ GameRuleData::generateId("SkillCategory", restricted_category.second.get_value<std::string>()) };
					ref.addRestrictedSkillCategory(cache().get<SkillCategoryData>(category_name));
				}
			}

			// Get Skill bonuses
			if (boost::optional<const pt::ptree&> skill_bonuses = v.second.get_child_optional("skills-bonus")) {
				for (const auto& skill_bonus : skill_bonuses.get()) {
					int bonus = skill_bonus.second.get<int>("<xmlattr>.bonus");
					std::string skill_name{ GameRuleData::generateId("Skill", skill_bonus.second.get_value<std::string>()) };
					std::string attr = skill_bonus.second.get<std::string>("<xmlattr>.subcategory", "");
					std::unique_ptr<SubcategoriedSkillData> skill_data;
					if (!attr.empty()) skill_data = std::make_unique<SubcategoriedSkillData>(cache().get<SkillData>(skill_name), attr);
					else skill_data = std::make_unique<SubcategoriedSkillData>(cache().get<SkillData>(skill_name));
					ref.setSkillBonus(std::move(skill_data), bonus);
				}
			}

			// Get the skill category everyman choices
			if (boost::optional<const pt::ptree&> category_choices = v.second.get_child_optional("skill-category-choices-everyman")) {
				for (const auto& choice : category_choices.get()) {

					std::unique_ptr<GameRuleDataChoice<SkillCategoryData>> choice_data = std::make_unique< GameRuleDataChoice<SkillCategoryData>>();
					choice_data->setNumChoices(choice.second.get<int>("<xmlattr>.num", 0));

					for (const auto& category_tree : choice.second) {
						std::string category_name{ category_tree.second.get_value<std::string>() };
						if (!category_name.empty()) {
							std::string category_id{ GameRuleData::generateId("SkillCategory", category_name) };
							choice_data->addOption(cache().get<SkillCategoryData>(category_id));
						}
					}
					ref.addCategoryEverymanSkillChoice(std::move(choice_data));
				}
			}

			std::cout << "\tRace name: " << ref.name() << std::endl;
		}
	}
}