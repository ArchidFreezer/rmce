#include <CultureSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value CultureSerializer::serializeObject(const CultureData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	if (!ref.description().empty())
		JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "culture-type", ref.cultureType().id());
	JsonConverter::setBool(obj, "high-culture", ref.highCulture());

	// Background languages
	JsonConverter::setLanguageAbilities(obj, "background-languages", ref.languageAbilities());

	// Hobby skills
	JsonConverter::setSkillArray(obj, "hobby-skills", ref.hobbySkills());

	// Hobby category skills
	JsonConverter::setDataObjectArray(obj, "hobby-categories", ref.hobbySkillCategories());

	// Preferred professions
	JsonConverter::setDataObjectArray(obj, "preferred-professions", ref.preferredProfessions());

	// Restricted professions
	JsonConverter::setDataObjectArray(obj, "restricted-professions", ref.restrictedProfessions());

	// Training package cost modifiers
	JsonConverter::setDataObjectPrimitiveMap(obj, "training-package-modifiers", ref.trainingPackageModifiers());

	return obj;
}

const CultureData& CultureSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	CultureData& ref = manager_.get<CultureData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));

	// Description is optionsal, so we check if it exists before trying to get it
	if (jsonObj.contains("description"))
		ref.setDescription(JsonConverter::getString(jsonObj, "description"));

	std::string culture_type_id = JsonConverter::getString(jsonObj, "culture-type");
	ref.setCultureType(manager_.get<CultureTypeData>(culture_type_id));

	ref.setHighCulture(JsonConverter::getBool(jsonObj, "high-culture"));

	// Background languages are optional, so we check if they exist before trying to get them
	ref.setLanguageAbilities(JsonConverter::getLanguageAbilityMap(jsonObj, "background-languages", manager_));

	// Hobby skills
	ref.setHobbySkills(JsonConverter::getSkillSet(jsonObj, "hobby-skills", manager_));

	// Hobby category skills
	ref.setHobbySkillCategories(JsonConverter::getDataObjectSet<SkillCategoryData>(jsonObj, "hobby-categories", manager_));

	// Preferred professions
	ref.setPreferredProfessions(JsonConverter::getDataObjectSet<ProfessionData>(jsonObj, "preferred-professions", manager_));

	// Restricted professions
	ref.setRestrictedProfessions(JsonConverter::getDataObjectSet<ProfessionData>(jsonObj, "restricted-professions", manager_));

	// Training package cost modifiers
	ref.setTrainingPackageModifiers(JsonConverter::getDataObjectPrimitiveMap<TrainingPackageData, float>(jsonObj, "training-package-modifiers", manager_));

	return ref;
}

} // namespace rm::rule::serial