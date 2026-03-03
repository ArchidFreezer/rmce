#include <AnimalSerializer.h>
#include <GameRuleBoostConverter.h>
#include <NumberMatcherFactory.h>

namespace rm::rule::parser {

void AnimalSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading Animal data ..." << std::endl;

	NumberMatcherFactory number_matcher{};

	// Get the animals to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		AnimalData& ref = manager().get<AnimalData>(id);
		ref.setName(name);

		// Description is optional
		std::string description = v.second.get<std::string>("description", "");
		if (!description.empty())
			ref.setDescription(description);
		ref.setBaseHits(v.second.get<int>("base-hits"));
		ref.setBaseMovement(v.second.get<int>("base-movement"));
		ref.setDefensiveBonus(v.second.get<int>("defensive-bonus"));
		ref.setFrequencyFactor(v.second.get<int>("frequency-code"));

		boost::optional<int> carry_capacity = v.second.get_optional<int>("carry_capacity");
		if (carry_capacity) {
			ref.setCarryCapacity(carry_capacity.get());
		}

		boost::optional<int> riding_bonus = v.second.get_optional<int>("riding-bonus");
		if (riding_bonus) {
			ref.setRidingBonus(riding_bonus.get());
		}

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
		if (treasure_code_id)
			ref.setTreasureCode(manager().get<TreasureCodeData>(treasure_code_id.value()));

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

		std::string max_pace_id{v.second.get<std::string>("max-pace")};
		ref.setMaxPace(manager().get<CreaturePaceData>(max_pace_id));

		AnimalOutlookType::Type outlook_code{};
		AnimalOutlookType::fromString(v.second.get<std::string>("outlook"), outlook_code);
		ref.setOutlook(outlook_code);

		CriticalSizeTableType::Type critical_table_code{};
		CriticalSizeTableType::fromString(v.second.get<std::string>("critical-table"), critical_table_code);
		ref.setCriticalTableType(critical_table_code);

		GameRuleBoostConverter converter{manager()};

