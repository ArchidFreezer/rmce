#include <CultureTypeDatafileParserJson.h>


void CultureTypeDatafileParserJson::parse() {
	std::cout << "Loading CultureType data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		CultureTypeData& ref = factory().get<CultureTypeData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));


		std::cout << "\tCultureType name: " << ref.name() << std::endl;
	}

}

void CultureTypeDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	CultureTypeData& game_data = factory().get<CultureTypeData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	datum.put("character-concepts", game_data.characterConcept());
	datum.put("clothing", game_data.clothing());
	datum.put("aspirations", game_data.aspirations());
	datum.put("fears", game_data.fears());
	datum.put("marriage-patterns", game_data.marriagePatterns());
	datum.put("prejudices", game_data.prejudices());
	datum.put("religious-beliefs", game_data.religiousBeliefs());
	datum.put("hobby-skill-ranks", game_data.hobbySkillRanks());
	if (game_data.spellListRanks()) datum.put("spell-list-ranks", game_data.spellListRanks());

	// Get the container tree for the preferred armour
	pt::ptree armours_tree{};
	for (ArmourType::Type armour : game_data.preferredArmour()) {
		// Get the armours container
		pt::ptree armour_tree{};
		armour_tree.put("", ArmourType::description(armour));
		armours_tree.push_back(std::make_pair("", armour_tree));
	}
	datum.push_back(std::make_pair("preferred-armours", armours_tree));

	// Get the container tree for the preferred weapons
	pt::ptree weapons_tree{};
	for (const WeaponTypeData* weapon : game_data.preferredWeapons()) {
		// Get the weapons container
		pt::ptree weapon_tree{};
		weapon_tree.put("", weapon->id());
		weapons_tree.push_back(std::make_pair("", weapon_tree));
	}
	datum.push_back(std::make_pair("preferred-weapons", weapons_tree));

	// Skill ranks
	pt::ptree skill_ranks_tree{};
	for (auto& skill : game_data.skillsWithRanks()) {
		pt::ptree skill_rank_tree{};
		skill_rank_tree.put("skill", skill.skillData().id());
		if (skill.subcategory()) skill_rank_tree.put("subcategory", skill.subcategory().value());
		skill_rank_tree.put("rank", game_data.skillRank(skill));
		skill_ranks_tree.push_back(std::make_pair("", skill_rank_tree));
	}
	if (!skill_ranks_tree.empty()) datum.push_back(std::make_pair("skill-ranks", skill_ranks_tree));

	// Skill category ranks
	{
		pt::ptree skill_category_ranks_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithRanks()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_rank_tree{};
			skill_category_rank_tree.put("category", skill_category.first);
			skill_category_rank_tree.put("rank", game_data.skillCategoryRank(*skill_category.second));
			skill_category_ranks_tree.push_back(std::make_pair("", skill_category_rank_tree));
		}
		if (skill_category_ranks_tree.size()) datum.push_back(std::make_pair("skill-category-ranks", skill_category_ranks_tree));
	}

	// Skill category ranks
	{
		pt::ptree skill_category_ranks_tree{};
		std::map<std::string, const SkillCategoryData*> categories{};
		for (auto& skill_category : game_data.skillCategoriesWithSkillRanks()) {
			categories.emplace(skill_category->id(), skill_category);
		}
		for (const auto& skill_category : categories) {
			pt::ptree skill_category_rank_tree{};
			skill_category_rank_tree.put("category", skill_category.first);
			skill_category_rank_tree.put("rank", game_data.skillCategorySkillRank(*skill_category.second));
			skill_category_ranks_tree.push_back(std::make_pair("", skill_category_rank_tree));
		}
		if (skill_category_ranks_tree.size()) datum.push_back(std::make_pair("skill-category-skill-ranks", skill_category_ranks_tree));
	}

	// Required Climates
	pt::ptree climates_tree{};
	for (const ClimateData* climate : game_data.requiredClimates()) {
		// Get the climates container
		pt::ptree climate_tree{};
		climate_tree.put("", climate->id());
		climates_tree.push_back(std::make_pair("", climate_tree));
	}
	if (!climates_tree.empty()) datum.push_back(std::make_pair("required-climates", climates_tree));

	// Required environmental special features
	pt::ptree features_tree{};
	for (EnvironmentType::Feature feature : game_data.requiredFeatures()) {
		pt::ptree feature_tree{};
		feature_tree.put("", EnvironmentType::toString(feature));
		features_tree.push_back(std::make_pair("", feature_tree));
	}
	if (!features_tree.empty()) datum.push_back(std::make_pair("required-features", features_tree));

	// Required environmental terrains
	pt::ptree terrains_tree{};
	for (EnvironmentType::Terrain terrain : game_data.requiredTerrains()) {
		pt::ptree terrain_tree{};
		terrain_tree.put("", EnvironmentType::toString(terrain));
		terrains_tree.push_back(std::make_pair("", terrain_tree));
	}
	if (!terrains_tree.empty()) datum.push_back(std::make_pair("required-terrains", terrains_tree));

	// Required environmental vegetation
	pt::ptree vegetations_tree{};
	for (EnvironmentType::Vegetation vegetation : game_data.requiredVegetations()) {
		pt::ptree vegetation_tree{};
		vegetation_tree.put("", EnvironmentType::toString(vegetation));
		vegetations_tree.push_back(std::make_pair("", vegetation_tree));
	}
	if (!vegetations_tree.empty()) datum.push_back(std::make_pair("required-vegetations", vegetations_tree));

	// Required environmental water sources
	pt::ptree waters_tree{};
	for (EnvironmentType::Water water : game_data.requiredWaterSources()) {
		pt::ptree water_tree{};
		water_tree.put("", EnvironmentType::toString(water));
		waters_tree.push_back(std::make_pair("", water_tree));
	}
	if (!waters_tree.empty()) datum.push_back(std::make_pair("required-water-sources", waters_tree));

}