#include <ClimateSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value ClimateSerializer::serializeObject(const ClimateData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "temperature", HabitatType::toString(ref.temperature()));

	// We convert the set of precipitations to a JSON array of strings
	json::array precipitations;
	for (const auto& precipitation : ref.precipitations()) {
		precipitations.push_back(json::value(HabitatType::toString(precipitation)));
	}
	obj["precipitations"] = precipitations;

	return obj;
}

const ClimateData& ClimateSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	ClimateData& ref = manager_.get<ClimateData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));

	std::optional<HabitatType::Temperature> temperature = HabitatType::temperature(JsonConverter::getString(jsonObj, "temperature"));
	if (temperature)
		ref.setTemperature(temperature.value());

	std::set<HabitatType::Precipitation> precipitations = JsonConverter::getEnumSet<HabitatType::Precipitation>(jsonObj, "precipitations");
	if (!precipitations.empty())
		ref.setPrecipitations(precipitations);

	return ref;
}

} // namespace rm::rule::serial