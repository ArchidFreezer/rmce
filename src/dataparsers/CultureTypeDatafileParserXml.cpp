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
						ref.setSkillRanks(factory().subcategoriedSkillData(skill_id, subcategory.value()), ranks);
					} else {
						ref.setSkillRanks(factory().subcategoriedSkillData(skill_id), ranks);
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
					ref.addSkillCategoryRanks(factory().get<SkillCategoryData>(skill_id), ranks);
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
					ref.addSkillCategorySkillRanks(factory().get<SkillCategoryData>(skill_id), ranks);
				}
			}
		}

		std::cout << "\tCultureType name: " << ref.name() << std::endl;
	}
}