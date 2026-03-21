#include <DiseaseTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value DiseaseTypeSerializer::serializeObject(const DiseaseTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "type", DiseaseType::toString(ref.type()));
	JsonConverter::setString(obj, "transmission", ref.transmission());
	JsonConverter::setString(obj, "description", ref.description());

	// We need to convert the symptoms map into a json array of objects
	json::array symptoms_array;
	for (const auto& [severity, symptoms] : ref.symptoms()) {
		json::object symptom_obj;
		JsonConverter::setString(symptom_obj, "severity", DiseasePoisonSeverityType::toString(severity));
		JsonConverter::setString(symptom_obj, "symptoms", symptoms);
		symptoms_array.push_back(symptom_obj);
	}
	if (symptoms_array.size())
		obj["severitySymptoms"] = symptoms_array;

	return obj;
}

const DiseaseTypeData& DiseaseTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	DiseaseTypeData& ref = manager_.get<DiseaseTypeData>(id);

	// Set the type of poison based on the string value in the json file
	std::string type_str = JsonConverter::getString(jsonObj, "type");
	DiseaseType::Type type;
	DiseaseType::fromString(type_str, type);
	ref.setType(type);

	ref.setTransmission(JsonConverter::getString(jsonObj, "transmission", ""));
	ref.setDescription(JsonConverter::getString(jsonObj, "description", ""));

	// Severity symptoms are stored in the json as an object with keys corresponding to the severity level and values corresponding to the symptoms for that severity level. We need to loop through the object and add each severity level and
	// its corresponding symptoms to the DiseaseTypeData object
	json::array symptoms_array = JsonConverter::getJsonArray(jsonObj, "severitySymptoms");
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