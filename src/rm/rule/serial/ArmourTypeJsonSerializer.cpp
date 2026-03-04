#include <ArmourTypeJsonSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value ArmourTypeJsonSerializer::serializeObject(const ArmourTypeData& armourtype) const {
	json::object obj;

	JsonConverter::setString(obj, "id", armourtype.id());
	JsonConverter::setString(obj, "name", armourtype.name());
	JsonConverter::setString(obj, "type", toString(armourtype.type()));
	JsonConverter::setString(obj, "description", armourtype.description());
	JsonConverter::setInt(obj, "min-manoeuvre-mod", armourtype.minManoeuvreMod());
	JsonConverter::setInt(obj, "max-manoeuvre-mod", armourtype.maxManoeuvreMod());
	JsonConverter::setInt(obj, "missile-attack-penalty", armourtype.missileAttackPenalty());
	JsonConverter::setInt(obj, "quickness-penalty", armourtype.quicknessPenalty());
	JsonConverter::setBool(obj, "animal-only", armourtype.animalOnly());
	JsonConverter::setBool(obj, "includes-greaves", armourtype.includesGreaves());

	return obj;
}

const ArmourTypeData& ArmourTypeJsonSerializer::deserializeObject(json::object& jsonObj) const {
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