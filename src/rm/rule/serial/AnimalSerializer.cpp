#include <AnimalSerializer.h>
#include <JsonConverter.h>
#include <NumberMatcherFactory.h>

namespace rm::rule::serial {

json::value AnimalSerializer::serializeObject(const AnimalData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	if (!ref.description().empty()) {
		JsonConverter::setString(obj, "description", ref.description());
	}
	JsonConverter::setInt(obj, "base-hits", ref.baseHits());
	JsonConverter::setInt(obj, "base-movement", ref.baseMovement());
	JsonConverter::setInt(obj, "defensive-bonus", ref.defensiveBonus());
	JsonConverter::setInt(obj, "frequency-code", ref.frequencyFactor());
	if (ref.bonusXpCode() != CreatureBonusXpType::Type::kNone)
		JsonConverter::setString(obj, "bonus-xp-code", CreatureBonusXpType::toString(ref.bonusXpCode()));
	if (ref.constitutionVarianceType() != CreatureConstitutionVarianceType::Type::kNone)
		JsonConverter::setString(obj, "constitution-variance-type", CreatureConstitutionVarianceType::toString(ref.constitutionVarianceType()));
	JsonConverter::setString(obj, "level-variance-type", LevelVarianceType::toString(ref.levelVarianceType()));
	JsonConverter::setInt(obj, "average-level", ref.averageLevel());
	if (ref.treasureCode())
		JsonConverter::setString(obj, "treasure-code", (ref.treasureCode().value()->id()));
	JsonConverter::setString(obj, "size", CreatureSizeType::toString(ref.size()));
	JsonConverter::setString(obj, "armour-type", ArmourType::toString(ref.armourType()));
	JsonConverter::setString(obj, "movement-speed", CreatureMovementSpeedType::toString(ref.movementSpeed()));
	JsonConverter::setString(obj, "attack-quickness", CreatureMovementSpeedType::toString(ref.attackQuickness()));
	JsonConverter::setString(obj, "max-pace", ref.maxPace()->id());
	JsonConverter::setString(obj, "outlook", AnimalOutlookType::toString(ref.outlook()));
	JsonConverter::setString(obj, "critical-table", CriticalSizeTableType::toString(ref.criticalTableType()));
	if (ref.carryCapacity() != 0)
		JsonConverter::setInt(obj, "carry-capacity", ref.carryCapacity());
	if (ref.ridingBonus() != 0)
		JsonConverter::setInt(obj, "riding-bonus", ref.ridingBonus());
	if (ref.criticalModifiers().size() > 0)
		JsonConverter::setEnumArray(obj, "critical-modifiers", ref.criticalModifiers());

	std::pair<int, int> encounter_range = ref.encounterRange();
	if (encounter_range.first != 0 || encounter_range.second != 0) {
		JsonConverter::createNested(obj).beginObject("encounter-range").setInt("min", encounter_range.first).setInt("max", encounter_range.second).endObject();
	}

	std::pair<int, int> number_young_range = ref.numberYoungRange();
	if (number_young_range.first != 0 || number_young_range.second != 0) {
		JsonConverter::createNested(obj).beginObject("number-young-range").setInt("min", number_young_range.first).setInt("max", number_young_range.second).endObject();
	}

	// Location data
	{
		rm::game::Location location = ref.location();
		JsonConverter::NestedBuilder location_builder = JsonConverter::createNested(obj).beginObject("location");
		if (location.features().size()) {
			std::set<std::string> features_str{};
			for (const auto& feature : location.features()) {
				features_str.emplace(EnvironmentType::toString(feature));
			}
			if (features_str.size())
				location_builder.setStringArray("features", features_str);
		}
		if (location.terrains().size()) {
			std::set<std::string> terrains_str{};
			for (const auto& terrain : location.terrains()) {
				terrains_str.emplace(EnvironmentType::toString(terrain));
			}
			if (terrains_str.size())
				location_builder.setStringArray("terrains", terrains_str);
		}
		if (location.vegetation().size()) {
			std::set<std::string> vegetation_str{};
			for (const auto& vegetation : location.vegetation()) {
				vegetation_str.emplace(EnvironmentType::toString(vegetation));
			}
			if (vegetation_str.size())
				location_builder.setStringArray("vegetation", vegetation_str);
		}
		if (location.water().size()) {
			std::set<std::string> water_str{};
			for (const auto& water : location.water()) {
				water_str.emplace(EnvironmentType::toString(water));
			}
			if (water_str.size())
				location_builder.setStringArray("water-sources", water_str);
		}
		if (location.climates().size()) {
			std::set<std::string> climates_str{};
			for (const auto& climate : location.climates()) {
				climates_str.emplace(climate->id());
			}
			if (climates_str.size())
				location_builder.setStringArray("climates", climates_str);
		}
		location_builder.endObject();
	}

	// Standard attacks
	if (ref.attacks().size()) {
		// Get the map of attacks from the game data
		std::map<const archid::NumberRange<int>*, rm::game::AnimalAttack> attacks = ref.attacks();

		// Start but sorting the attacks by their number range pointer value so that they are output in a consistent order in the json file.
		// We use a map to do this and store the pointer value as the key and the pointer itself as the value so that we can access the attack data when
		// populating the boost ptree for each attack.
		std::map<const archid::NumberRange<int>, const archid::NumberRange<int>*> ordered_attacks{};
		for (auto& attack : ref.attacks()) {
			for (auto& attack : ref.attacks()) {
				ordered_attacks.insert(std::make_pair(*attack.first, attack.first));
			}
		}

		JsonConverter::NestedBuilder std_attacks = JsonConverter::createNested(obj).beginArray("standard-attacks");
		for (auto& attack : ordered_attacks) {
			const archid::NumberRange<int>* number_range = attack.second;
			const rm::game::AnimalAttack& attack_data = attacks[number_range];
			JsonConverter::NestedBuilder attack_builder = std_attacks.addObject();
			serializeAnimalAttack(attack_builder, attack_data);
			attack_builder.endObject();
		}
		std_attacks.endArray();
	}

	// Group attacks
	{
		if (ref.groupAttacks().size()) {
			JsonConverter::NestedBuilder group_attacks_builder = JsonConverter::createNested(obj).beginArray("group-attacks");
			for (const auto& group_attack : ref.groupAttacks()) {
				JsonConverter::NestedBuilder group_attack_builder = group_attacks_builder.addObject();
				group_attack_builder.setInt("min-group-size", group_attack.first);
				serializeAnimalAttack(group_attack_builder, group_attack.second);
				group_attack_builder.endObject();
			}
			group_attacks_builder.endArray();
		}
	}

	// Ranged attacks
	{
		if (ref.rangedAttacks().size()) {
			JsonConverter::NestedBuilder ranged_attacks_builder = JsonConverter::createNested(obj).beginArray("ranged-attacks");
			for (const auto& ranged_attack : ref.rangedAttacks()) {
				JsonConverter::NestedBuilder ranged_attack_builder = ranged_attacks_builder.addObject();
				serializeAnimalAttack(ranged_attack_builder, ranged_attack);
				ranged_attack_builder.endObject();
			}
			ranged_attacks_builder.endArray();
		}
	}

	// Conditional attacks
	{
		if (ref.conditionalAttacks().size()) {
			JsonConverter::NestedBuilder conditional_attacks_builder = JsonConverter::createNested(obj).beginArray("conditional-attacks");
			for (const auto& conditional_attack : ref.conditionalAttacks()) {
				JsonConverter::NestedBuilder conditional_attack_builder = conditional_attacks_builder.addObject();
				serializeAnimalAttack(conditional_attack_builder, conditional_attack.second);
				conditional_attack_builder.endObject();
			}
			conditional_attacks_builder.endArray();
		}
	}

	return obj;
}

void AnimalSerializer::serializeAnimalAttack(JsonConverter::NestedBuilder& location_builder, const rm::game::AnimalAttack& attack) const {
	if (attack.chance())
		location_builder.setInt("chance-min", attack.chance().value()->min()).setInt("chance-max", attack.chance().value()->max());
	if (attack.conditionalAttackRef())
		location_builder.setInt("id", attack.conditionalAttackRef().value());
	if (attack.minGroupSize() > 1)
		location_builder.setInt("min-group-size", attack.minGroupSize());
	if (attack.range())
		location_builder.setInt("range", attack.range());
	if (attack.hasWeaponAttack() || attack.hasNonWeaponAttack())
		location_builder.setInt("offensive-bonus", attack.offensiveBonus());
	if (attack.hasWeaponAttack())
		location_builder.setString("weapon-attack", attack.weaponTable()->id());
	if (attack.hasNonWeaponAttack())
		location_builder.beginObject("non-weapon-attack").setString("table", attack.nonWeaponTable()->id()).setString("size", AttackSizeType::toString(attack.nonWeaponSize())).endObject();
	if (attack.useAllAttacks())
		location_builder.setBool("use-all-attacks", attack.useAllAttacks());
	if (attack.numAttacks() > 1)
		location_builder.setInt("attacks-per-round", attack.numAttacks());
	if (attack.special())
		location_builder.setString("special", attack.special().value());

	if (attack.poison())
		location_builder.setString("poison", attack.poison().value()->id());
	if (attack.disease())
		location_builder.setString("disease", attack.disease().value()->id());
	if (attack.autoCriticalType())
		location_builder.setString("auto-critical-type", CriticalType::toString(attack.autoCriticalType().value()));
	if (attack.autoCriticalSize())
		location_builder.setString("auto-critical-size", attack.autoCriticalSize().value());

	if (attack.sameRoundAttackId())
		location_builder.setInt("same-round-conditional-attack-id", attack.sameRoundAttackId());
	if (attack.nextRoundAttackId())
		location_builder.setInt("next-round-conditional-attack-id", attack.nextRoundAttackId());
}

const AnimalData& AnimalSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	AnimalData& ref = manager_.get<AnimalData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setBaseHits(JsonConverter::getInt(jsonObj, "base-hits"));
	ref.setBaseMovement(JsonConverter::getInt(jsonObj, "base-movement"));
	ref.setDefensiveBonus(JsonConverter::getInt(jsonObj, "defensive-bonus"));
	ref.setFrequencyFactor(JsonConverter::getInt(jsonObj, "frequency-code"));
	if (jsonObj.contains("carry-capacity"))
		ref.setCarryCapacity(JsonConverter::getInt(jsonObj, "carry-capacity"));
	if (jsonObj.contains("riding-bonus"))
		ref.setRidingBonus(JsonConverter::getInt(jsonObj, "riding-bonus"));

