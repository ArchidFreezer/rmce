#include <CultureTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value CultureTypeSerializer::serializeObject(const CultureTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "characterConcepts", ref.characterConcept());
	JsonConverter::setString(obj, "clothing", ref.clothing());
	JsonConverter::setString(obj, "aspirations", ref.aspirations());
	JsonConverter::setString(obj, "fears", ref.fears());
	JsonConverter::setString(obj, "marriagePatterns", ref.marriagePatterns());
	JsonConverter::setString(obj, "prejudices", ref.prejudices());
	JsonConverter::setString(obj, "religiousBeliefs", ref.religiousBeliefs());
	JsonConverter::setInt(obj, "hobbySkillRanks", ref.hobbySkillRanks());
	JsonConverter::setInt(obj, "adolescentLanguageRanks", ref.adolescentLanguageRanks());
	if (ref.spellListRanks())
		JsonConverter::setInt(obj, "spellListRanks", ref.spellListRanks());
	JsonConverter::setDataSet(obj, "preferredArmours", ref.preferredArmour());
	JsonConverter::setDataSet(obj, "preferredWeapons", ref.preferredWeapons());
	JsonConverter::setSkillPrimitiveMap(obj, "skillRanks", ref.skillRanks());
	JsonConverter::setDataPrimitiveMap(obj, "skillCategoryRanks", ref.skillCategoryRanks());
	JsonConverter::setDataPrimitiveMap(obj, "skillCategorySkillRanks", ref.skillCategorySkillRanks());
	JsonConverter::setEnumSet(obj, "requiredClimates", ref.requiredClimates());
	JsonConverter::setEnumSet(obj, "requiredFeatures", ref.requiredFeatures());
	JsonConverter::setEnumSet(obj, "requiredTerrains", ref.requiredTerrains());
	JsonConverter::setEnumSet(obj, "requiredVegetations", ref.requiredVegetations());
	JsonConverter::setEnumSet(obj, "requiredWaterSources", ref.requiredWaterSources());

	return obj;
}

const CultureTypeData& CultureTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	CultureTypeData& ref = manager_.get<CultureTypeData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setCharacterConcept(JsonConverter::getString(jsonObj, "characterConcepts"));
	ref.setClothing(JsonConverter::getString(jsonObj, "clothing"));
	ref.setAspirations(JsonConverter::getString(jsonObj, "aspirations"));
	ref.setFears(JsonConverter::getString(jsonObj, "fears"));
	ref.setMarriagePatterns(JsonConverter::getString(jsonObj, "marriagePatterns"));
	ref.setPrejudices(JsonConverter::getString(jsonObj, "prejudices"));
	ref.setReligiousBeliefs(JsonConverter::getString(jsonObj, "religiousBeliefs"));
	ref.setHobbySkillRanks(JsonConverter::getInt(jsonObj, "hobbySkillRanks", 0));
	ref.setAdolescentLanguageRanks(JsonConverter::getInt(jsonObj, "adolescentLanguageRanks", 0));
	if (jsonObj.find("spellListRanks") != jsonObj.end())
		ref.setSpellListRanks(JsonConverter::getInt(jsonObj, "spellListRanks", 0));
	ref.setPreferredArmours(JsonConverter::getDataSet<ArmourTypeData>(jsonObj, "preferredArmours", manager_));
	ref.setPreferredWeapons(JsonConverter::getDataSet<WeaponTypeData>(jsonObj, "preferredWeapons", manager_));
	ref.setSkillRanks(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillRanks", manager_));
	ref.setSkillCategoryRanks(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skillCategoryRanks", manager_));
	ref.setSkillCategorySkillRanks(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skillCategorySkillRanks", manager_));
	ref.setRequiredClimates(JsonConverter::getEnumSet<ClimateType::KoppenSubGroup>(jsonObj, "requiredClimates"));
	ref.setRequiredFeatures(JsonConverter::getEnumSet<EnvironmentType::Feature>(jsonObj, "requiredFeatures"));
	ref.setRequiredTerrains(JsonConverter::getEnumSet<EnvironmentType::Terrain>(jsonObj, "requiredTerrains"));
	ref.setRequiredVegetations(JsonConverter::getEnumSet<EnvironmentType::Vegetation>(jsonObj, "requiredVegetations"));
	ref.setRequiredWaterSources(JsonConverter::getEnumSet<EnvironmentType::Water>(jsonObj, "requiredWaterSources"));

	return ref;
}

} // namespace rm::serial