#include <CultureTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value CultureTypeSerializer::serializeObject(const CultureTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "character-concepts", ref.characterConcept());
	JsonConverter::setString(obj, "clothing", ref.clothing());
	JsonConverter::setString(obj, "aspirations", ref.aspirations());
	JsonConverter::setString(obj, "fears", ref.fears());
	JsonConverter::setString(obj, "marriage-patterns", ref.marriagePatterns());
	JsonConverter::setString(obj, "prejudices", ref.prejudices());
	JsonConverter::setString(obj, "religious-beliefs", ref.religiousBeliefs());
	JsonConverter::setInt(obj, "hobby-skill-ranks", ref.hobbySkillRanks());
	if (ref.spellListRanks())
		JsonConverter::setInt(obj, "spell-list-ranks", ref.spellListRanks());
	JsonConverter::setEnumSet(obj, "preferred-armours", ref.preferredArmour());
	JsonConverter::setDataSet(obj, "preferred-weapons", ref.preferredWeapons());
	JsonConverter::setSkillPrimitiveMap(obj, "skill-ranks", ref.skillRanks());
	JsonConverter::setDataPrimitiveMap(obj, "skill-category-ranks", ref.skillCategoryRanks());
	JsonConverter::setDataPrimitiveMap(obj, "skill-category-skill-ranks", ref.skillCategorySkillRanks());
	JsonConverter::setDataSet(obj, "required-climates", ref.requiredClimates());
	JsonConverter::setEnumSet(obj, "required-features", ref.requiredFeatures());
	JsonConverter::setEnumSet(obj, "required-terrains", ref.requiredTerrains());
	JsonConverter::setEnumSet(obj, "required-vegetations", ref.requiredVegetations());
	JsonConverter::setEnumSet(obj, "required-water-sources", ref.requiredWaterSources());


	return obj;
}

const CultureTypeData& CultureTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	CultureTypeData& ref = manager_.get<CultureTypeData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setCharacterConcept(JsonConverter::getString(jsonObj, "character-concepts"));
	ref.setClothing(JsonConverter::getString(jsonObj, "clothing"));
	ref.setAspirations(JsonConverter::getString(jsonObj, "aspirations"));
	ref.setFears(JsonConverter::getString(jsonObj, "fears"));
	ref.setMarriagePatterns(JsonConverter::getString(jsonObj, "marriage-patterns"));
	ref.setPrejudices(JsonConverter::getString(jsonObj, "prejudices"));
	ref.setReligiousBeliefs(JsonConverter::getString(jsonObj, "religious-beliefs"));
	ref.setHobbySkillRanks(JsonConverter::getInt(jsonObj, "hobby-skill-ranks", 0));
	if (jsonObj.find("spell-list-ranks") != jsonObj.end())
		ref.setSpellListRanks(JsonConverter::getInt(jsonObj, "spell-list-ranks", 0));
	ref.setPreferredArmours(JsonConverter::getEnumSet<ArmourType::Type>(jsonObj, "preferred-armours"));
	ref.setPreferredWeapons(JsonConverter::getDataSet<WeaponTypeData>(jsonObj, "preferred-weapons", manager_));
	ref.setSkillRanks(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skill-ranks", manager_));
	ref.setSkillCategoryRanks(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skill-category-ranks", manager_));
	ref.setSkillCategorySkillRanks(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skill-category-skill-ranks", manager_));
	ref.setRequiredClimates(JsonConverter::getDataSet<ClimateData>(jsonObj, "required-climates", manager_));
	ref.setRequiredFeatures(JsonConverter::getEnumSet<EnvironmentType::Feature>(jsonObj, "required-features"));
	ref.setRequiredTerrains(JsonConverter::getEnumSet<EnvironmentType::Terrain>(jsonObj, "required-terrains"));
	ref.setRequiredVegetations(JsonConverter::getEnumSet<EnvironmentType::Vegetation>(jsonObj, "required-vegetations"));
	ref.setRequiredWaterSources(JsonConverter::getEnumSet<EnvironmentType::Water>(jsonObj, "required-water-sources"));

	return ref;
}

} // namespace rm::rule::serial