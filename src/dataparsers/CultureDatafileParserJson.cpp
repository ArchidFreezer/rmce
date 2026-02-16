#include <RealmType.h>
#include <CultureData.h>
#include <CultureDatafileParserJson.h>

void CultureDatafileParserJson::parse() {
	std::cout << "Loading Culture data ..." << std::endl;

	// Get the books to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		CultureData& ref = factory().get<CultureData>(id);
		ref.setName(name);

		// Description is optional
		std::string description = v.second.get<std::string>("description", "");
		if (!description.empty()) ref.setDescription(description);

		// Culture type is required
		std::string culture_type_id = v.second.get<std::string>("culture-type");
		ref.setCultureType(factory().get<CultureTypeData>(culture_type_id));

		// High culture is optional, defaulting to false
		ref.setHighCulture(v.second.get<bool>("high-culture", false));

		// Background languages are optional
		ref.setLanguageAbilities(parseLanguageAbilityMapTree(v.second.get_child_optional("background-languages")));

		// Hobby skills are optional
		ref.setHobbySkills(parseSkillSetTree(v.second.get_child_optional("hobby-skills")));

		// Hobby Categories are optional
		ref.setHobbySkillCategories(parseGameDataSetTree<SkillCategoryData>(v.second.get_child_optional("hobby-categories")));

		// Preferred professions are optional
		ref.setPreferredProfessions(parseGameDataSetTree<ProfessionData>(v.second.get_child_optional("preferred-professions")));

		// Restricted professions are optional
		ref.setRestrictedProfessions(parseGameDataSetTree<ProfessionData>(v.second.get_child_optional("restricted-professions")));

		// Training package modifiers are optional
		ref.setTrainingPackageModifiers(parseGameDataPairTree<TrainingPackageData, float>(v.second.get_child_optional("training-package-modifiers")));

		std::cout << "\tSpell List name: " << ref.name() << std::endl;

	}
	std::cout << " done" << std::endl;

}

void CultureDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	CultureData& game_data = factory().get<CultureData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	if (!game_data.description().empty()) datum.put("description", game_data.description());
	datum.put("culture-type", game_data.cultureType().id());
	datum.put("high-culture", game_data.highCulture());
	
	// Background languages
	{
		pt::ptree tree{ getLanguageAbilityMapTree(game_data.languageAbilities()) };
		if (tree.size()) datum.push_back(std::make_pair("background-languages", tree));
	}

	// Hobby skills
	{
		pt::ptree tree{ getSkillSetTree(game_data.hobbySkills()) };
		if (tree.size()) datum.push_back(std::make_pair("hobby-skills", tree));
	}

	// Hobby Categories
	{
		pt::ptree tree{ getGameDataSetTree<SkillCategoryData>(game_data.hobbySkillCategories()) };
		if (tree.size()) datum.push_back(std::make_pair("hobby-categories", tree));
	}

	// Preferred professions
	{
		pt::ptree tree{ getGameDataSetTree<ProfessionData>(game_data.preferredProfessions()) };
		if (tree.size()) datum.push_back(std::make_pair("preferred-professions", tree));
	}

	// Restricted professions
	{
		pt::ptree tree{ getGameDataSetTree<ProfessionData>(game_data.restrictedProfessions()) };
		if (tree.size()) datum.push_back(std::make_pair("restricted-professions", tree));
	}

	// Training package modifiers
	{
		pt::ptree tree{ getGameDataPairTree<TrainingPackageData, float>(game_data.trainingPackageModifiers()) };
		if (tree.size()) datum.push_back(std::make_pair("training-package-modifiers", tree));
	}
}