		// Critical modifiers are optional
		// TODO: The data in the legacy XML file is incorrect and the json file needs to be manually updated.
		boost::optional<const pt::ptree&> critical_modifiers = v.second.get_child_optional("critical_modifiers");
		if (critical_modifiers)
			ref.setCriticalModifiers(converter.parseEnumSetTree<CriticalModifierType::Type>(critical_modifiers));

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
				rm::game::Location location{};
				boost::optional<const pt::ptree&> features_tree = location_tree->get_child_optional("features");
				if (features_tree)
					location.setFeatures(converter.parseEnumSetTree<EnvironmentType::Feature>(features_tree));
				boost::optional<const pt::ptree&> terrains_tree = location_tree->get_child_optional("terrains");
				if (terrains_tree)
					location.setTerrains(converter.parseEnumSetTree<EnvironmentType::Terrain>(terrains_tree));
				boost::optional<const pt::ptree&> vegetation_tree = location_tree->get_child_optional("vegetation");
				if (vegetation_tree)
					location.setVegetation(converter.parseEnumSetTree<EnvironmentType::Vegetation>(vegetation_tree));
				boost::optional<const pt::ptree&> water_sources_tree = location_tree->get_child_optional("water-sources");
				if (water_sources_tree)
					location.setWater(converter.parseEnumSetTree<EnvironmentType::Water>(water_sources_tree));
				boost::optional<const pt::ptree&> climates_tree = location_tree->get_child_optional("climates");
				if (climates_tree)
					location.setClimates(converter.parseGameDataSetTree<ClimateData>(climates_tree));
				ref.setLocation(location);
			}
		}

		// Standard attacks
		{
			boost::optional<const pt::ptree&> tree_opt = v.second.get_child_optional("standard-attacks");
			if (tree_opt) {
				for (const auto& attack_tree : tree_opt.value()) {
					const pt::ptree& tree = attack_tree.second;
					const archid::NumberRange<int>* range = number_matcher.matcher(tree.get<int>("chance-min"), tree.get<int>("chance-max"));
					rm::game::AnimalAttack attack{};
					parseAnimalAttack(attack, tree);
					ref.addAttack(range, attack);
				}
			}
		}

		// Ranged attacks
		{
			boost::optional<const pt::ptree&> tree_opt = v.second.get_child_optional("ranged-attacks");
			if (tree_opt) {
				for (const auto& attack_tree : tree_opt.value()) {
					const pt::ptree& tree = attack_tree.second;
					rm::game::AnimalAttack attack{};
					parseAnimalAttack(attack, tree, false);
					ref.addRangedAttack(attack);
				}
			}
		}

		// Conditional Attacks
		{
			boost::optional<const pt::ptree&> tree_opt = v.second.get_child_optional("conditional-attacks");
			if (tree_opt) {
				for (const auto& attack_tree : tree_opt.value()) {
					const pt::ptree& tree = attack_tree.second;
					rm::game::AnimalAttack attack{};
					parseAnimalAttack(attack, tree);
					ref.addConditionalAttack(attack.conditionalAttackRef().value(), attack);
				}
			}
		}

		// Group Attacks
		{
			boost::optional<const pt::ptree&> tree_opt = v.second.get_child_optional("group-attacks");
			if (tree_opt) {
				for (const auto& attack_tree : tree_opt.value()) {
					const pt::ptree& tree = attack_tree.second;
					rm::game::AnimalAttack attack{};
					parseAnimalAttack(attack, tree, false);
					ref.addGroupAttack(attack.minGroupSize(), attack);
				}
			}
		}

		std::cout << "\tAnimal name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void AnimalSerializer::populateDatum(std::string key, pt::ptree& datum) {
	AnimalData& game_data = manager().get<AnimalData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	if (!game_data.description().empty())
		datum.put("description", game_data.description());
	datum.put("base-hits", game_data.baseHits());
	datum.put("base-movement", game_data.baseMovement());
	datum.put("defensive-bonus", game_data.defensiveBonus());
	datum.put("frequency-code", game_data.frequencyFactor());
	if (game_data.bonusXpCode() != CreatureBonusXpType::Type::kNone)
		datum.put("bonus-xp-code", CreatureBonusXpType::toString(game_data.bonusXpCode()));
	if (game_data.constitutionVarianceType() != CreatureConstitutionVarianceType::Type::kNone)
		datum.put("constitution-variance-type", CreatureConstitutionVarianceType::toString(game_data.constitutionVarianceType()));
	datum.put("level-variance-type", LevelVarianceType::toString(game_data.levelVarianceType()));
	datum.put("average-level", game_data.averageLevel());
	if (game_data.treasureCode())
		datum.put("treasure-code", game_data.treasureCode().value()->id());
	datum.put("size", CreatureSizeType::toString(game_data.size()));
	datum.put("armour-type", ArmourType::toString(game_data.armourType()));
	datum.put("movement-speed", CreatureMovementSpeedType::toString(game_data.movementSpeed()));
	datum.put("attack-quickness", CreatureMovementSpeedType::toString(game_data.attackQuickness()));
	datum.put("max-pace", game_data.maxPace()->id());
	datum.put("outlook", AnimalOutlookType::toString(game_data.outlook()));
	datum.put("critical-table", CriticalSizeTableType::toString(game_data.criticalTableType()));

	if (game_data.carryCapacity())
		datum.put("carry_capacity", game_data.carryCapacity());
	if (game_data.ridingBonus())
		datum.put("riding-bonus", game_data.ridingBonus());

	GameRuleBoostConverter converter{manager()};

	// Critical modifiers are optional, only add to the tree if there are any
	{
		pt::ptree tree{converter.getEnumSetTree<CriticalModifierType::Type>(game_data.criticalModifiers())};
		if (tree.size())
			datum.push_back(std::make_pair("critical_modifiers", tree));
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
		rm::game::Location location = game_data.location();

		// required features
		{
			pt::ptree tree{converter.getEnumSetTree<EnvironmentType::Feature>(location.features())};
			if (tree.size())
				location_tree.push_back(std::make_pair("features", tree));
		}
		// required terrains
		{
			pt::ptree tree{converter.getEnumSetTree<EnvironmentType::Terrain>(location.terrains())};
			if (tree.size())
				location_tree.push_back(std::make_pair("terrains", tree));
		}
		// required vegetation
		{
			pt::ptree tree{converter.getEnumSetTree<EnvironmentType::Vegetation>(location.vegetation())};
			if (tree.size())
				location_tree.push_back(std::make_pair("vegetation", tree));
		}
		// required water sources
		{
			pt::ptree tree{converter.getEnumSetTree<EnvironmentType::Water>(location.water())};
			if (tree.size())
				location_tree.push_back(std::make_pair("water-sources", tree));
		}
		// required climates
		{
			pt::ptree tree{converter.getGameDataSetTree<ClimateData>(location.climates())};
			if (tree.size())
				location_tree.push_back(std::make_pair("climates", tree));
		}
		if (location_tree.size())
			datum.push_back(std::make_pair("location", location_tree));
	}

	// Standard Attacks
	{
		// Get the map of attacks from the game data
		std::map<const archid::NumberRange<int>*, rm::game::AnimalAttack> attacks = game_data.attacks();

		// Start but sorting the attacks by their number range pointer value so that they are output in a consistent order in the json file.
		// We use a map to do this and store the pointer value as the key and the pointer itself as the value so that we can access the attack data when
		// populating the boost ptree for each attack.
		std::map<const archid::NumberRange<int>, const archid::NumberRange<int>*> ordered_attacks{};
		for (auto& attack : game_data.attacks()) {
			for (auto& attack : game_data.attacks()) {
				ordered_attacks.insert(std::make_pair(*attack.first, attack.first));
			}
		}

		// Parse the attackdata and place into the boost ptree
		pt::ptree attacks_tree{};
		for (auto& attack : ordered_attacks) {
			pt::ptree attack_tree{};
			populateAnimalAttack(attack_tree, attacks[attack.second]);
			attacks_tree.push_back(std::make_pair("", attack_tree));
		}

		// Only add the attacks to the tree if there are any, otherwise we will end up with an empty attacks array in the json file which is not ideal.
		if (attacks_tree.size())
			datum.push_back(std::make_pair("standard-attacks", attacks_tree));
	}

	// Group Attacks
	{
		// Parse the attackdata and place into the boost ptree
		pt::ptree group_attacks_tree{};
		for (const auto& group_attack : game_data.groupAttacks()) {
			pt::ptree group_attack_tree{};
			populateAnimalAttack(group_attack_tree, group_attack.second);
			group_attacks_tree.push_back(std::make_pair("", group_attack_tree));
		}
		// Only add the group attacks to the tree if there are any, otherwise we will end up with an empty group attacks array in the json file which is not
		// ideal.
		if (group_attacks_tree.size())
			datum.push_back(std::make_pair("group-attacks", group_attacks_tree));
	}

	// Ranged Attacks
	{
		// Parse the attackdata and place into the boost ptree
		pt::ptree ranged_attacks_tree{};
		for (const auto& ranged_attack : game_data.rangedAttacks()) {
			pt::ptree ranged_attack_tree{};
			populateAnimalAttack(ranged_attack_tree, ranged_attack);
			ranged_attacks_tree.push_back(std::make_pair("", ranged_attack_tree));
		}
		// Only add the ranged attacks to the tree if there are any, otherwise we will end up with an empty ranged attacks array in the json file which is not
		// ideal.
		if (ranged_attacks_tree.size())
			datum.push_back(std::make_pair("ranged-attacks", ranged_attacks_tree));
	}

	// Conditional Attacks
	{
		// Parse the attackdata and place into the boost ptree
		pt::ptree conditional_attacks_tree{};
		for (const auto& conditional_attack : game_data.conditionalAttacks()) {
			pt::ptree conditional_attack_tree{};
			populateAnimalAttack(conditional_attack_tree, conditional_attack.second);
			conditional_attacks_tree.push_back(std::make_pair("", conditional_attack_tree));
		}
		// Only add the conditional attacks to the tree if there are any, otherwise we will end up with an empty conditional attacks array in the json file
		// which is not ideal.
		if (conditional_attacks_tree.size())
			datum.push_back(std::make_pair("conditional-attacks", conditional_attacks_tree));
	}
}