	if (jsonObj.contains("bonus-xp-code")) {
		std::string bonus_xp_code_str = JsonConverter::getString(jsonObj, "bonus-xp-code");
		ref.setBonusXpCode(CreatureBonusXpType::fromString(bonus_xp_code_str).value_or(CreatureBonusXpType::Type::kNone));
	}

	if (jsonObj.contains("constitution-variance-type")) {
		std::string constitution_variance_type_str = JsonConverter::getString(jsonObj, "constitution-variance-type");
		ref.setConstitutionVarianceType(CreatureConstitutionVarianceType::fromString(constitution_variance_type_str).value_or(CreatureConstitutionVarianceType::Type::kNone));
	}

	std::string level_variance_type_str = JsonConverter::getString(jsonObj, "level-variance-type");
	ref.setLevelVarianceType(LevelVarianceType::fromString(level_variance_type_str).value_or(LevelVarianceType::Type::kNone));
	ref.setAverageLevel(JsonConverter::getInt(jsonObj, "average-level"));

	if (jsonObj.contains("treasure-code")) {
		std::string treasure_code_id = JsonConverter::getString(jsonObj, "treasure-code");
		ref.setTreasureCode(manager_.get<TreasureCodeData>(treasure_code_id));
	}

	std::string size_str = JsonConverter::getString(jsonObj, "size");
	ref.setSize(CreatureSizeType::fromString(size_str).value());

