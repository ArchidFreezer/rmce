#include <CultureSerializer.h>
#include <GameRuleBoostConverter.h>

namespace rm::rule::parser {

void CultureSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading Culture data ..." << std::endl;

	// Get the cultures to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a CultureData object from the object manager
		CultureData& ref = manager().get<CultureData>(id);
		ref.setName(name);

		// Description is optional
		std::string description = v.second.get<std::string>("description", "");
		if (!description.empty())
			ref.setDescription(description);

		// Culture type is required
		std::string culture_type_id = v.second.get<std::string>("culture-type");
		ref.setCultureType(manager().get<CultureTypeData>(culture_type_id));

		// High culture is optional, defaulting to false
		ref.setHighCulture(v.second.get<bool>("high-culture", false));

		GameRuleBoostConverter converter{manager()};

		// Background languages are optional
		ref.setLanguageAbilities(converter.parseLanguageAbilityMapTree(v.second.get_child_optional("background-languages")));

		// Hobby skills are optional
		ref.setHobbySkills(converter.parseSkillSetTree(v.second.get_child_optional("hobby-skills")));

		// Hobby Categories are optional
		ref.setHobbySkillCategories(converter.parseGameDataSetTree<SkillCategoryData>(v.second.get_child_optional("hobby-categories")));

		// Preferred professions are optional
		ref.setPreferredProfessions(converter.parseGameDataSetTree<ProfessionData>(v.second.get_child_optional("preferred-professions")));

		// Restricted professions are optional
		ref.setRestrictedProfessions(converter.parseGameDataSetTree<ProfessionData>(v.second.get_child_optional("restricted-professions")));

		// Training package modifiers are optional
		ref.setTrainingPackageModifiers(converter.parseGameDataPairTree<TrainingPackageData, float>(v.second.get_child_optional("training-package-modifiers")));
		std::cout << "\tCulture name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void CultureSerializer::populateDatum(std::string key, pt::ptree& datum) {
	CultureData& game_data = manager().get<CultureData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	if (!game_data.description().empty())
		datum.put("description", game_data.description());
	datum.put("culture-type", game_data.cultureType().id());
	datum.put("high-culture", game_data.highCulture());

	GameRuleBoostConverter converter{manager()};

	// Background languages
	{
		pt::ptree tree{converter.getLanguageAbilityMapTree(game_data.languageAbilities())};
		if (tree.size())
			datum.push_back(std::make_pair("background-languages", tree));
	}

	// Hobby skills
	{
		pt::ptree tree{converter.getSkillSetTree(game_data.hobbySkills())};
		if (tree.size())
			datum.push_back(std::make_pair("hobby-skills", tree));
	}

	// Hobby Categories
	{
		pt::ptree tree{converter.getGameDataSetTree<SkillCategoryData>(game_data.hobbySkillCategories())};
		if (tree.size())
			datum.push_back(std::make_pair("hobby-categories", tree));
	}

	// Preferred professions
	{
		pt::ptree tree{converter.getGameDataSetTree<ProfessionData>(game_data.preferredProfessions())};
		if (tree.size())
			datum.push_back(std::make_pair("preferred-professions", tree));
	}

	// Restricted professions
	{
		pt::ptree tree{converter.getGameDataSetTree<ProfessionData>(game_data.restrictedProfessions())};
		if (tree.size())
			datum.push_back(std::make_pair("restricted-professions", tree));
	}

	// Training package modifiers
	{
		pt::ptree tree{converter.getGameDataPairTree<TrainingPackageData, float>(game_data.trainingPackageModifiers())};
		if (tree.size())
			datum.push_back(std::make_pair("training-package-modifiers", tree));
	}
}

std::string CultureSerializer::rootNode() const {
	return "cultures";
}

} // namespace rm::rule::parser