#include <RaceSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value RaceSerializer::serializeObject(const RaceData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "book", ref.book() ? ref.book().value()->id() : "");
	JsonConverter::setBool(obj, "high-culture", ref.highCulture());
	JsonConverter::setString(obj, "creature-size", toString(ref.size()));
	JsonConverter::setString(obj, "critical-table", toString(ref.criticalTableType()));
	JsonConverter::setFloat(obj, "recovery-multiplier", ref.recoveryMultiplier());
	JsonConverter::setInt(obj, "background-options", ref.numBackgroundOptions());
	JsonConverter::setInt(obj, "exhaustion-bonus", ref.exhaustionBonus());
	JsonConverter::setInt(obj, "stat-loss-racial-type", ref.statLossRacialType());
	JsonConverter::setInt(obj, "required-sleep", ref.requiredSleep());
	JsonConverter::setInt(obj, "required-sleep-frequency", ref.requiredSleepFrequency());
	JsonConverter::setInt(obj, "soul-departure", ref.soulDeparture());
	JsonConverter::setInt(obj, "build-modifier", ref.buildModifier());
	JsonConverter::setInt(obj, "average-male-height", ref.averageMaleHeight());
	JsonConverter::setInt(obj, "average-female-height", ref.averageFemaleHeight());
	JsonConverter::setInt(obj, "average-lifespan", ref.averageLifespan());
	JsonConverter::setInt(obj, "male-weight-modifier", ref.maleWeightModifier());
	JsonConverter::setInt(obj, "female-weight-modifier", ref.femaleWeightModifier());
	JsonConverter::setString(obj, "arcane-progression", ref.arcaneProgression().id());
	JsonConverter::setString(obj, "arms-progression", ref.armsProgression().id());
	JsonConverter::setString(obj, "channeling-progression", ref.channelingProgression().id());
	JsonConverter::setString(obj, "essence-progression", ref.essenceProgression().id());
	JsonConverter::setString(obj, "mentalism-progression", ref.mentalismProgression().id());
	JsonConverter::setLanguageAbilities(obj, "starting-languages", ref.startingLanguageAbilities());
	JsonConverter::setLanguageAbilities(obj, "adolescent-languages", ref.adolescentLanguageAbilities());
	JsonConverter::setEnumPrimitiveMap<StatType::Type, int>(obj, "stat-bonuses", ref.statBonuses());
	JsonConverter::setSkillSet(obj, "everyman-skills", ref.everymanSkills());
	JsonConverter::setSkillSet(obj, "restricted-skills", ref.restrictedSkills());
	JsonConverter::setDataSet<SkillCategoryData>(obj, "everyman-categories", ref.everymanSkillCategories());
	JsonConverter::setDataSet<SkillCategoryData>(obj, "restricted-categories", ref.restrictedSkillCategories());
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skill-bonuses", ref.skillBonuses());
	JsonConverter::setDataChoiceSet<SkillCategoryData>(obj, "skill-category-choices-everyman", ref.categoryEverymanSkillChoices());

	return obj;
}

const RaceData& RaceSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	RaceData& ref = manager_.get<RaceData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setHighCulture(JsonConverter::getBool(jsonObj, "high-culture"));
	ref.setRecoveryMultiplier(JsonConverter::getFloat(jsonObj, "recovery-multiplier"));
	ref.setNumBackgroundOptions(JsonConverter::getInt(jsonObj, "background-options"));
	ref.setExhaustionBonus(JsonConverter::getInt(jsonObj, "exhaustion-bonus"));
	ref.setStatLossRacialType(JsonConverter::getInt(jsonObj, "stat-loss-racial-type"));
	ref.setRequiredSleep(JsonConverter::getInt(jsonObj, "required-sleep"));
	ref.setRequiredSleepFrequency(JsonConverter::getInt(jsonObj, "required-sleep-frequency"));
	ref.setSoulDeparture(JsonConverter::getInt(jsonObj, "soul-departure"));
	ref.setBuildModifier(JsonConverter::getInt(jsonObj, "build-modifier"));
	ref.setAverageMaleHeight(JsonConverter::getInt(jsonObj, "average-male-height"));
	ref.setAverageFemaleHeight(JsonConverter::getInt(jsonObj, "average-female-height"));
	ref.setAverageLifespan(JsonConverter::getInt(jsonObj, "average-lifespan"));
	ref.setMaleWeightModifier(JsonConverter::getInt(jsonObj, "male-weight-modifier"));
	ref.setFemaleWeightModifier(JsonConverter::getInt(jsonObj, "female-weight-modifier"));

	// Book
	std::string book_id = JsonConverter::getString(jsonObj, "book");
	ref.setBook(manager_.get<BookData>(book_id));

	// Creature size
	CreatureSizeType::Type size;
	fromString(JsonConverter::getString(jsonObj, "creature-size"), size);
	ref.setSize(size);

		// get the critical type
	CriticalTableType::Type crit_table_type{};
	fromString(JsonConverter::getString(jsonObj, "critical-table"), crit_table_type);
	ref.setCriticalTableType(crit_table_type);

	// Get the realm progressions
	std::string arcane_progression = JsonConverter::getString(jsonObj, "arcane-progression");
	ref.setArcaneProgression(manager().get<SkillProgressionTypeData>(arcane_progression));
	std::string arms_progression = JsonConverter::getString(jsonObj, "arms-progression");
	ref.setArmsProgression(manager().get<SkillProgressionTypeData>(arms_progression));
	std::string channeling_progression = JsonConverter::getString(jsonObj, "channeling-progression");
	ref.setChannelingProgression(manager().get<SkillProgressionTypeData>(channeling_progression));
	std::string essence_progression = JsonConverter::getString(jsonObj, "essence-progression");
	ref.setEssenceProgression(manager().get<SkillProgressionTypeData>(essence_progression));
	std::string mentalism_progression = JsonConverter::getString(jsonObj, "mentalism-progression");
	ref.setMentalismProgression(manager().get<SkillProgressionTypeData>(mentalism_progression));
	
	// Languages
	ref.setStartingLanguageAbilities(JsonConverter::getLanguageAbilityMap(jsonObj, "starting-languages", manager_));
	ref.setAdolescentLanguageAbilities(JsonConverter::getLanguageAbilityMap(jsonObj, "adolescent-languages", manager_));

	ref.setStatBonuses(JsonConverter::getEnumPrimitiveMap<StatType::Type, int>(jsonObj, "stat-bonuses"));
	ref.setEverymanSkills(JsonConverter::getSkillSet(jsonObj, "everyman-skills", manager_));
	ref.setRestrictedSkills(JsonConverter::getSkillSet(jsonObj, "restricted-skills", manager_));
	ref.setEverymanSkillCategories(JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "everyman-categories", manager_));
	ref.setRestrictedSkillCategories(JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "restricted-categories", manager_));
	ref.setSkillBonuses(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skill-bonuses", manager_));
	ref.setCategoryEverymanSkillChoices(JsonConverter::getDataChoiceSet<SkillCategoryData>(jsonObj, "skill-category-choices-everyman", manager_));

	return ref;
}

} // namespace rm::rule::serial