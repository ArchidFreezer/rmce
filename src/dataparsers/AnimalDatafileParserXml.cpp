#include <AnimalData.h>
#include <AnimalDatafileParserXml.h>
#include <NumberMatcherFactory.h>
#include <table/CreatureBonusXpTable.h>
#include <table/CreatureLevelVarianceTable.h>

void AnimalDatafileParserXml::parse() {
	std::cout << "Loading Animal data ...\n";

	buildCreatureBonusXpTable();
	buildCreatureLevelVarianceTable();

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		AnimalData& ref = factory().get<AnimalData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));
		ref.setBaseHits(v.second.get<int>("base-hits"));
		ref.setBaseMovement(v.second.get<int>("base-movement"));
		ref.setDefensiveBonus(v.second.get<int>("defensive-bonus"));
		ref.setFrequencyFactor(v.second.get<int>("frequency-code"));
		ref.setMovingManoeuvreBonus(v.second.get<int>("moving-manoeuvre-bonus"));

		CreatureBonusXpType::Type xp_code{};
		CreatureBonusXpType::fromString(v.second.get<std::string>("bonus-xp-code"), xp_code);
		ref.setBonusXpCode(xp_code);

		CreatureConstitutionVarianceType::Type constitution_code{};
		CreatureConstitutionVarianceType::fromString(v.second.get<std::string>("constitution-code"), constitution_code);
		ref.setConstitutionVarianceType(constitution_code);

		const auto& plevel = v.second.get_child("level-code");
		CreatureLevelVarianceType::Type level_code{};
		CreatureLevelVarianceType::fromString(plevel.get_value<std::string>(), level_code);
		ref.setLevelVarianceType(level_code);
		ref.setAverageLevel(plevel.get<int>("<xmlattr>.level"));

		std::string treasure_code_id = v.second.get<std::string>("treasure-code");
		if (treasure_code_id != "-") {
			ref.setTreasureCode(factory().get<TreasureCodeData>(treasure_code_id));
		}

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

		std::string max_pace_id{ GameRuleData::generateId("CreaturePace", v.second.get<std::string>("max-pace")) };
		ref.setMaxPace(factory().get<CreaturePaceData>(max_pace_id));

		AnimalOutlookType::Type outlook_code{};
		AnimalOutlookType::fromString(v.second.get<std::string>("outlook"), outlook_code);
		ref.setOutlook(outlook_code);

		CriticalSizeTableType::Type critical_table_code{};
		CriticalSizeTableType::fromString(v.second.get<std::string>("critical-table"), critical_table_code);
		ref.setCriticalTableType(critical_table_code);

		std::cout << "\tAnimal name: " << ref.name() << std::endl;

	}
}

void AnimalDatafileParserXml::buildCreatureBonusXpTable() {
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

void AnimalDatafileParserXml::buildCreatureLevelVarianceTable() {
	std::string id = "CREATURE_LEVEL_VARIANCE_TABLE";
	CreatureLevelVarianceTable& table = factory().get<CreatureLevelVarianceTable>(id);
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