	std::string armour_type_str = JsonConverter::getString(jsonObj, "armour-type");
	ref.setArmourType(ArmourType::fromString(armour_type_str).value());

	std::string movement_speed_str = JsonConverter::getString(jsonObj, "movement-speed");
	ref.setMovementSpeed(CreatureMovementSpeedType::fromString(movement_speed_str).value());

	std::string attack_quickness_str = JsonConverter::getString(jsonObj, "attack-quickness");
	ref.setAttackQuickness(CreatureMovementSpeedType::fromString(attack_quickness_str).value());

	std::string max_pace_id = JsonConverter::getString(jsonObj, "max-pace");
	ref.setMaxPace(manager_.get<CreaturePaceData>(max_pace_id));

	std::string outlook_str = JsonConverter::getString(jsonObj, "outlook");
	ref.setOutlook(AnimalOutlookType::fromString(outlook_str).value());

	std::string critical_table_type_str = JsonConverter::getString(jsonObj, "critical-table");
	ref.setCriticalTableType(CriticalSizeTableType::fromString(critical_table_type_str).value());

	if (jsonObj.contains("critical-modifiers"))
		ref.setCriticalModifiers(JsonConverter::getEnumArray<CriticalModifierType::Type>(jsonObj, "critical-modifiers"));

	{
		int min = JsonConverter::getNestedInt(jsonObj, "encounter-range/min", 0);
		int max = JsonConverter::getNestedInt(jsonObj, "encounter-range/max", 0);
		if (min != 0 || max != 0) {
			ref.setEncounterRange(std::make_pair(min, max));
		}
	}

