#include <AnimalData.h>
#include <AnimalDatafileParserJson.h>
#include <NumberMatcherFactory.h>
#include <table/CreatureBonusXpTable.h>
#include <table/LevelVarianceTable.h>

void AnimalDatafileParserJson::parse() {
	std::cout << "Loading Animal data ..." << std::endl;

	buildCreatureBonusXpTable();
	buildLevelVarianceTable();

	// Get the books to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		AnimalData& ref = factory().get<AnimalData>(id);
		ref.setName(name);

		// Description is optional
		std::string description = v.second.get<std::string>("description", "");
		if (!description.empty()) ref.setDescription(description);
		ref.setBaseHits(v.second.get<int>("base-hits"));
		ref.setBaseMovement(v.second.get<int>("base-movement"));
		ref.setDefensiveBonus(v.second.get<int>("defensive-bonus"));
		ref.setFrequencyFactor(v.second.get<int>("frequency-code"));

		boost::optional<std::string> bonus_xp_code_str = v.second.get_optional<std::string>("bonus-xp-code");
		if (bonus_xp_code_str) {
			CreatureBonusXpType::Type xp_code{};
			CreatureBonusXpType::fromString(bonus_xp_code_str.value(), xp_code);
			ref.setBonusXpCode(xp_code);
		} else {
			// If the bonus XP code is not provided, set it to kNone
			ref.setBonusXpCode(CreatureBonusXpType::Type::kNone);
		}

		boost::optional<std::string> bonus_con_code_str = v.second.get_optional<std::string>("constitution-variance-type");
		if (bonus_con_code_str) {
			CreatureConstitutionVarianceType::Type constitution_code{};
			CreatureConstitutionVarianceType::fromString(bonus_con_code_str.value(), constitution_code);
			ref.setConstitutionVarianceType(constitution_code);
		} else {
			// If the bonus constitution code is not provided, set it to kNone
			ref.setConstitutionVarianceType(CreatureConstitutionVarianceType::Type::kNone);
		}

		std::string level_code_str = v.second.get<std::string>("level-variance-type");
		LevelVarianceType::Type level_code{};
		LevelVarianceType::fromString(level_code_str, level_code);
		ref.setLevelVarianceType(level_code);

		ref.setAverageLevel(v.second.get<int>("average-level")); 

		boost::optional<std::string> treasure_code_id = v.second.get_optional<std::string>("treasure-code");
		if (treasure_code_id) ref.setTreasureCode(factory().get<TreasureCodeData>(treasure_code_id.value()));

		CreatureSizeType::Type size_code{};
		CreatureSizeType::fromString(v.second.get<std::string>("size"), size_code);
		ref.setSize(size_code);

		ArmourType::Type armour_code{};
		ArmourType::fromString(v.second.get<std::string>("armour-type"), armour_code);
		ref.setArmourType(armour_code);

		CreatureMovementSpeedType::Type movement_speed_code{};
		CreatureMovementSpeedType::fromString(v.second.get<std::string>("movement-speed"), movement_speed_code);
		ref.setMovementSpeed(movement_speed_code);

		CreatureMovementSpeedType::Type attack_quickness_code{};
		CreatureMovementSpeedType::fromString(v.second.get<std::string>("attack-quickness"), attack_quickness_code);
		ref.setAttackQuickness(attack_quickness_code);

		std::string max_pace_id{ v.second.get<std::string>("max-pace") };
		ref.setMaxPace(factory().get<CreaturePaceData>(max_pace_id));

		AnimalOutlookType::Type outlook_code{};
		AnimalOutlookType::fromString(v.second.get<std::string>("outlook"), outlook_code);
		ref.setOutlook(outlook_code);

		CriticalSizeTableType::Type critical_table_code{};
		CriticalSizeTableType::fromString(v.second.get<std::string>("critical-table"), critical_table_code);
		ref.setCriticalTableType(critical_table_code);

		// Critical modifiers are optional
		// TODO: The data in the legacy XML file is incorrect and the json file needs to be manually updated.
		boost::optional<const pt::ptree&> critical_modifiers = v.second.get_child_optional("critical_modifiers");
		if (critical_modifiers) ref.setCriticalModifiers(parseEnumSetTree<CriticalModifierType::Type>(critical_modifiers));

		// Encounter range
		{
			boost::optional<const pt::ptree&> tree = v.second.get_child_optional("encounter-range");
			if (tree) {
				int min = tree->get<int>("min");
				int max = tree->get<int>("max");
				ref.setEncounterRange(std::make_pair(min, max));
			}
		}

		// Number young range
		{
			boost::optional<const pt::ptree&> tree = v.second.get_child_optional("number-young-range");
			if (tree) {
				int number_young_min = tree->get<int>("min");
				int number_young_max = tree->get<int>("max");
				ref.setNumberYoungRange(std::make_pair(number_young_min, number_young_max));
			}
		}

		// Location
		{
			boost::optional<const pt::ptree&> location_tree = v.second.get_child_optional("location");
			if (location_tree) {
				Location location{};
				boost::optional<const pt::ptree&> features_tree = location_tree->get_child_optional("features");
				if (features_tree) location.setFeatures(parseEnumSetTree<EnvironmentType::Feature>(features_tree));
				boost::optional<const pt::ptree&> terrains_tree = location_tree->get_child_optional("terrains");
				if (terrains_tree) location.setTerrains(parseEnumSetTree<EnvironmentType::Terrain>(terrains_tree));
				boost::optional<const pt::ptree&> vegetation_tree = location_tree->get_child_optional("vegetation");
				if (vegetation_tree) location.setVegetation(parseEnumSetTree<EnvironmentType::Vegetation>(vegetation_tree));
				boost::optional<const pt::ptree&> water_sources_tree = location_tree->get_child_optional("water-sources");
				if (water_sources_tree) location.setWater(parseEnumSetTree<EnvironmentType::Water>(water_sources_tree));
				boost::optional<const pt::ptree&> climates_tree = location_tree->get_child_optional("climates");
				if (climates_tree) location.setClimates(parseGameDataSetTree<ClimateData>(climates_tree));
				ref.setLocation(location);
			}
		}

		std::cout << "\tAnimal name: " << ref.name() << std::endl;

	}
	std::cout << " done" << std::endl;

}

void AnimalDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	AnimalData& game_data = factory().get<AnimalData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	if (!game_data.description().empty()) datum.put("description", game_data.description());
	datum.put("base-hits", game_data.baseHits());
	datum.put("base-movement", game_data.baseMovement());
	datum.put("defensive-bonus", game_data.defensiveBonus());
	datum.put("frequency-code", game_data.frequencyFactor());
	if (game_data.bonusXpCode() != CreatureBonusXpType::Type::kNone) datum.put("bonus-xp-code", CreatureBonusXpType::toString(game_data.bonusXpCode()));
	if (game_data.constitutionVarianceType() != CreatureConstitutionVarianceType::Type::kNone) datum.put("constitution-variance-type", CreatureConstitutionVarianceType::toString(game_data.constitutionVarianceType()));
	datum.put("level-variance-type", LevelVarianceType::toString(game_data.levelVarianceType()));
	datum.put("average-level", game_data.averageLevel());
	if (game_data.treasureCode()) datum.put("treasure-code", game_data.treasureCode().value()->id());
	datum.put("size", CreatureSizeType::toString(game_data.size()));
	datum.put("armour-type", ArmourType::toString(game_data.armourType()));
	datum.put("movement-speed", CreatureMovementSpeedType::toString(game_data.movementSpeed()));
	datum.put("attack-quickness", CreatureMovementSpeedType::toString(game_data.attackQuickness()));
	datum.put("max-pace", game_data.maxPace()->id());
	datum.put("outlook", AnimalOutlookType::toString(game_data.outlook()));
	datum.put("critical-table", CriticalSizeTableType::toString(game_data.criticalTableType()));

	// Critical modifiers are optional, only add to the tree if there are any
	{
		pt::ptree tree{ getEnumSetTree<CriticalModifierType::Type>(game_data.criticalModifiers()) };
		if (tree.size()) datum.push_back(std::make_pair("critical_modifiers", tree));
	}


	// Encounter range
	std::pair<int, int> encounter_range = game_data.encounterRange();
	if (encounter_range.first > 0 && encounter_range.second > 0) {
		pt::ptree encounter_range_tree{};
		encounter_range_tree.put("min", encounter_range.first);
		encounter_range_tree.put("max", encounter_range.second);
		datum.push_back(std::make_pair("encounter-range", encounter_range_tree));
	}

	// Number young range
	std::pair<int, int> number_young_range = game_data.numberYoungRange();
	if (number_young_range.first > 0 && number_young_range.second > 0) {
		pt::ptree number_young_range_tree{};
		number_young_range_tree.put("min", number_young_range.first);
		number_young_range_tree.put("max", number_young_range.second);
		datum.push_back(std::make_pair("number-young-range", number_young_range_tree));
	}

	// Location
	{
		pt::ptree location_tree{};
		Location location = game_data.location();

		// required features
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Feature>(location.features()) };
			if (tree.size()) location_tree.push_back(std::make_pair("features", tree));
		}
		// required terrains
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Terrain>(location.terrains()) };
			if (tree.size()) location_tree.push_back(std::make_pair("terrains", tree));
		}
		// required vegetation
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Vegetation>(location.vegetation()) };
			if (tree.size()) location_tree.push_back(std::make_pair("vegetation", tree));
		}
		// required water sources
		{
			pt::ptree tree{ getEnumSetTree<EnvironmentType::Water>(location.water()) };
			if (tree.size()) location_tree.push_back(std::make_pair("water-sources", tree));
		}
		// required climates
		{
			pt::ptree tree{ getGameDataSetTree<ClimateData>(location.climates()) };
			if (tree.size()) location_tree.push_back(std::make_pair("climates", tree));
		}
		if (location_tree.size()) datum.push_back(std::make_pair("location", location_tree));
	}

}

