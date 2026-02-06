#include <CultureTypeData.h>
#include <CultureTypeDatafileParserXml.h>

void CultureTypeDatafileParserXml::parse() {
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
		ref.setAspirations(v.second.get<std::string>("cultural-aspirations"));
		ref.setFears(v.second.get<std::string>("collective-fears"));
		ref.setMarriagePatterns(v.second.get<std::string>("marriage-patterns"));
		ref.setPrejudices(v.second.get<std::string>("prejudices"));
		ref.setReligiousBeliefs(v.second.get<std::string>("religious-beliefs"));
		ref.setHobbySkillRanks(v.second.get<int>("hobby-skill-ranks"));

		// Get spell list ranks if there are any
		boost::optional<int> spell_ranks = v.second.get_optional<int>("spell-list-rank-modifiers");
		if (spell_ranks) ref.setSpellListRanks(spell_ranks.value());

		// Get the preferred armour types
		for (const auto& armours_tree : v.second.get_child("preferred-armours")) {
			std::string armour_name = armours_tree.second.get_value<std::string>();
			ref.addPreferredArmour(ArmourType::fromDescription(armour_name).value());
		}

		// Get the preferred weapons
		for (const auto& weapons_tree : v.second.get_child("preferred-weapons")) {
			std::string weapon_id = GameRuleData::generateId("WeaponType", weapons_tree.second.get_value<std::string>());
			ref.addPreferredWeapon(factory().get<WeaponTypeData>(weapon_id));
		}

		// Skill Ranks
		if (boost::optional<const pt::ptree&> skill_ranks = v.second.get_child_optional("adolescent-skill-rank-modifiers")) {
			for (const auto& skill_rank : skill_ranks.get()) {
				// If there are ranks to set then add the skills to the appropriate container
				int ranks = skill_rank.second.get_value<int>();
				if (ranks) { // The XML file has some ranks set to 0 for convenience of construction so we need to handle that
					std::string skill_id = GameRuleData::generateId("Skill", skill_rank.second.get<std::string>("<xmlattr>.skill"));
					boost::optional<std::string> subcategory = skill_rank.second.get_optional<std::string>("<xmlattr>.subcategory");
					if (subcategory) {
						ref.addSkillRank(factory().subcategoriedSkillData(skill_id, subcategory.value()), ranks);
					} else {
						ref.addSkillRank(factory().subcategoriedSkillData(skill_id), ranks);
					}
				}
			}
		}

		// Category Ranks
		if (boost::optional<const pt::ptree&> skill_ranks = v.second.get_child_optional("adolescent-category-rank-modifiers")) {
			for (const auto& skill_rank : skill_ranks.get()) {
				// If there are ranks to set then add the skills to the appropriate container
				int ranks = skill_rank.second.get_value<int>();
				if (ranks) { // The XML file has some ranks set to 0 for convenience of construction so we need to handle that
					std::string skill_id = GameRuleData::generateId("SkillCategory", skill_rank.second.get<std::string>("<xmlattr>.category"));
					ref.addSkillCategoryRank(factory().get<SkillCategoryData>(skill_id), ranks);
				}
			}
		}

		// Category Skill Ranks
		if (boost::optional<const pt::ptree&> skill_ranks = v.second.get_child_optional("adolescent-category-skill-rank-modifiers")) {
			for (const auto& skill_rank : skill_ranks.get()) {
				// If there are ranks to set then add the skills to the appropriate container
				int ranks = skill_rank.second.get_value<int>();
				if (ranks) { // The XML file has some ranks set to 0 for convenience of construction so we need to handle that
					std::string skill_id = GameRuleData::generateId("SkillCategory", skill_rank.second.get<std::string>("<xmlattr>.category"));
					ref.addSkillCategorySkillRank(factory().get<SkillCategoryData>(skill_id), ranks);
				}
			}
		}

		// Required Climates
		if (boost::optional<const pt::ptree&> required_climates = v.second.get_child_optional("climates")) {
			for (const auto& required_climate : required_climates.get()) {
				std::string climate_id = GameRuleData::generateId("ClimateType", required_climate.second.get_value<std::string>());
				ref.addRequiredClimate(factory().get<ClimateData>(climate_id));
			}
		}

		// Required Special Features
		if (boost::optional<const pt::ptree&> required_features = v.second.get_child_optional("special-features")) {
			for (const auto& required_feature : required_features.get()) {
				ref.addRequiredFeature(EnvironmentType::feature(required_feature.second.get_value<std::string>()).value());
			}
		}

		// Required Terrains
		if (boost::optional<const pt::ptree&> required_terrains = v.second.get_child_optional("terrains")) {
			for (const auto& required_terrain : required_terrains.get()) {
				ref.addRequiredTerrain(EnvironmentType::terrain(required_terrain.second.get_value<std::string>()).value());
			}
		}

		// Required Vegetations
		if (boost::optional<const pt::ptree&> required_vegetations = v.second.get_child_optional("vegetations")) {
			for (const auto& required_vegetation : required_vegetations.get()) {
				ref.addRequiredVegetation(EnvironmentType::vegetation(required_vegetation.second.get_value<std::string>()).value());
			}
		}

		// Required Special Features
		if (boost::optional<const pt::ptree&> required_water_sources = v.second.get_child_optional("water-sources")) {
			for (const auto& required_water_source : required_water_sources.get()) {
				ref.addRequiredWaterSource(EnvironmentType::water(required_water_source.second.get_value<std::string>()).value());
			}
		}

		std::cout << "\tCultureType name: " << ref.name() << std::endl;
	}
}