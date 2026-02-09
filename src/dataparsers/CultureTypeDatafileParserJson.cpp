#include <CultureTypeDatafileParserJson.h>


void CultureTypeDatafileParserJson::parse() {
	std::cout << "Loading CultureType data ... " << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		CultureTypeData& ref = factory().get<CultureTypeData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));
		ref.setCharacterConcept(v.second.get<std::string>("character-concepts"));
		ref.setClothing(v.second.get<std::string>("clothing"));
		ref.setAspirations(v.second.get<std::string>("aspirations"));
		ref.setFears(v.second.get<std::string>("fears"));
		ref.setMarriagePatterns(v.second.get<std::string>("marriage-patterns"));
		ref.setPrejudices(v.second.get<std::string>("prejudices"));
		ref.setReligiousBeliefs(v.second.get<std::string>("religious-beliefs"));
		ref.setHobbySkillRanks(v.second.get<int>("hobby-skill-ranks"));
		boost::optional<int> spell_ranks = v.second.get_optional<int>("spell-list-ranks");
		if (spell_ranks) ref.setSpellListRanks(spell_ranks.value());

		// Get the preferred armour
		for (const auto& armours_tree : v.second.get_child("preferred-armours")) {
			std::optional<ArmourType::Type> armour = ArmourType::fromDescription(armours_tree.second.get_value<std::string>());
			if (armour) ref.addPreferredArmour(armour.value());
		}

		// Get the preferred weapons
		ref.setPreferredWeapons(parseGameDataSetTree<WeaponTypeData>(v.second.get_child_optional("preferred-weapons")));

		// Get skill ranks
		ref.setSkillRanks(parseSkillPairTree<int>(v.second.get_child_optional("skill-ranks")));

		// Get skill category ranks
		ref.setSkillCategoryRanks(parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-ranks")));

		// Get skill category skill ranks
		ref.setSkillCategorySkillRanks(parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-skill-ranks")));

		// Get required climates
		ref.setRequiredClimates(parseGameDataSetTree<ClimateData>(v.second.get_child_optional("required-climates")));

		// Get required environmental special features
		if (boost::optional<const pt::ptree&> required_features = v.second.get_child_optional("required-features")) {
			for (const auto& required_feature : required_features.get()) {
				std::optional<EnvironmentType::Feature> feature = EnvironmentType::feature(required_feature.second.get_value<std::string>());
				if (feature) ref.addRequiredFeature(feature.value());
			}
		}

		// Get required environmental terrains
		if (boost::optional<const pt::ptree&> required_terrains = v.second.get_child_optional("required-terrains")) {
			for (const auto& required_terrain : required_terrains.get()) {
				std::optional<EnvironmentType::Terrain> terrain = EnvironmentType::terrain(required_terrain.second.get_value<std::string>());
				if (terrain) ref.addRequiredTerrain(terrain.value());
			}
		}

		// Get required environmental vegetations
		if (boost::optional<const pt::ptree&> required_vegetations = v.second.get_child_optional("required-vegetations")) {
			for (const auto& required_vegetation : required_vegetations.get()) {
				std::optional<EnvironmentType::Vegetation> vegetation = EnvironmentType::vegetation(required_vegetation.second.get_value<std::string>());
				if (vegetation) ref.addRequiredVegetation(vegetation.value());
			}
		}

		// Get required environmental water sources
		if (boost::optional<const pt::ptree&> required_water_sources = v.second.get_child_optional("required-water-sources")) {
			for (const auto& required_water_source : required_water_sources.get()) {
				std::optional<EnvironmentType::Water> water = EnvironmentType::water(required_water_source.second.get_value<std::string>());
				if (water) ref.addRequiredWaterSource(water.value());
			}
		}

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

	// Preferred weapons
	{
		pt::ptree tree{ getGameDataSetTree<WeaponTypeData>(game_data.preferredWeapons()) };
		if (tree.size()) datum.push_back(std::make_pair("preferred-weapons", tree));
	}
	

	// Skill ranks
	{
		pt::ptree tree{ getSkillPairTree<int>(game_data.skillRanks()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-ranks", tree));
	}

	// Skill category ranks
	{
		pt::ptree tree{ getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategoryRanks()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-category-ranks", tree));
	}

	// Skill category skill ranks
	{
		pt::ptree tree{ getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategorySkillRanks()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-category-skill-ranks", tree));
	}

	// Required Climates
	{
		pt::ptree tree{ getGameDataSetTree<ClimateData>(game_data.requiredClimates()) };
		if (tree.size()) datum.push_back(std::make_pair("required-climates", tree));
	}

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