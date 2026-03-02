#include <CreaturePaceSerializer.h>

namespace rm::rule::parser {

void CreaturePaceSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading CreaturePace data ..." << std::endl;

	// Get the creaturepaces to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a CreaturePaceData object from the object manager
		CreaturePaceData& ref = manager().get<CreaturePaceData>(id);
		ref.setName(name);

		ref.setExhaustionCostMultiplier(v.second.get<float>("exhaustion-multiplier"));
		ref.setMovementRateMultiplier(v.second.get<float>("movement-multiplier"));

		ManoeuvreDifficultyType::Type manoeuvre_difficulty{};
		fromString(v.second.get<std::string>("manoeuvre-difficulty"), manoeuvre_difficulty);
		ref.setManoeuvreDifficultyType(manoeuvre_difficulty);

		std::cout << "\tCreaturePace name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void CreaturePaceSerializer::populateDatum(std::string key, pt::ptree& datum) {
	CreaturePaceData& game_data = manager().get<CreaturePaceData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("exhaustion-multiplier", game_data.exhaustionCostMultiplier());
	datum.put("movement-multiplier", game_data.movementRateMultiplier());
	datum.put("manoeuvre-difficulty", ManoeuvreDifficultyType::toString(game_data.manoeuvreDifficultyType()));
}

std::string CreaturePaceSerializer::rootNode() const {
	return "creature-paces";
}

} // namespace rm::rule::parser