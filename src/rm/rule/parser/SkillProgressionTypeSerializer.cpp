#include <SkillProgressionTypeSerializer.h>

namespace rm::rule::parser {

void SkillProgressionTypeSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading SkillProgressionType data ..." << std::endl;

	// Get the skillprogressiontypes to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a SkillProgressionTypeData object from the object manager
		SkillProgressionTypeData& ref = manager().get<SkillProgressionTypeData>(id);
		ref.setName(name);
		ref.setZero(std::stof(v.second.get<std::string>("zero")));
		ref.setTen(std::stof(v.second.get<std::string>("ten")));
		ref.setTwenty(std::stof(v.second.get<std::string>("twenty")));
		ref.setThirty(std::stof(v.second.get<std::string>("thirty")));
		ref.setRemaining(std::stof(v.second.get<std::string>("remaining")));

		std::cout << "\tSkillProgressionType name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void SkillProgressionTypeSerializer::populateDatum(std::string key, pt::ptree& datum) {
	SkillProgressionTypeData& game_data = manager().get<SkillProgressionTypeData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("zero", game_data.zero());
	datum.put("ten", game_data.ten());
	datum.put("twenty", game_data.twenty());
	datum.put("thirty", game_data.thirty());
	datum.put("remaining", game_data.remaining());
}

std::string SkillProgressionTypeSerializer::rootNode() const {
	return "skill-progression-types";
}

} // namespace rm::rule::parser