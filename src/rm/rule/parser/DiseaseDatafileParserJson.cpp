#include <DiseaseData.h>
#include <DiseaseDatafileParserJson.h>
#include <DiseaseTypeData.h>

void DiseaseDatafileParserJson::parse() {
	std::cout << "Loading Disease data ..." << std::endl;

	// Get the diseases to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		DiseaseData& ref = factory().get<DiseaseData>(id);

		ref.setName(name);

		// Set the type of pioson based on the string value in the json file
		std::string type_str = v.second.get<std::string>("type");
		ref.setType(factory().get<DiseaseTypeData>(type_str));

		// Set the average level of the disease
		ref.setAverageLevel(v.second.get<int>("level"));

		// Get the level variance type based on the string value in the json file and set it
		LevelVarianceType::Type level_variance_type{};
		LevelVarianceType::fromString(v.second.get<std::string>("level-variance"), level_variance_type);
		ref.setLevelVarianceType(level_variance_type);


		std::cout << "\tDisease name: " << ref.name() << std::endl;

	}
	std::cout << " done" << std::endl;
}

void DiseaseDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	DiseaseData& game_data = factory().get<DiseaseData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("type", game_data.type().id());
	datum.put("level", game_data.averageLevel());
	datum.put("level-variance", LevelVarianceType::toString(game_data.levelVarianceType()));
}
