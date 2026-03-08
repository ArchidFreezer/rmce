#include <SkillCategorySerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value SkillCategorySerializer::serializeObject(const SkillCategoryData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "group", ref.group().id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setBool(obj, "use-realm-stats", ref.useRealmStats());
	JsonConverter::setString(obj, "skill-progression", ref.defaultSkillProgression().id());
	JsonConverter::setString(obj, "category-progression", ref.skillCategoryProgression().id());
	JsonConverter::setEnumSet(obj, "stats", ref.stats());

	return obj;
}

const SkillCategoryData& SkillCategorySerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	SkillCategoryData& ref = manager_.get<SkillCategoryData>(id);

	ref.setGroup(manager_.get<SkillGroupData>(JsonConverter::getString(jsonObj, "group")));
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setUseRealmStats(JsonConverter::getBool(jsonObj, "use-realm-stats", false));

	// Get the skill and category progressions
	std::string skill_progression_id = JsonConverter::getString(jsonObj, "skill-progression", "");
	SkillProgressionTypeData& skill_progression{manager().get<SkillProgressionTypeData>(skill_progression_id)};
	std::string category_progression_id = JsonConverter::getString(jsonObj, "category-progression", "");
	SkillProgressionTypeData& category_progression{manager().get<SkillProgressionTypeData>(category_progression_id)};
	ref.setSkillProgressions(skill_progression, category_progression);
	ref.setStats(JsonConverter::getEnumVector<StatType::Type>(jsonObj, "stats"));


	return ref;
}

} // namespace rm::rule::serial