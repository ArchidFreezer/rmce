#include <CreaturePaceSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value CreaturePaceSerializer::serializeObject(const CreaturePaceData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setFloat(obj, "exhaustionMultiplier", ref.exhaustionCostMultiplier());
	JsonConverter::setFloat(obj, "movementMultiplier", ref.movementRateMultiplier());
	JsonConverter::setString(obj, "manoeuvreDifficulty", rule::enums::ManoeuvreDifficultyType::toString(ref.manoeuvreDifficultyType()));

	return obj;
}

const CreaturePaceData& CreaturePaceSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	CreaturePaceData& ref = manager_.get<CreaturePaceData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setExhaustionCostMultiplier(JsonConverter::getFloat(jsonObj, "exhaustionMultiplier"));
	ref.setMovementRateMultiplier(JsonConverter::getFloat(jsonObj, "movementMultiplier"));

	ManoeuvreDifficultyType::Type manoeuvre_difficulty{};
	fromString(JsonConverter::getString(jsonObj, "manoeuvreDifficulty"), manoeuvre_difficulty);
	ref.setManoeuvreDifficultyType(manoeuvre_difficulty);

	return ref;
}

} // namespace rm::rule::serial