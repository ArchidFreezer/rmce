#include <PoisonSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value PoisonSerializer::serializeObject(const PoisonData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "type", ref.type().id());
	JsonConverter::setInt(obj, "level", ref.averageLevel());
	JsonConverter::setString(obj, "levelVariance", LevelVarianceType::toString(ref.levelVarianceType()));

	return obj;
}

const PoisonData& PoisonSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	PoisonData& ref = manager_.get<PoisonData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));

	std::string type_name = JsonConverter::getString(jsonObj, "type");
	ref.setType(manager_.get<PoisonTypeData>(type_name));

	ref.setAverageLevel(JsonConverter::getInt(jsonObj, "level", 1));

	// Get the level variance type based on the string value in the json file and set it
	LevelVarianceType::Type level_variance_type{};
	LevelVarianceType::fromString(JsonConverter::getString(jsonObj, "levelVariance"), level_variance_type);
	ref.setLevelVarianceType(level_variance_type);

	return ref;
}

} // namespace rm::rule::serial