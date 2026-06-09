#include <ForagableSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value ForagableSerializer::serializeObject(const ForagableData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "effectType", ForagableEffectType::toString(ref.effectType()));
	JsonConverter::setString(obj, "form", ref.form());
	JsonConverter::setString(obj, "difficulty", SkillDifficultyType::toString(ref.difficulty()));
	JsonConverter::setString(obj, "preparationType", ForagablePreparationType::toString(ref.preparationType()));
	JsonConverter::setInt(obj, "addictionFactor", ref.addictionFactor());
	JsonConverter::setString(obj, "cost", ref.cost());

	// Location data
	JsonConverter::nestLocation(obj, "location", ref.location());

	JsonConverter::setString(obj, "effect", ref.effect());

	return obj;
}

const ForagableData& ForagableSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	ForagableData& ref = manager_.get<ForagableData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setForm(JsonConverter::getString(jsonObj, "form"));

	std::string difficulty_str = JsonConverter::getString(jsonObj, "difficulty");
	ref.setDifficulty(SkillDifficultyType::fromString(difficulty_str).value());

	std::string preparation_type_str = JsonConverter::getString(jsonObj, "preparationType");
	ref.setPreparationType(ForagablePreparationType::fromString(preparation_type_str).value());

	std::string effect_type_str = JsonConverter::getString(jsonObj, "effectType");
	ref.setEffectType(ForagableEffectType::fromString(effect_type_str).value());

	ref.setAddictionFactor(JsonConverter::getInt(jsonObj, "addictionFactor"));
	ref.setCost(JsonConverter::getString(jsonObj, "cost"));

	// Location data
	rm::game::Location location = JsonConverter::getLocation(jsonObj, "location", manager_);
	ref.setLocation(location);

	ref.setEffect(JsonConverter::getString(jsonObj, "effect"));

	return ref;
}

} // namespace rm::serial