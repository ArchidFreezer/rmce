#include <RaceSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value RaceSerializer::serializeObject(const RaceData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "book", ref.book() ? ref.book().value()->id() : "");
	JsonConverter::setBool(obj, "highCulture", ref.highCulture());
	JsonConverter::setString(obj, "creatureSize", toString(ref.size()));
	JsonConverter::setString(obj, "criticalTable", toString(ref.criticalTableType()));
	JsonConverter::setFloat(obj, "recoveryMultiplier", ref.recoveryMultiplier());
	JsonConverter::setInt(obj, "backgroundOptions", ref.numBackgroundOptions());
	JsonConverter::setInt(obj, "exhaustionBonus", ref.exhaustionBonus());
	JsonConverter::setInt(obj, "statLossRacialType", ref.statLossRacialType());
	JsonConverter::setInt(obj, "requiredSleep", ref.requiredSleep());
	JsonConverter::setInt(obj, "requiredSleepFrequency", ref.requiredSleepFrequency());
	JsonConverter::setInt(obj, "soulDeparture", ref.soulDeparture());
	JsonConverter::setInt(obj, "buildModifier", ref.buildModifier());
	JsonConverter::setInt(obj, "averageMaleHeight", ref.averageMaleHeight());
	JsonConverter::setInt(obj, "averageFemaleHeight", ref.averageFemaleHeight());
	JsonConverter::setInt(obj, "averageLifespan", ref.averageLifespan());
	JsonConverter::setInt(obj, "maleWeightModifier", ref.maleWeightModifier());
	JsonConverter::setInt(obj, "femaleWeightModifier", ref.femaleWeightModifier());
	JsonConverter::setString(obj, "arcaneProgression", ref.arcaneProgression().id());
	JsonConverter::setString(obj, "armsProgression", ref.armsProgression().id());
	JsonConverter::setString(obj, "channelingProgression", ref.channelingProgression().id());
	JsonConverter::setString(obj, "essenceProgression", ref.essenceProgression().id());
	JsonConverter::setString(obj, "mentalismProgression", ref.mentalismProgression().id());
	JsonConverter::setLanguageAbilities(obj, "startingLanguages", ref.startingLanguageAbilities());
	JsonConverter::setLanguageAbilities(obj, "adolescentLanguages", ref.adolescentLanguageAbilities());
	JsonConverter::setEnumPrimitiveMap<StatType::Type, int>(obj, "statBonuses", ref.statBonuses());
	JsonConverter::setSkillSet(obj, "everymanSkills", ref.everymanSkills());
	JsonConverter::setSkillSet(obj, "restrictedSkills", ref.restrictedSkills());
	JsonConverter::setDataSet<SkillCategoryData>(obj, "everymanCategories", ref.everymanSkillCategories());
	JsonConverter::setDataSet<SkillCategoryData>(obj, "restrictedCategories", ref.restrictedSkillCategories());
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skillBonuses", ref.skillBonuses());
	JsonConverter::setDataChoiceSet<SkillCategoryData>(obj, "skillCategoryChoicesEveryman", ref.categoryEverymanSkillChoices());

	return obj;
}

const RaceData& RaceSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	RaceData& ref = manager_.get<RaceData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setHighCulture(JsonConverter::getBool(jsonObj, "highCulture"));
	ref.setRecoveryMultiplier(JsonConverter::getFloat(jsonObj, "recoveryMultiplier"));
	ref.setNumBackgroundOptions(JsonConverter::getInt(jsonObj, "backgroundOptions"));
	ref.setExhaustionBonus(JsonConverter::getInt(jsonObj, "exhaustionBonus"));
	ref.setStatLossRacialType(JsonConverter::getInt(jsonObj, "statLossRacialType"));
	ref.setRequiredSleep(JsonConverter::getInt(jsonObj, "requiredSleep"));
	ref.setRequiredSleepFrequency(JsonConverter::getInt(jsonObj, "requiredSleepFrequency"));
	ref.setSoulDeparture(JsonConverter::getInt(jsonObj, "soulDeparture"));
	ref.setBuildModifier(JsonConverter::getInt(jsonObj, "buildModifier"));
	ref.setAverageMaleHeight(JsonConverter::getInt(jsonObj, "averageMaleHeight"));
	ref.setAverageFemaleHeight(JsonConverter::getInt(jsonObj, "averageFemaleHeight"));
	ref.setAverageLifespan(JsonConverter::getInt(jsonObj, "averageLifespan"));
	ref.setMaleWeightModifier(JsonConverter::getInt(jsonObj, "maleWeightModifier"));
	ref.setFemaleWeightModifier(JsonConverter::getInt(jsonObj, "femaleWeightModifier"));

	// Book
	std::string book_id = JsonConverter::getString(jsonObj, "book");
	ref.setBook(manager_.get<BookData>(book_id));

	// Creature size
	CreatureSizeType::Type size;
	fromString(JsonConverter::getString(jsonObj, "creatureSize"), size);
	ref.setSize(size);

		// get the critical type
	CriticalTableType::Type crit_table_type{};
	fromString(JsonConverter::getString(jsonObj, "criticalTable"), crit_table_type);
	ref.setCriticalTableType(crit_table_type);

	// Get the realm progressions
	std::string arcane_progression = JsonConverter::getString(jsonObj, "arcaneProgression");
	ref.setArcaneProgression(manager().get<SkillProgressionTypeData>(arcane_progression));
	std::string arms_progression = JsonConverter::getString(jsonObj, "armsProgression");
	ref.setArmsProgression(manager().get<SkillProgressionTypeData>(arms_progression));
	std::string channeling_progression = JsonConverter::getString(jsonObj, "channelingProgression");
	ref.setChannelingProgression(manager().get<SkillProgressionTypeData>(channeling_progression));
	std::string essence_progression = JsonConverter::getString(jsonObj, "essenceProgression");
	ref.setEssenceProgression(manager().get<SkillProgressionTypeData>(essence_progression));
	std::string mentalism_progression = JsonConverter::getString(jsonObj, "mentalismProgression");
	ref.setMentalismProgression(manager().get<SkillProgressionTypeData>(mentalism_progression));
	
	// Languages
	ref.setStartingLanguageAbilities(JsonConverter::getLanguageAbilityMap(jsonObj, "startingLanguages", manager_));
	ref.setAdolescentLanguageAbilities(JsonConverter::getLanguageAbilityMap(jsonObj, "adolescentLanguages", manager_));

	ref.setStatBonuses(JsonConverter::getEnumPrimitiveMap<StatType::Type, int>(jsonObj, "statBonuses"));
	ref.setEverymanSkills(JsonConverter::getSkillSet(jsonObj, "everymanSkills", manager_));
	ref.setRestrictedSkills(JsonConverter::getSkillSet(jsonObj, "restrictedSkills", manager_));
	ref.setEverymanSkillCategories(JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "everymanCategories", manager_));
	ref.setRestrictedSkillCategories(JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "restrictedCategories", manager_));
	ref.setSkillBonuses(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillBonuses", manager_));
	ref.setCategoryEverymanSkillChoices(JsonConverter::getDataChoiceSet<SkillCategoryData>(jsonObj, "skillCategoryChoicesEveryman", manager_));

	return ref;
}

} // namespace rm::rule::serial