void AnimalDatafileParserJson::buildCreatureBonusXpTable() {
	std::string id = "CREATURE_BONUS_XP_TABLE";
	CreatureBonusXpTable& table = factory().get<CreatureBonusXpTable>(id);

	NumberMatcherFactory number_matcher{};
	table.addRow(number_matcher.matcher(1, 2), TableRow<int>().addCell(0).addCell(50).addCell(75).addCell(100).addCell(200).addCell(400).addCell(800).addCell(1200).addCell(1600).addCell(2000).addCell(3000).addCell(4000).addCell(5000));
	table.addRow(number_matcher.matcher(3, 4), TableRow<int>().addCell(0).addCell(40).addCell(60).addCell(95).addCell(190).addCell(380).addCell(760).addCell(1140).addCell(1520).addCell(1900).addCell(2850).addCell(3800).addCell(4750));
	table.addRow(number_matcher.matcher(5, 6), TableRow<int>().addCell(0).addCell(30).addCell(50).addCell(90).addCell(180).addCell(360).addCell(720).addCell(1080).addCell(1440).addCell(1800).addCell(2700).addCell(3600).addCell(4500));
	table.addRow(number_matcher.matcher(7, 8), TableRow<int>().addCell(0).addCell(20).addCell(40).addCell(85).addCell(170).addCell(340).addCell(680).addCell(1020).addCell(1360).addCell(1700).addCell(2550).addCell(3400).addCell(4250));
	table.addRow(number_matcher.matcher(9, 10), TableRow<int>().addCell(0).addCell(10).addCell(30).addCell(80).addCell(160).addCell(320).addCell(640).addCell(960).addCell(1280).addCell(1600).addCell(2400).addCell(3200).addCell(4000));
	table.addRow(number_matcher.matcher(11, 12), TableRow<int>().addCell(0).addCell(0).addCell(20).addCell(75).addCell(150).addCell(300).addCell(600).addCell(900).addCell(1200).addCell(1500).addCell(2250).addCell(3000).addCell(3750));
	table.addRow(number_matcher.matcher(13, 14), TableRow<int>().addCell(0).addCell(0).addCell(10).addCell(70).addCell(140).addCell(280).addCell(560).addCell(840).addCell(1120).addCell(1400).addCell(2100).addCell(2800).addCell(3500));
	table.addRow(number_matcher.matcher(15, 16), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(65).addCell(130).addCell(260).addCell(520).addCell(780).addCell(1040).addCell(1300).addCell(1950).addCell(2600).addCell(3250));
	table.addRow(number_matcher.matcher(17, 18), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(60).addCell(120).addCell(240).addCell(480).addCell(720).addCell(960).addCell(1200).addCell(1800).addCell(2400).addCell(3000));
	table.addRow(number_matcher.matcher(19, 20), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(55).addCell(110).addCell(220).addCell(440).addCell(660).addCell(880).addCell(1100).addCell(1650).addCell(2200).addCell(2750));
	table.addRow(number_matcher.matcher(20, 1000), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(50).addCell(100).addCell(200).addCell(400).addCell(600).addCell(800).addCell(1000).addCell(1500).addCell(2000).addCell(2500));
}

