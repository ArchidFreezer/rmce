#include <CultureTypeDatafileParserJson.h>

using namespace rm::rule::enums;

namespace rm::rule::parser {

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
			ref.setPreferredArmours(parseEnumSetTree<ArmourType::Type>(v.second.get_child_optional("preferred-armours")));

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
			ref.setRequiredFeatures(parseEnumSetTree<EnvironmentType::Feature>(v.second.get_child_optional("required-features")));

			// Get required environmental terrains
			ref.setRequiredTerrains(parseEnumSetTree<EnvironmentType::Terrain>(v.second.get_child_optional("required-terrains")));

			// Get required environmental vegetations
			ref.setRequiredVegetations(parseEnumSetTree<EnvironmentType::Vegetation>(v.second.get_child_optional("required-vegetations")));

			// Get required environmental water sources
			ref.setRequiredWaterSources(parseEnumSetTree<EnvironmentType::Water>(v.second.get_child_optional("required-water-sources")));

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
		{
			pt::ptree tree{ getEnumSetTree<ArmourType::Type>(game_data.preferredArmour()) };
			if (tree.size()) datum.push_back(std::make_pair("preferred-armours", tree));
		}

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
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Feature>(game_data.requiredFeatures()) };
			if (tree.size()) datum.push_back(std::make_pair("required-features", tree));
		}

		// Required environmental terrains
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Terrain>(game_data.requiredTerrains()) };
			if (tree.size()) datum.push_back(std::make_pair("required-terrains", tree));
		}

		// Required environmental vegetation
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Vegetation>(game_data.requiredVegetations()) };
			if (tree.size()) datum.push_back(std::make_pair("required-vegetations", tree));
		}

		// Required environmental water sources
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Water>(game_data.requiredWaterSources()) };
			if (tree.size()) datum.push_back(std::make_pair("required-water-sources", tree));
		}

	}

} // namespace rm::rule::parser