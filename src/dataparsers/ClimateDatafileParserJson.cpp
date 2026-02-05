#include <ClimateData.h>
#include <ClimateDatafileParserJson.h>

void ClimateDatafileParserJson::parse() {
	std::cout << "Loading Climate data ..." << std::endl;

	// Get the climates to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		ClimateData& ref = factory().get<ClimateData>(id);
		ref.setName(name);

		std::optional<HabitatType::Temperature> temperature = HabitatType::temperature(v.second.get<std::string>("temperature"));
		if (temperature) ref.setTemperature(temperature.value());

		// Get the precipitations
		for (const auto& precipitations : v.second.get_child("precipitations")) {
			std::optional<HabitatType::Precipitation> precipitation = HabitatType::precipitation(precipitations.second.get_value<std::string>());
			if (precipitation) ref.addPrecipitation(precipitation.value());
		}
		std::cout << "\tClimate name: " << ref.name() << std::endl;

	}
	std::cout << " done" << std::endl;
}

void ClimateDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	ClimateData& game_data = factory().get<ClimateData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("temperature", HabitatType::toString(game_data.temperature()));

	// Get the container tree for the precipitations
	pt::ptree precipitations_tree{};
	for (HabitatType::Precipitation precipitation : game_data.precipitations()) {
		// Get the realm container
		pt::ptree precipitation_tree{};
		precipitation_tree.put("", HabitatType::toString(precipitation));
		precipitations_tree.push_back(std::make_pair("", precipitation_tree));
	}
	datum.push_back(std::make_pair("precipitations", precipitations_tree));
}
