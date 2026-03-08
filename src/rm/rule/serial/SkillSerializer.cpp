#include <SkillSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value SkillSerializer::serializeObject(const SkillData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "category", ref.category().id());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "book", ref.book().id());
	JsonConverter::setString(obj, "action", toString(ref.actionType()));
	JsonConverter::setString(obj, "difficulties-summary", ref.difficultySummary());
	JsonConverter::setString(obj, "notes", ref.notes());
	JsonConverter::setBool(obj, "is-restricted", ref.restricted());
	JsonConverter::setBool(obj, "can-specialise", ref.canSpecialise());
	JsonConverter::setBool(obj, "mandatory-subcategory", ref.mandatorySubcategory());
	JsonConverter::setStringSet(obj, "subcategories", ref.subcategories());
	JsonConverter::setEnumSet(obj, "stats", ref.stats());
	JsonConverter::setFloat(obj, "exhaustion", ref.exhaustionCost());
	JsonConverter::setFloat(obj, "distance-multiplier", ref.distanceMultiplier());

	return obj;
}

const SkillData& SkillSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	SkillData& ref = manager_.get<SkillData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setCategory(manager_.get<SkillCategoryData>(JsonConverter::getString(jsonObj, "category")));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setBook(manager_.get<BookData>(JsonConverter::getString(jsonObj, "book")));
	
	// Set action type
	SkillActionType::Type action_type{};
	fromString(JsonConverter::getString(jsonObj, "action"), action_type);
	ref.setActionType(action_type);

	ref.setDifficultySummary(JsonConverter::getString(jsonObj, "difficulties-summary"));
	ref.setNotes(JsonConverter::getString(jsonObj, "notes"));
	ref.setIsRestricted(JsonConverter::getBool(jsonObj, "is-restricted", false));
	ref.setCanSpecialise(JsonConverter::getBool(jsonObj, "can-specialise", false));
	ref.setMandatorySubcategory(JsonConverter::getBool(jsonObj, "mandatory-subcategory", false));
	ref.setSubcategories(JsonConverter::getStringSet(jsonObj, "subcategories"));
	ref.setStats(JsonConverter::getEnumVector<StatType::Type>(jsonObj, "stats"));
	ref.setExhaustionCost(JsonConverter::getFloat(jsonObj, "exhaustion", 0.0f));
	ref.setDistanceMultiplier(JsonConverter::getFloat(jsonObj, "distance-multiplier", 1.0f));

	return ref;
}

} // namespace rm::rule::serial