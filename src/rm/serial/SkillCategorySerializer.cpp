#include <SkillCategorySerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value SkillCategorySerializer::serializeObject(const SkillCategoryData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "group", ref.group().id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setBool(obj, "useRealmStats", ref.useRealmStats());
	JsonConverter::setString(obj, "skillProgression", ref.defaultSkillProgression().id());
	JsonConverter::setString(obj, "categoryProgression", ref.skillCategoryProgression().id());
	JsonConverter::setEnumSet(obj, "stats", ref.stats());

	return obj;
}

const SkillCategoryData& SkillCategorySerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	SkillCategoryData& ref = manager_.get<SkillCategoryData>(id);

	ref.setGroup(manager_.get<SkillGroupData>(JsonConverter::getString(jsonObj, "group")));
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setUseRealmStats(JsonConverter::getBool(jsonObj, "useRealmStats", false));

	// Get the skill and category progressions
	std::string skill_progression_id = JsonConverter::getString(jsonObj, "skillProgression", "");
	SkillProgressionTypeData& skill_progression{manager().get<SkillProgressionTypeData>(skill_progression_id)};
	std::string category_progression_id = JsonConverter::getString(jsonObj, "categoryProgression", "");
	SkillProgressionTypeData& category_progression{manager().get<SkillProgressionTypeData>(category_progression_id)};
	ref.setSkillProgressions(skill_progression, category_progression);
	ref.setStats(JsonConverter::getEnumVector<StatType::Type>(jsonObj, "stats"));


	return ref;
}

} // namespace rm::serial