std::string AnimalSerializer::rootNode() const {
	return "animals";
}

// Write boost ptree data for an AnimalAttack into an AnimalAttack object
void AnimalSerializer::parseAnimalAttack(rm::game::AnimalAttack& attack, const pt::ptree& tree, bool parse_chance) {
	NumberMatcherFactory number_matcher{};

	if (parse_chance) {
		boost::optional<int> chance_min = tree.get_optional<int>("chance-min");
		boost::optional<int> chance_max = tree.get_optional<int>("chance-max");
		if (chance_max && chance_min)
			attack.setChance(number_matcher.matcher(chance_min.value(), chance_max.value()));
	}
	if (tree.get_optional<int>("id"))
		attack.setConditionalAttackRef(tree.get<int>("id"));
	if (tree.get_optional<int>("min-group-size"))
		attack.setMinGroupSize(tree.get<int>("min-group-size"));
	if (tree.get_optional<int>("range"))
		attack.setRange(tree.get<int>("range"));
	if (tree.get_optional<int>("offensive-bonus"))
		attack.setOffensiveBonus(tree.get<int>("offensive-bonus"));
	if (tree.get_optional<std::string>("weapon-attack")) {
		std::string weapon_attack_id = tree.get<std::string>("weapon-attack");
		attack.setWeaponTable(manager().get<AttackTable>(weapon_attack_id));
	}
	boost::optional<const pt::ptree&> non_weapon_attack_tree = tree.get_child_optional("non-weapon-attack");
	if (non_weapon_attack_tree) {
		std::string attack_table_id = non_weapon_attack_tree->get<std::string>("table");
		attack.setNonWeaponTable(manager().get<SpecialAttackTable>(attack_table_id));
		std::string size_str = non_weapon_attack_tree->get<std::string>("size");
		AttackSizeType::Type size{};
		AttackSizeType::fromString(size_str, size);
		attack.setNonWeaponSize(size);
	}
	if (tree.get_optional<bool>("use-all-attacks"))
		attack.setUseAllAttacks(tree.get<bool>("use-all-attacks"));
	if (tree.get_optional<int>("attacks-per-round"))
		attack.setNumAttacks(tree.get<int>("attacks-per-round"));
	if (tree.get_optional<std::string>("special"))
		attack.setSpecial(tree.get<std::string>("special"));
	if (tree.get_optional<std::string>("poison")) {
		std::string poison_id = tree.get<std::string>("poison");
		attack.setPoison(manager().get<PoisonData>(poison_id));
	}
	if (tree.get_optional<std::string>("disease")) {
		std::string disease_id = tree.get<std::string>("disease");
		attack.setDisease(manager().get<DiseaseData>(disease_id));
	}
	if (tree.get_optional<std::string>("auto-critical-type")) {
		std::string auto_critical_type_str = tree.get<std::string>("auto-critical-type");
		CriticalType::Type auto_critical_type{};
		CriticalType::fromString(auto_critical_type_str, auto_critical_type);
		attack.setAutoCriticalType(auto_critical_type);
	}
	if (tree.get_optional<std::string>("auto-critical-size"))
		attack.setAutoCriticalSize(tree.get<std::string>("auto-critical-size"));
	if (tree.get_optional<int>("same-round-conditional-attack-id"))
		attack.setSameRoundAttackId(tree.get<int>("same-round-conditional-attack-id"));
	if (tree.get_optional<int>("next-round-conditional-attack-id"))
		attack.setNextRoundAttackId(tree.get<int>("next-round-conditional-attack-id"));
}

