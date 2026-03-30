#include <ArmourTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value ArmourTypeSerializer::serializeObject(const ArmourTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "type", toString(ref.type()));
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setInt(obj, "minManoeuvreMod", ref.minManoeuvreMod());
	JsonConverter::setInt(obj, "maxManoeuvreMod", ref.maxManoeuvreMod());
	JsonConverter::setInt(obj, "missileAttackPenalty", ref.missileAttackPenalty());
	JsonConverter::setInt(obj, "quicknessPenalty", ref.quicknessPenalty());
	JsonConverter::setBool(obj, "animalOnly", ref.animalOnly());
	JsonConverter::setBool(obj, "includesGreaves", ref.includesGreaves());

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
	ref.setMinManoeuvreMod(JsonConverter::getInt(jsonObj, "minManoeuvreMod"));
	ref.setMaxManoeuvreMod(JsonConverter::getInt(jsonObj, "maxManoeuvreMod"));
	ref.setMissileAttackPenalty(JsonConverter::getInt(jsonObj, "missileAttackPenalty"));
	ref.setQuicknessPenalty(JsonConverter::getInt(jsonObj, "quicknessPenalty"));
	ref.setAnimalOnly(JsonConverter::getBool(jsonObj, "animalOnly"));
	ref.setIncludesGreaves(JsonConverter::getBool(jsonObj, "includesGreaves"));
	return ref;
}

} // namespace rm::serial