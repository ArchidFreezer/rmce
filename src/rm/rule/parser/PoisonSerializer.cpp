#include <PoisonSerializer.h>

namespace rm::rule::parser {

void PoisonSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading Poison data ..." << std::endl;

	// Get the poisons to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a PoisonData object from the object manager
		PoisonData& ref = manager().get<PoisonData>(id);
		ref.setName(name);

		// Set the type of pioson based on the string value in the json file
		std::string type_str = v.second.get<std::string>("type");
		ref.setType(manager().get<PoisonTypeData>(type_str));

		// Set the average level of the poison
		ref.setAverageLevel(v.second.get<int>("level"));

		// Get the level variance type based on the string value in the json file and set it
		LevelVarianceType::Type level_variance_type{};
		LevelVarianceType::fromString(v.second.get<std::string>("level-variance"), level_variance_type);
		ref.setLevelVarianceType(level_variance_type);

		std::cout << "\tPoison name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void PoisonSerializer::populateDatum(std::string key, pt::ptree& datum) {
	PoisonData& game_data = manager().get<PoisonData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("type", game_data.type().id());
	datum.put("level", game_data.averageLevel());
	datum.put("level-variance", LevelVarianceType::toString(game_data.levelVarianceType()));
}

std::string PoisonSerializer::rootNode() const {
	return "poisons";
}

} // namespace rm::rule::parser