void AnimalDatafileParserJson::buildLevelVarianceTable() {
	std::string id = "CREATURE_LEVEL_VARIANCE_TABLE";
	LevelVarianceTable& table = factory().get<LevelVarianceTable>(id);
	NumberMatcherFactory number_matcher{};
	table.addRow(number_matcher.matcher(-100, 0), TableRow<int>().addCell(0).addCell(-1).addCell(-2).addCell(-3).addCell(-4).addCell(-5).addCell(-6).addCell(-10).addCell(-3));
	table.addRow(number_matcher.matcher(1, 10), TableRow<int>().addCell(0).addCell(-1).addCell(-2).addCell(-3).addCell(-4).addCell(-5).addCell(-6).addCell(-10).addCell(-2));
	table.addRow(number_matcher.matcher(11, 15), TableRow<int>().addCell(0).addCell(0).addCell(-1).addCell(-2).addCell(-3).addCell(-4).addCell(-5).addCell(-8).addCell(-2));
	table.addRow(number_matcher.matcher(16, 20), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(-1).addCell(-2).addCell(-3).addCell(-4).addCell(-6).addCell(-1));
	table.addRow(number_matcher.matcher(21, 25), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(-1).addCell(-2).addCell(-3).addCell(-4).addCell(-1));
	table.addRow(number_matcher.matcher(26, 35), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(-1).addCell(-2).addCell(-2).addCell(-1));
	table.addRow(number_matcher.matcher(36, 45), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(-1).addCell(-1).addCell(0));
	table.addRow(number_matcher.matcher(46, 55), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(0));
	table.addRow(number_matcher.matcher(56, 65), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(1).addCell(1).addCell(0));
	table.addRow(number_matcher.matcher(66, 75), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(0).addCell(1).addCell(2).addCell(2).addCell(1));
	table.addRow(number_matcher.matcher(76, 80), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(0).addCell(1).addCell(2).addCell(3).addCell(4).addCell(1));
	table.addRow(number_matcher.matcher(81, 85), TableRow<int>().addCell(0).addCell(0).addCell(0).addCell(1).addCell(2).addCell(3).addCell(4).addCell(6).addCell(1));
	table.addRow(number_matcher.matcher(86, 90), TableRow<int>().addCell(0).addCell(0).addCell(1).addCell(1).addCell(3).addCell(4).addCell(5).addCell(8).addCell(2));
	table.addRow(number_matcher.matcher(91, 100), TableRow<int>().addCell(0).addCell(1).addCell(1).addCell(2).addCell(4).addCell(5).addCell(6).addCell(10).addCell(2));
	table.addRow(number_matcher.matcher(101, 140), TableRow<int>().addCell(0).addCell(1).addCell(1).addCell(2).addCell(5).addCell(6).addCell(7).addCell(11).addCell(3));
	table.addRow(number_matcher.matcher(141, 170), TableRow<int>().addCell(0).addCell(1).addCell(2).addCell(3).addCell(6).addCell(7).addCell(8).addCell(12).addCell(3));
	table.addRow(number_matcher.matcher(171, 190), TableRow<int>().addCell(0).addCell(1).addCell(2).addCell(4).addCell(7).addCell(8).addCell(9).addCell(13).addCell(3));
	table.addRow(number_matcher.matcher(191, 200), TableRow<int>().addCell(0).addCell(2).addCell(3).addCell(5).addCell(8).addCell(9).addCell(10).addCell(14).addCell(3));
	table.addRow(number_matcher.matcher(201, 250), TableRow<int>().addCell(0).addCell(2).addCell(4).addCell(6).addCell(9).addCell(10).addCell(11).addCell(15).addCell(3));
	table.addRow(number_matcher.matcher(251, 300), TableRow<int>().addCell(0).addCell(3).addCell(5).addCell(7).addCell(10).addCell(11).addCell(12).addCell(16).addCell(4));
	table.addRow(number_matcher.matcher(301, 350), TableRow<int>().addCell(0).addCell(4).addCell(6).addCell(8).addCell(11).addCell(12).addCell(13).addCell(17).addCell(4));
}