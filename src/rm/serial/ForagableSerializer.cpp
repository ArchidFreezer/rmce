#include <ForagableSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value ForagableSerializer::serializeObject(const ForagableData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "otherNames", ref.otherNames());
	JsonConverter::setString(obj, "loreSkill", ref.loreSkill() ? ref.loreSkill()->id() : "");
	JsonConverter::setString(obj, "characteristics", ref.characteristics());
	JsonConverter::setString(obj, "medicinalUses", ref.medicinalUses());
	JsonConverter::setString(obj, "otherUses", ref.otherUses());
	JsonConverter::setString(obj, "warning", ref.warning());
	JsonConverter::setString(obj, "preparationType", ForagablePreparationType::toString(ref.preparationType()));

	JsonConverter::setString(obj, "effectType", ForagableEffectType::toString(ref.effectType()));
	JsonConverter::setString(obj, "findDifficulty", SkillDifficultyType::toString(ref.findDifficulty()));
	JsonConverter::setInt(obj, "addictionFactor", ref.addictionFactor());
	JsonConverter::setString(obj, "cost", ref.cost());

	// Location data
	JsonConverter::nestLocation(obj, "location", ref.location());


	return obj;
}

const ForagableData& ForagableSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	ForagableData& ref = manager_.get<ForagableData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setOtherNames(JsonConverter::getString(jsonObj, "otherNames"));
	std::string lore_skill_id = JsonConverter::getString(jsonObj, "loreSkill");
	if (!lore_skill_id.empty()) {
		const SubcategoriedSkillData& lore_skill = manager_.subcategoriedSkillData(lore_skill_id);
		ref.setLoreSkill(&lore_skill);
	}

	std::string find_difficulty_str = JsonConverter::getString(jsonObj, "findDifficulty");
	ref.setFindDifficulty(SkillDifficultyType::fromString(find_difficulty_str).value());

	ref.setCharacteristics(JsonConverter::getString(jsonObj, "characteristics"));
	ref.setMedicinalUses(JsonConverter::getString(jsonObj, "medicinalUses"));
	ref.setOtherUses(JsonConverter::getString(jsonObj, "otherUses"));
	ref.setWarning(JsonConverter::getString(jsonObj, "warning"));

	std::string preparation_type_str = JsonConverter::getString(jsonObj, "preparationType");
	ref.setPreparationType(ForagablePreparationType::fromString(preparation_type_str).value());

	std::string effect_type_str = JsonConverter::getString(jsonObj, "effectType");
	ref.setEffectType(ForagableEffectType::fromString(effect_type_str).value());

	ref.setAddictionFactor(JsonConverter::getInt(jsonObj, "addictionFactor"));
	ref.setCost(JsonConverter::getString(jsonObj, "cost"));

	// Location data
	rm::game::Location location = JsonConverter::getLocation(jsonObj, "location", manager_);
	ref.setLocation(location);


	return ref;
}

} // namespace rm::serial