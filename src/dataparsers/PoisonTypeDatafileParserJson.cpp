#include <NumberMatcherFactory.h>
#include <PoisonTypeData.h>
#include <PoisonTypeDatafileParserJson.h>

void PoisonTypeDatafileParserJson::parse() {
	std::cout << "Loading PoisonType data ..." << std::endl;

	NumberMatcherFactory numberMatcherFactory{};

	// Get the poisontypes to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string type_str = v.second.get<std::string>("type");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), type_str));

		PoisonTypeData& ref = factory().get<PoisonTypeData>(id);

		// Set the type of poson based on the string value in the json file
		PoisonType::Type type;
		PoisonType::fromString(type_str, type);
		ref.setType(type);

		ref.setAreasAffected(v.second.get<std::string>("areas-affected"));

		// Effect onset times
		for (auto& severity_effect_onset : v.second.get_child("severity-effect-onsets")) {
			std::string severity_str = severity_effect_onset.second.get<std::string>("severity");
			DiseasePoisonSeverityType::Type severity;
			DiseasePoisonSeverityType::fromString(severity_str, severity);
			int min = severity_effect_onset.second.get<int>("min");
			int max = severity_effect_onset.second.get<int>("max");
			ref.addEffectOnset(severity, numberMatcherFactory.matcher(min, max));
		}

		// Severity symptoms
		for (auto& severity_symptoms : v.second.get_child("severity-symptoms")) {
			std::string severity_str = severity_symptoms.second.get<std::string>("severity");
			DiseasePoisonSeverityType::Type severity;
			DiseasePoisonSeverityType::fromString(severity_str, severity);
			std::string symptoms = severity_symptoms.second.get<std::string>("symptoms");
			ref.addSymptom(severity, symptoms);
		}

		std::cout << "\tPoisonType name: " << ref.name() << std::endl;

	}
	std::cout << " done" << std::endl;
}

void PoisonTypeDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	PoisonTypeData& game_data = factory().get<PoisonTypeData>(id);
	datum.put("id", game_data.id());
	datum.put("type", PoisonType::toString(game_data.type()));
	datum.put("areas-affected", game_data.areasAffected());

	// Effect onset times
	{
		pt::ptree tree{};

		for (const auto& severity_effect_onset : game_data.effectOnsets()) {
			pt::ptree severity_effect_onset_node;
			severity_effect_onset_node.put("severity", DiseasePoisonSeverityType::toString(severity_effect_onset.first));
			severity_effect_onset_node.put("min", severity_effect_onset.second->min());
			severity_effect_onset_node.put("max", severity_effect_onset.second->max());
			tree.push_back(std::make_pair("", severity_effect_onset_node));
		}

		if (tree.size()) datum.push_back(std::make_pair("severity-effect-onsets", tree));
	}

	// Severity symptoms
	{
		pt::ptree tree{};
		for (const auto& severity_symptoms : game_data.symptoms()) {
			pt::ptree severity_symptoms_node;
			severity_symptoms_node.put("severity", DiseasePoisonSeverityType::toString(severity_symptoms.first));
			severity_symptoms_node.put("symptoms", severity_symptoms.second);
			tree.push_back(std::make_pair("", severity_symptoms_node));
		}
		if (tree.size()) datum.push_back(std::make_pair("severity-symptoms", tree));
	}	
}
