#include <NumberMatcherFactory.h>
#include <PoisonTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value PoisonTypeSerializer::serializeObject(const PoisonTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "type", PoisonType::toString(ref.type()));
	JsonConverter::setString(obj, "areas-affected", ref.areasAffected());

	// Onset times are stored in the json as an array of objects with keys corresponding to the severity level and values corresponding to the effect onset times for that severity level. We need to loop through the map and add each severity
	// level and its corresponding effect onset times to the json object
	json::array onset_times_array;
	for (const auto& [severity, onset_time] : ref.effectOnsets()) {
		json::object onset_time_obj;
		JsonConverter::setString(onset_time_obj, "severity", DiseasePoisonSeverityType::toString(severity));
		JsonConverter::setInt(onset_time_obj, "min", onset_time->min());
		JsonConverter::setInt(onset_time_obj, "max", onset_time->max());
		onset_times_array.push_back(onset_time_obj);
	}
	if (onset_times_array.size())
		obj["severity-effect-onsets"] = onset_times_array;

	// We need to convert the symptoms map into a json array of objects
	json::array symptoms_array;
	for (const auto& [severity, symptoms] : ref.symptoms()) {
		json::object symptom_obj;
		JsonConverter::setString(symptom_obj, "severity", DiseasePoisonSeverityType::toString(severity));
		JsonConverter::setString(symptom_obj, "symptoms", symptoms);
		symptoms_array.push_back(symptom_obj);
	}
	if (symptoms_array.size())
		obj["severity-symptoms"] = symptoms_array;

	return obj;
}

const PoisonTypeData& PoisonTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	PoisonTypeData& ref = manager_.get<PoisonTypeData>(id);

	// Set the type of poison based on the string value in the json file
	std::string type_str = JsonConverter::getString(jsonObj, "type");
	PoisonType::Type type;
	PoisonType::fromString(type_str, type);
	ref.setType(type);

	ref.setAreasAffected(JsonConverter::getString(jsonObj, "areas-affected"));

	rm::rule::table::NumberMatcherFactory number_matcher_factory{};

	// Effact onset times are stored in the json as an object with keys corresponding to the severity level and values corresponding to the effect onset times for that severity level. We need to loop through the object and add each severity
	// level and its corresponding effect onset times to the PoisonTypeData object
	json::array onset_times_array = JsonConverter::getJsonArray(jsonObj, "severity-effect-onsets");
	for (const json::value& onset_time_entry : onset_times_array) {
		if (onset_time_entry.is_object()) {
			json::object onset_time_obj = onset_time_entry.as_object();
			std::string severity_str = JsonConverter::getString(onset_time_obj, "severity");
			DiseasePoisonSeverityType::Type severity;
			DiseasePoisonSeverityType::fromString(severity_str, severity);
			int min = JsonConverter::getInt(onset_time_obj, "min");
			int max = JsonConverter::getInt(onset_time_obj, "max");
			ref.addEffectOnset(severity, number_matcher_factory.matcher(min, max));
		}
	}

	// Severity symptoms are stored in the json as an object with keys corresponding to the severity level and values corresponding to the symptoms for that severity level. We need to loop through the object and add each severity level and
	// its corresponding symptoms to the PoisonTypeData object
	json::array symptoms_array = JsonConverter::getJsonArray(jsonObj, "severity-symptoms");
	for (const json::value& symptom_entry : symptoms_array) {
		if (symptom_entry.is_object()) {
			json::object symptom_obj = symptom_entry.as_object();
			std::string severity_str = JsonConverter::getString(symptom_obj, "severity");
			DiseasePoisonSeverityType::Type severity;
			DiseasePoisonSeverityType::fromString(severity_str, severity);
			std::string symptoms = JsonConverter::getString(symptom_obj, "symptoms");
			ref.addSymptom(severity, symptoms);
		}
	}

	return ref;
}

} // namespace rm::rule::serial