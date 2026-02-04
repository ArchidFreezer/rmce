#include <SkillGroupDatafileParserJson.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>

void SkillGroupDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SkillGroupData& game_data = factory().get<SkillGroupData>(id);

	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
}

void SkillGroupDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading SkillGroup data ... " << std::endl;

	// Loop through the groups
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		SkillGroupData& datum = factory().get<SkillGroupData>(id);
		datum.setName(name);

		std::cout << "\tSkillGroup name: " << datum.name() << std::endl;

		}
}