// Write AnimalAttack to boost ptree
void AnimalSerializer::populateAnimalAttack(pt::ptree& tree, const rm::game::AnimalAttack& attack) {
	if (attack.chance()) {
		// Store a copy of the NumberRange, not a pointer to a temporary
		const archid::NumberRange<int> chance_range{*attack.chance().value()};
		tree.put("chance-min", chance_range.min());
		tree.put("chance-max", chance_range.max());
	}
	if (attack.conditionalAttackRef())
		tree.put("id", attack.conditionalAttackRef().value());
	if (attack.minGroupSize() > 1)
		tree.put("min-group-size", attack.minGroupSize());
	if (attack.range())
		tree.put("range", attack.range());
	if (attack.hasWeaponAttack() || attack.hasNonWeaponAttack())
		tree.put("offensive-bonus", attack.offensiveBonus());
	if (attack.hasWeaponAttack())
		tree.put("weapon-attack", attack.weaponTable()->id());
	if (attack.hasNonWeaponAttack()) {
		pt::ptree non_weapon_tree{};
		non_weapon_tree.put("table", attack.nonWeaponTable()->id());
		non_weapon_tree.put("size", AttackSizeType::toString(attack.nonWeaponSize()));
		tree.push_back(std::make_pair("non-weapon-attack", non_weapon_tree));
	}
	if (attack.useAllAttacks())
		tree.put("use-all-attacks", attack.useAllAttacks());
	if (attack.numAttacks() > 1)
		tree.put("attacks-per-round", attack.numAttacks());
	if (attack.special())
		tree.put("special", attack.special().value());

	if (attack.poison())
		tree.put("poison", attack.poison().value()->id());
	if (attack.disease())
		tree.put("disease", attack.disease().value()->id());
	if (attack.autoCriticalType())
		tree.put("auto-critical-type", CriticalType::toString(attack.autoCriticalType().value()));
	if (attack.autoCriticalSize())
		tree.put("auto-critical-size", attack.autoCriticalSize().value());

	if (attack.sameRoundAttackId())
		tree.put("same-round-conditional-attack-id", attack.sameRoundAttackId());
	if (attack.nextRoundAttackId())
		tree.put("next-round-conditional-attack-id", attack.nextRoundAttackId());
}

} // namespace rm::rule::parser