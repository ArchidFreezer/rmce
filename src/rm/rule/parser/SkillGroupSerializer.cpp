#include <SkillGroupSerializer.h>

namespace rm::rule::parser {

void SkillGroupSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading SkillGroup data ..." << std::endl;

	// Get the skillgroups to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a SkillGroupData object from the object manager
		SkillGroupData& ref = manager().get<SkillGroupData>(id);
		ref.setName(name);

		std::cout << "\tSkillGroup name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void SkillGroupSerializer::populateDatum(std::string key, pt::ptree& datum) {
	SkillGroupData& game_data = manager().get<SkillGroupData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
}

std::string SkillGroupSerializer::rootNode() const {
	return "skill-groups";
}

} // namespace rm::rule::parser