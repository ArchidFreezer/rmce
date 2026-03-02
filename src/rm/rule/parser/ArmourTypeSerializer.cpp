#include <ArmourTypeSerializer.h>

namespace rm::rule::parser {

void ArmourTypeSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading ArmourType data ..." << std::endl;

	// Get the armourtypes to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get an ArmourTypeData object from the object manager
		ArmourTypeData& datum = manager().get<ArmourTypeData>(id);
		datum.setName(name);
		datum.setDescription(v.second.get<std::string>("description"));
		datum.setMinManoeuvreMod(v.second.get<int>("min-manoeuvre-mod"));
		datum.setMaxManoeuvreMod(v.second.get<int>("max-manoeuvre-mod"));
		datum.setMissileAttackPenalty(v.second.get<int>("missile-attack-penalty"));
		datum.setQuicknessPenalty(v.second.get<int>("quickness-penalty"));
		datum.setAnimalOnly(v.second.get<bool>("animal-only"));
		datum.setIncludesGreaves(v.second.get<bool>("includes-greaves"));

		// Attempt to find the ArmourType
		std::string type_name = v.second.get<std::string>("type");
		ArmourType::Type type;
		if (ArmourType::fromString(type_name)) {
			datum.setType(ArmourType::fromString(type_name).value());
		} else {
			throw ArmourType::ArmourTypeNotFoundException("Could not find a skill group for: " + type_name);
		}

		std::cout << "\tArmourType name: " << datum.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void ArmourTypeSerializer::populateDatum(std::string key, pt::ptree& datum) {
	ArmourTypeData& game_data = manager().get<ArmourTypeData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("type", toString(game_data.type()));
	datum.put("description", game_data.description());
	datum.put("min-manoeuvre-mod", game_data.minManoeuvreMod());
	datum.put("max-manoeuvre-mod", game_data.maxManoeuvreMod());
	datum.put("missile-attack-penalty", game_data.missileAttackPenalty());
	datum.put("quickness-penalty", game_data.quicknessPenalty());
	datum.put("animal-only", game_data.animalOnly());
	datum.put("includes-greaves", game_data.includesGreaves());
}

std::string ArmourTypeSerializer::rootNode() const {
	return "armour-types";
}

} // namespace rm::rule::parser