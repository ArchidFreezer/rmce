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
	JsonConverter::setInt(obj, "baseHits", ref.baseHits());
	JsonConverter::setInt(obj, "baseMovement", ref.baseMovement());
	JsonConverter::setInt(obj, "defensiveBonus", ref.defensiveBonus());
	JsonConverter::setInt(obj, "frequencyCode", ref.frequencyFactor());
	if (ref.bonusXpCode() != CreatureBonusXpType::Type::kNone)
		JsonConverter::setString(obj, "bonusXpCode", CreatureBonusXpType::toString(ref.bonusXpCode()));
	if (ref.constitutionVarianceType() != CreatureConstitutionVarianceType::Type::kNone)
		JsonConverter::setString(obj, "constitutionVarianceType", CreatureConstitutionVarianceType::toString(ref.constitutionVarianceType()));
	JsonConverter::setString(obj, "levelVarianceType", LevelVarianceType::toString(ref.levelVarianceType()));
	JsonConverter::setInt(obj, "averageLevel", ref.averageLevel());
	if (ref.treasureCode())
		JsonConverter::setString(obj, "treasureCode", (ref.treasureCode().value()->id()));
	JsonConverter::setString(obj, "size", CreatureSizeType::toString(ref.size()));
	JsonConverter::setString(obj, "armourType", ArmourType::toString(ref.armourType()));
	JsonConverter::setString(obj, "movementSpeed", CreatureMovementSpeedType::toString(ref.movementSpeed()));
	JsonConverter::setString(obj, "attackQuickness", CreatureMovementSpeedType::toString(ref.attackQuickness()));
	JsonConverter::setString(obj, "maxPace", ref.maxPace()->id());
	JsonConverter::setString(obj, "outlook", AnimalOutlookType::toString(ref.outlook()));
	JsonConverter::setString(obj, "criticalTable", CriticalSizeTableType::toString(ref.criticalTableType()));
	if (ref.carryCapacity() != 0)
		JsonConverter::setInt(obj, "carryCapacity", ref.carryCapacity());
	if (ref.ridingBonus() != 0)
		JsonConverter::setInt(obj, "ridingBonus", ref.ridingBonus());
	if (ref.criticalModifiers().size() > 0)
		JsonConverter::setEnumSet(obj, "criticalModifiers", ref.criticalModifiers());

	std::pair<int, int> encounter_range = ref.encounterRange();
	if (encounter_range.first != 0 || encounter_range.second != 0) {
		JsonConverter::createNested(obj).beginObject("encounterRange").setInt("min", encounter_range.first).setInt("max", encounter_range.second).endObject();
	}

	std::pair<int, int> number_young_range = ref.numberYoungRange();
	if (number_young_range.first != 0 || number_young_range.second != 0) {
		JsonConverter::createNested(obj).beginObject("numberYoungRange").setInt("min", number_young_range.first).setInt("max", number_young_range.second).endObject();
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
				location_builder.setStringArray("waterSources", water_str);
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

		JsonConverter::NestedBuilder std_attacks = JsonConverter::createNested(obj).beginArray("standardAttacks");
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
			JsonConverter::NestedBuilder group_attacks_builder = JsonConverter::createNested(obj).beginArray("groupAttacks");
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
			JsonConverter::NestedBuilder ranged_attacks_builder = JsonConverter::createNested(obj).beginArray("rangedAttacks");
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
			JsonConverter::NestedBuilder conditional_attacks_builder = JsonConverter::createNested(obj).beginArray("conditionalAttacks");
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
		location_builder.setInt("chanceMin", attack.chance().value()->min()).setInt("chanceMax", attack.chance().value()->max());
	if (attack.conditionalAttackRef())
		location_builder.setInt("id", attack.conditionalAttackRef().value());
	if (attack.minGroupSize() > 1)
		location_builder.setInt("minGroupSize", attack.minGroupSize());
	if (attack.range())
		location_builder.setInt("range", attack.range());
	if (attack.hasWeaponAttack() || attack.hasNonWeaponAttack())
		location_builder.setInt("offensiveBonus", attack.offensiveBonus());
	if (attack.hasWeaponAttack())
		location_builder.setString("weaponAttack", attack.weaponTable()->id());
	if (attack.hasNonWeaponAttack())
		location_builder.beginObject("nonWeaponAttack").setString("table", attack.nonWeaponTable()->id()).setString("size", AttackSizeType::toString(attack.nonWeaponSize())).endObject();
	if (attack.useAllAttacks())
		location_builder.setBool("useAllAttacks", attack.useAllAttacks());
	if (attack.numAttacks() > 1)
		location_builder.setInt("attacksPerRound", attack.numAttacks());
	if (attack.special())
		location_builder.setString("special", attack.special().value());

	if (attack.poison())
		location_builder.setString("poison", attack.poison().value()->id());
	if (attack.disease())
		location_builder.setString("disease", attack.disease().value()->id());
	if (attack.autoCriticalType())
		location_builder.setString("autoCriticalType", CriticalType::toString(attack.autoCriticalType().value()));
	if (attack.autoCriticalSize())
		location_builder.setString("autoCriticalSize", attack.autoCriticalSize().value());

	if (attack.sameRoundAttackId())
		location_builder.setInt("sameRoundConditionalAttackId", attack.sameRoundAttackId());
	if (attack.nextRoundAttackId())
		location_builder.setInt("nextRoundConditionalAttackId", attack.nextRoundAttackId());
}

