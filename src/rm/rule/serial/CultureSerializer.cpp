#include <CultureSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value CultureSerializer::serializeObject(const CultureData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	if (!ref.description().empty())
		JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "cultureType", ref.cultureType().id());
	JsonConverter::setBool(obj, "highCulture", ref.highCulture());

	// Background languages
	JsonConverter::setLanguageAbilities(obj, "backgroundLanguages", ref.languageAbilities());

	// Hobby skills
	JsonConverter::setSkillSet(obj, "hobbySkills", ref.hobbySkills());

	// Hobby category skills
	JsonConverter::setDataSet(obj, "hobbyCategories", ref.hobbySkillCategories());

	// Preferred professions
	JsonConverter::setDataSet(obj, "preferredProfessions", ref.preferredProfessions());

	// Restricted professions
	JsonConverter::setDataSet(obj, "restrictedProfessions", ref.restrictedProfessions());

	// Training package cost modifiers
	JsonConverter::setDataPrimitiveMap(obj, "trainingPackageModifiers", ref.trainingPackageModifiers());

	return obj;
}

const CultureData& CultureSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	CultureData& ref = manager_.get<CultureData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));

	// Description is optionsal, so we check if it exists before trying to get it
	if (jsonObj.contains("description"))
		ref.setDescription(JsonConverter::getString(jsonObj, "description"));

	std::string culture_type_id = JsonConverter::getString(jsonObj, "cultureType");
	ref.setCultureType(manager_.get<CultureTypeData>(culture_type_id));

	ref.setHighCulture(JsonConverter::getBool(jsonObj, "highCulture"));

	// Background languages are optional, so we check if they exist before trying to get them
	ref.setLanguageAbilities(JsonConverter::getLanguageAbilityMap(jsonObj, "backgroundLanguages", manager_));

	// Hobby skills
	ref.setHobbySkills(JsonConverter::getSkillSet(jsonObj, "hobbySkills", manager_));

	// Hobby category skills
	ref.setHobbySkillCategories(JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "hobbyCategories", manager_));

	// Preferred professions
	ref.setPreferredProfessions(JsonConverter::getDataSet<ProfessionData>(jsonObj, "preferredProfessions", manager_));

	// Restricted professions
	ref.setRestrictedProfessions(JsonConverter::getDataSet<ProfessionData>(jsonObj, "restrictedProfessions", manager_));

	// Training package cost modifiers
	ref.setTrainingPackageModifiers(JsonConverter::getDataPrimitiveMap<TrainingPackageData, float>(jsonObj, "trainingPackageModifiers", manager_));

	return ref;
}

} // namespace rm::rule::serial