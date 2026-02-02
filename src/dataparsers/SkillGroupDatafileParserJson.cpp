#include <SkillGroupDatafileParserJson.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>

void SkillGroupDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SkillGroupData& game_data = cache().get<SkillGroupData>(id);

	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
}

void SkillGroupDatafileParserJson::parse(bool id_only) {
	// We know there are no references so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading SkillGroup data ... " << std::endl;

	// Loop through the groups
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		std::unique_ptr<SkillGroupData> datum = std::make_unique<SkillGroupData>(id, name);
		cache().add<SkillGroupData>(std::move(datum), id);
		SkillGroupData& ref = cache().get<SkillGroupData>(id);

		std::cout << "\tSkillGroup name: " << ref.name() << std::endl;

		}
}