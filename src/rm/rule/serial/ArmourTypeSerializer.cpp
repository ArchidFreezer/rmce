#include <ArmourTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value ArmourTypeSerializer::serializeObject(const ArmourTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "type", toString(ref.type()));
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setInt(obj, "min-manoeuvre-mod", ref.minManoeuvreMod());
	JsonConverter::setInt(obj, "max-manoeuvre-mod", ref.maxManoeuvreMod());
	JsonConverter::setInt(obj, "missile-attack-penalty", ref.missileAttackPenalty());
	JsonConverter::setInt(obj, "quickness-penalty", ref.quicknessPenalty());
	JsonConverter::setBool(obj, "animal-only", ref.animalOnly());
	JsonConverter::setBool(obj, "includes-greaves", ref.includesGreaves());

	return obj;
}

const ArmourTypeData& ArmourTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	ArmourTypeData& ref = manager_.get<ArmourTypeData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	std::string type_name = JsonConverter::getString(jsonObj, "type");
	if (ArmourType::fromString(type_name)) {
		ref.setType(ArmourType::fromString(type_name).value());
	} else {
		throw ArmourType::ArmourTypeNotFoundException("Could not find an armour type for: " + type_name);
	}
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));
	ref.setMinManoeuvreMod(JsonConverter::getInt(jsonObj, "min-manoeuvre-mod"));
	ref.setMaxManoeuvreMod(JsonConverter::getInt(jsonObj, "max-manoeuvre-mod"));
	ref.setMissileAttackPenalty(JsonConverter::getInt(jsonObj, "missile-attack-penalty"));
	ref.setQuicknessPenalty(JsonConverter::getInt(jsonObj, "quickness-penalty"));
	ref.setAnimalOnly(JsonConverter::getBool(jsonObj, "animal-only"));
	ref.setIncludesGreaves(JsonConverter::getBool(jsonObj, "includes-greaves"));
	return ref;
}

} // namespace rm::rule::serial