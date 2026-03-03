#include <DiseaseTypeSerializer.h>

namespace rm::rule::parser {

void DiseaseTypeSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading DiseaseType data ..." << std::endl;

	// Get the diseasetypes to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string type_str = v.second.get<std::string>("type");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), type_str));

		DiseaseTypeData& ref = manager().get<DiseaseTypeData>(id);

		// Set the type of poson based on the string value in the json file
		DiseaseType::Type type;
		DiseaseType::fromString(type_str, type);
		ref.setType(type);

		ref.setTransmission(v.second.get<std::string>("transmission"));
		ref.setDescription(v.second.get<std::string>("description"));

		// Severity symptoms
		for (auto& severity_symptoms : v.second.get_child("severity-symptoms")) {
			std::string severity_str = severity_symptoms.second.get<std::string>("severity");
			DiseasePoisonSeverityType::Type severity;
			DiseasePoisonSeverityType::fromString(severity_str, severity);
			std::string symptoms = severity_symptoms.second.get<std::string>("symptoms");
			ref.addSymptom(severity, symptoms);
		}

		std::cout << "\tDiseaseType name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void DiseaseTypeSerializer::populateDatum(std::string key, pt::ptree& datum) {
	DiseaseTypeData& game_data = manager().get<DiseaseTypeData>(key);
	datum.put("id", game_data.id());
	datum.put("type", DiseaseType::toString(game_data.type()));
	datum.put("transmission", game_data.transmission());
	datum.put("description", game_data.description());

	// Severity symptoms
	{
		pt::ptree tree{};
		for (const auto& severity_symptoms : game_data.symptoms()) {
			pt::ptree severity_symptoms_node;
			severity_symptoms_node.put("severity", DiseasePoisonSeverityType::toString(severity_symptoms.first));
			severity_symptoms_node.put("symptoms", severity_symptoms.second);
			tree.push_back(std::make_pair("", severity_symptoms_node));
		}
		if (tree.size())
			datum.push_back(std::make_pair("severity-symptoms", tree));
	}
}

std::string DiseaseTypeSerializer::rootNode() const {
	return "disease-types";
}

} // namespace rm::rule::parser