	{
		int min = JsonConverter::getNestedInt(jsonObj, "number-young-range/min", 0);
		int max = JsonConverter::getNestedInt(jsonObj, "number-young-range/max", 0);
		if (min != 0 || max != 0) {
			ref.setNumberYoungRange(std::make_pair(min, max));
		}
	}

	{
		rm::game::Location location{};
		std::set<std::string> features_str = JsonConverter::getNestedStringArray(jsonObj, "location/features");
		for (const auto& feature_str : features_str) {
			EnvironmentType::Feature feature{};
			fromString(feature_str, feature);
			location.addFeature(feature);
		}
		std::set<std::string> terrains_str = JsonConverter::getNestedStringArray(jsonObj, "location/terrains");
		for (const auto& terrain_str : terrains_str) {
			EnvironmentType::Terrain terrain{};
			fromString(terrain_str, terrain);
			location.addTerrain(terrain);
		}
		std::set<std::string> vegetation_str = JsonConverter::getNestedStringArray(jsonObj, "location/vegetation");
		for (const auto& vegetation_str : vegetation_str) {
			EnvironmentType::Vegetation vegetation{};
			fromString(vegetation_str, vegetation);
			location.addVegetation(vegetation);
		}
		std::set<std::string> water_str = JsonConverter::getNestedStringArray(jsonObj, "location/water-sources");
		for (const auto& water_str : water_str) {
			EnvironmentType::Water water{};
			fromString(water_str, water);
			location.addWater(water);
		}
		std::set<std::string> climates_str = JsonConverter::getNestedStringArray(jsonObj, "location/climates");
		for (const auto& climate_str : climates_str) {
			location.addClimate(&manager_.get<ClimateData>(climate_str));
		}
		ref.setLocation(location);
	}

	NumberMatcherFactory number_matcher{};

	// Standard attacks have a chance range
	if (jsonObj.contains("standard-attacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "standard-attacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, true);
			const archid::NumberRange<int>* range = number_matcher.matcher(chance.first, chance.second);
			ref.addAttack(range, attack);
		}
	}

	// Ranged attacks
	if (jsonObj.contains("ranged-attacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "ranged-attacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, false);
			ref.addRangedAttack(std::move(attack));
		}
	}

	// Conditional attacks
	if (jsonObj.contains("conditional-attacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "conditional-attacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, true);
			int ref_id = attack.conditionalAttackRef().value();
			ref.addConditionalAttack(ref_id, std::move(attack));
		}
	}

	// Group attacks
	if (jsonObj.contains("group-attacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "group-attacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, true);
			int min_group_size = attack.minGroupSize();
			ref.addGroupAttack(min_group_size, std::move(attack));
		}
	}

	return ref;
}