const AnimalData& AnimalSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	AnimalData& ref = manager_.get<AnimalData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setBaseHits(JsonConverter::getInt(jsonObj, "baseHits"));
	ref.setBaseMovement(JsonConverter::getInt(jsonObj, "baseMovement"));
	ref.setDefensiveBonus(JsonConverter::getInt(jsonObj, "defensiveBonus"));
	ref.setFrequencyFactor(JsonConverter::getInt(jsonObj, "frequencyCode"));
	if (jsonObj.contains("carryCapacity"))
		ref.setCarryCapacity(JsonConverter::getInt(jsonObj, "carryCapacity"));
	if (jsonObj.contains("ridingBonus"))
		ref.setRidingBonus(JsonConverter::getInt(jsonObj, "ridingBonus"));

	if (jsonObj.contains("bonusXpCode")) {
		std::string bonus_xp_code_str = JsonConverter::getString(jsonObj, "bonusXpCode");
		ref.setBonusXpCode(CreatureBonusXpType::fromString(bonus_xp_code_str).value_or(CreatureBonusXpType::Type::kNone));
	}

	if (jsonObj.contains("constitutionVarianceType")) {
		std::string constitution_variance_type_str = JsonConverter::getString(jsonObj, "constitutionVarianceType");
		ref.setConstitutionVarianceType(CreatureConstitutionVarianceType::fromString(constitution_variance_type_str).value_or(CreatureConstitutionVarianceType::Type::kNone));
	}

	std::string level_variance_type_str = JsonConverter::getString(jsonObj, "levelVarianceType");
	ref.setLevelVarianceType(LevelVarianceType::fromString(level_variance_type_str).value_or(LevelVarianceType::Type::kNone));
	ref.setAverageLevel(JsonConverter::getInt(jsonObj, "averageLevel"));

	if (jsonObj.contains("treasureCode")) {
		std::string treasure_code_id = JsonConverter::getString(jsonObj, "treasureCode");
		ref.setTreasureCode(manager_.get<TreasureCodeData>(treasure_code_id));
	}

	std::string size_str = JsonConverter::getString(jsonObj, "size");
	ref.setSize(CreatureSizeType::fromString(size_str).value());

	std::string armour_type_str = JsonConverter::getString(jsonObj, "armourType");
	ref.setArmourType(ArmourType::fromString(armour_type_str).value());

	std::string movement_speed_str = JsonConverter::getString(jsonObj, "movementSpeed");
	ref.setMovementSpeed(CreatureMovementSpeedType::fromString(movement_speed_str).value());

	std::string attack_quickness_str = JsonConverter::getString(jsonObj, "attackQuickness");
	ref.setAttackQuickness(CreatureMovementSpeedType::fromString(attack_quickness_str).value());

	std::string max_pace_id = JsonConverter::getString(jsonObj, "maxPace");
	ref.setMaxPace(manager_.get<CreaturePaceData>(max_pace_id));

	std::string outlook_str = JsonConverter::getString(jsonObj, "outlook");
	ref.setOutlook(AnimalOutlookType::fromString(outlook_str).value());

	std::string critical_table_type_str = JsonConverter::getString(jsonObj, "criticalTable");
	ref.setCriticalTableType(CriticalSizeTableType::fromString(critical_table_type_str).value());

	if (jsonObj.contains("criticalModifiers"))
		ref.setCriticalModifiers(JsonConverter::getEnumSet<CriticalModifierType::Type>(jsonObj, "criticalModifiers"));

	{
		int min = JsonConverter::getNestedInt(jsonObj, "encounterRange/min", 0);
		int max = JsonConverter::getNestedInt(jsonObj, "encounterRange/max", 0);
		if (min != 0 || max != 0) {
			ref.setEncounterRange(std::make_pair(min, max));
		}
	}

	{
		int min = JsonConverter::getNestedInt(jsonObj, "numberYoungRange/min", 0);
		int max = JsonConverter::getNestedInt(jsonObj, "numberYoungRange/max", 0);
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
		std::set<std::string> water_str = JsonConverter::getNestedStringArray(jsonObj, "location/waterSources");
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
	if (jsonObj.contains("standardAttacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "standardAttacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, true);
			const archid::NumberRange<int>* range = number_matcher.matcher(chance.first, chance.second);
			ref.addAttack(range, attack);
		}
	}

	// Ranged attacks
	if (jsonObj.contains("rangedAttacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "rangedAttacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, false);
			ref.addRangedAttack(std::move(attack));
		}
	}

	// Conditional attacks
	if (jsonObj.contains("conditionalAttacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "conditionalAttacks");
		for (json::value& attack_value : attacks_array) {
			rm::game::AnimalAttack attack{};
			deserializeAnimalAttack(attack_value.as_object(), attack, chance, true);
			int ref_id = attack.conditionalAttackRef().value();
			ref.addConditionalAttack(ref_id, std::move(attack));
		}
	}

	// Group attacks
	if (jsonObj.contains("groupAttacks")) {
		std::pair<int, int> chance{};
		json::array attacks_array = JsonConverter::getJsonArray(jsonObj, "groupAttacks");
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
	if (jsonObj.contains("chanceMin") && jsonObj.contains("chanceMax")) {
		chance.first = JsonConverter::getInt(jsonObj, "chanceMin");
		chance.second = JsonConverter::getInt(jsonObj, "chanceMax");
		if (use_chance) {
			NumberMatcherFactory number_matcher{};
			attack.setChance(number_matcher.matcher(chance.first, chance.second));
		}
	}
	if (jsonObj.contains("id"))
		attack.setConditionalAttackRef(JsonConverter::getInt(jsonObj, "id"));
	if (jsonObj.contains("minGroupSize"))
		attack.setMinGroupSize(JsonConverter::getInt(jsonObj, "minGroupSize"));
	if (jsonObj.contains("range"))
		attack.setRange(JsonConverter::getInt(jsonObj, "range"));
	if (jsonObj.contains("offensiveBonus"))
		attack.setOffensiveBonus(JsonConverter::getInt(jsonObj, "offensiveBonus"));
	if (jsonObj.contains("weaponAttack")) {
		std::string weapon_attack_id = JsonConverter::getString(jsonObj, "weaponAttack");
		attack.setWeaponTable(manager_.get<AttackTable>(weapon_attack_id));
	}
	if (jsonObj.contains("nonWeaponAttack")) {
		std::string table_id = JsonConverter::getNestedString(jsonObj, "nonWeaponAttack/table");
		attack.setNonWeaponTable(manager_.get<SpecialAttackTable>(table_id));
		std::string size_str = JsonConverter::getNestedString(jsonObj, "nonWeaponAttack/size");
		AttackSizeType::Type size{};
		AttackSizeType::fromString(size_str, size);
		attack.setNonWeaponSize(size);
	}
	if (jsonObj.contains("useAllAttacks"))
		attack.setUseAllAttacks(JsonConverter::getBool(jsonObj, "useAllAttacks"));
	if (jsonObj.contains("attacksPerRound"))
		attack.setNumAttacks(JsonConverter::getInt(jsonObj, "attacksPerRound"));
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
	if (jsonObj.contains("autoCriticalType")) {
		std::string auto_critical_type_str = JsonConverter::getString(jsonObj, "autoCriticalType");
		CriticalType::Type auto_critical_type{};
		CriticalType::fromString(auto_critical_type_str, auto_critical_type);
		attack.setAutoCriticalType(auto_critical_type);
	}
	if (jsonObj.contains("autoCriticalSize"))
		attack.setAutoCriticalSize(JsonConverter::getString(jsonObj, "autoCriticalSize"));
	if (jsonObj.contains("sameRoundConditionalAttackId"))
		attack.setSameRoundAttackId(JsonConverter::getInt(jsonObj, "sameRoundConditionalAttackId"));
	if (jsonObj.contains("nextRoundConditionalAttackId"))
		attack.setNextRoundAttackId(JsonConverter::getInt(jsonObj, "nextRoundConditionalAttackId"));
}

} // namespace rm::rule::serial