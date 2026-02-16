#include <CultureData.h>
#include <CultureDatafileParserXml.h>
#include <SkillDevelopmentType.h>

void CultureDatafileParserXml::parse() {
	std::cout << "Loading Culture data ...\n";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		CultureData& ref = factory().get<CultureData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));
		ref.setHighCulture(v.second.get<bool>("high-culture", false));

		// Get the culture type from the cache
		std::string culture_type_id = GameRuleData::generateId("CultureType", v.second.get<std::string>("culture-type"));
		ref.setCultureType(factory().get<CultureTypeData>(culture_type_id));

		// Background languages
		if (boost::optional<const pt::ptree&> languages_tree = v.second.get_child_optional("background-languages")) {
			for (const auto& language : languages_tree.get()) {
				std::string language_name{ GameRuleData::generateId("Language", language.second.get<std::string>("<xmlattr>.language")) };
				LanguageAbility ability(factory().get<LanguageData>(language_name));
				for (const auto& form_tree : language.second) {
					int ranks{ form_tree.second.get<int>("<xmlattr>.ranks", 0) };
					std::string form{ form_tree.second.get_value<std::string>() };
					if (form == "Somantic") ability.updateSomanticRanks(ranks);
					else if (form == "Spoken") ability.updateSpokenRanks(ranks);
					else if (form == "Written") ability.updateWrittenRanks(ranks);
				}
				ref.addLanguageAbility(ability);
			}
		}

		// Hobby skills
		if (boost::optional<const pt::ptree&> hobby_skills = v.second.get_child_optional("hobby-skills")) {
			for (const auto& skill : hobby_skills.get()) {
				std::string string_val = skill.second.get_value<std::string>();
				ref.addHobbySkill(getSubcategory(string_val));
			}
		}

		// Hobby Categories
		if (boost::optional<const pt::ptree&> hobby_categories = v.second.get_child_optional("hobby-categories")) {
			for (const auto& category : hobby_categories.get()) {
				std::string string_val = category.second.get_value<std::string>();
				ref.addHobbySkillCategory(factory().get<SkillCategoryData>(string_val));
			}
		}

		// Preferred professions
		if (boost::optional<const pt::ptree&> preferred_professions = v.second.get_child_optional("preferred-professions")) {
			for (const auto& profession : preferred_professions.get()) {
				std::string string_val = profession.second.get_value<std::string>();
				ref.addPreferredProfession(factory().get<ProfessionData>(string_val));
			}
		}

		// Restricted professions
		if (boost::optional<const pt::ptree&> restricted_professions = v.second.get_child_optional("restricted-professions")) {
			for (const auto& profession : restricted_professions.get()) {
				std::string string_val = profession.second.get_value<std::string>();
				ref.addRestrictedProfession(factory().get<ProfessionData>(string_val));
			}
		}

		// Training package percentage costs
		if (boost::optional<const pt::ptree&> training_package_modifiers = v.second.get_child_optional("training-package-percentage-costs")) {
			for (const auto& modifier : training_package_modifiers.get()) {
				std::string package_name = modifier.second.get<std::string>("<xmlattr>.training-package");
				float cost_modifier = modifier.second.get_value<float>(100);
				if (cost_modifier <= 0) {
					std::cerr << "Invalid cost modifier for culture " << ref.name() << " and training package " << package_name << ": " << cost_modifier << ". Defaulting to 100%.\n";
					cost_modifier = 100;
				} else if (cost_modifier > 1000) {
					std::cerr << "Unrealistically high cost modifier for culture " << ref.name() << " and training package " << package_name << ": " << cost_modifier << ". Defaulting to 100%.\n";
					cost_modifier = 100;
				} else if (cost_modifier == 100) {
					continue; // No need to add a modifier if it's 100%
				}

				ref.addTrainingPackageModifier(factory().get<TrainingPackageData>(package_name), cost_modifier/100.0f);
			}
		}

		std::cout << "\tCulture name: " << ref.name() << std::endl;

	}
}

const SubcategoriedSkillData& CultureDatafileParserXml::getSubcategory(const std::string& val) {
	std::vector<std::string> parts = tokenise(val, "{}");
	if (parts.size() == 1) {
		return factory().subcategoriedSkillData(parts[0]);
	}
	else if (parts.size() == 2) {
		return factory().subcategoriedSkillData(parts[0], parts[1]);
	}
	else {
		throw std::runtime_error("Invalid skill subcategory format: " + val);
	}
}