void AnimalSerializer::deserializeAnimalAttack(json::object& jsonObj, rm::game::AnimalAttack& attack, std::pair<int, int>& chance, bool use_chance) const {
	if (jsonObj.contains("chance-min") && jsonObj.contains("chance-min")) {
		chance.first = JsonConverter::getInt(jsonObj, "chance-min");
		chance.second = JsonConverter::getInt(jsonObj, "chance-max");
		if (use_chance) {
			NumberMatcherFactory number_matcher{};
			attack.setChance(number_matcher.matcher(chance.first, chance.second));
		}
	}
	if (jsonObj.contains("id"))
		attack.setConditionalAttackRef(JsonConverter::getInt(jsonObj, "id"));
	if (jsonObj.contains("min-group-size"))
		attack.setMinGroupSize(JsonConverter::getInt(jsonObj, "min-group-size"));
	if (jsonObj.contains("range"))
		attack.setRange(JsonConverter::getInt(jsonObj, "range"));
	if (jsonObj.contains("offensive-bonus"))
		attack.setOffensiveBonus(JsonConverter::getInt(jsonObj, "offensive-bonus"));
	if (jsonObj.contains("weapon-attack")) {
		std::string weapon_attack_id = JsonConverter::getString(jsonObj, "weapon-attack");
		attack.setWeaponTable(manager_.get<AttackTable>(weapon_attack_id));
	}
	if (jsonObj.contains("non-weapon-attack")) {
		std::string table_id = JsonConverter::getNestedString(jsonObj, "non-weapon-attack/table");
		attack.setNonWeaponTable(manager_.get<SpecialAttackTable>(table_id));
		std::string size_str = JsonConverter::getNestedString(jsonObj, "non-weapon-attack/size");
		AttackSizeType::Type size{};
		AttackSizeType::fromString(size_str, size);
		attack.setNonWeaponSize(size);
	}
	if (jsonObj.contains("use-all-attacks"))
		attack.setUseAllAttacks(JsonConverter::getBool(jsonObj, "use-all-attacks"));
	if (jsonObj.contains("attacks-per-round"))
		attack.setNumAttacks(JsonConverter::getInt(jsonObj, "attacks-per-round"));
	if (jsonObj.contains("special"))
		attack.setSpecial(JsonConverter::getString(jsonObj, "special"));
	if (jsonObj.contains("poison")) {
		std::string poison_id = JsonConverter::getString(jsonObj, "poison");
		attack.setPoison(manager_.get<PoisonData>(poison_id));
	}
	if (jsonObj.contains("disease")) {
		std::string disease_id = JsonConverter::getString(jsonObj, "disease");
		attack.setDisease(manager_.get<DiseaseData>(disease_id));
	}
	if (jsonObj.contains("auto-critical-type")) {
		std::string auto_critical_type_str = JsonConverter::getString(jsonObj, "auto-critical-type");
		CriticalType::Type auto_critical_type{};
		CriticalType::fromString(auto_critical_type_str, auto_critical_type);
		attack.setAutoCriticalType(auto_critical_type);
	}
	if (jsonObj.contains("auto-critical-size"))
		attack.setAutoCriticalSize(JsonConverter::getString(jsonObj, "auto-critical-size"));
	if (jsonObj.contains("same-round-conditional-attack-id"))
		attack.setSameRoundAttackId(JsonConverter::getInt(jsonObj, "same-round-conditional-attack-id"));
	if (jsonObj.contains("next-round-conditional-attack-id"))
		attack.setNextRoundAttackId(JsonConverter::getInt(jsonObj, "next-round-conditional-attack-id"));
}

} // namespace rm::rule::serial