#include <RealmType.h>
#include <SkillCategoryData.h>
#include <SkillCategoryDatafileParserXml.h>
#include <SkillProgressionTypeData.h>

void SkillCategoryDatafileParserXml::parse() {
	std::cout << "Loading SkillCategory data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string xml_name = v.second.get<std::string>("name");
		std::string category_name{ parseName(xml_name) };
		std::string group_name{ v.second.get<std::string>("group") };
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), xml_name));

		SkillCategoryData& ref = factory().get<SkillCategoryData>(id);
		ref.setName(category_name);

		// Attempt to find the SkillGroupType
//			if (SkillGroupType::fromString(group_name)) {
//				ref.setGroup(SkillGroupType::fromString(group_name).value());
//			} else {
//				throw SkillGroupType::SkillGroupNotFoundException("Could not find a skill group for: " + group_name);
//			}

		bool use_realm_stats = v.second.get<bool>("use-realm-stat");
		ref.setUseRealmStats(use_realm_stats);

		std::string skill_progression_xml = v.second.get<std::string>("progression");
		// Power Point and Body Development progressions are based on the race so are character specific. Setting them to None
		// means that this will be spotted down the line if the character skill is not updated on creation.
		if (skill_progression_xml == "Racial") skill_progression_xml = "None";
		std::string skill_progression_id = GameRuleData::generateId("SkillProgressionType", skill_progression_xml);
		std::string category_progression_xml{ skill_progression_xml == "Standard" ? "Standard" : "None"};
		std::string category_progression_id = GameRuleData::generateId("SkillProgressionType", category_progression_xml);
		ref.setSkillProgressions(factory().get<SkillProgressionTypeData>(skill_progression_id), factory().get<SkillProgressionTypeData>(category_progression_id));

		if (!use_realm_stats) {
			if (boost::optional<const pt::ptree&> statTree = v.second.get_child_optional("stats")) {
				for (const auto& stat : statTree.get()) {
					if (StatType::fromString(stat.second.data())) {
						ref.addStat(StatType::fromString(stat.second.data()).value());
					}
				}
			}
		}

		std::cout << "\tSkillCategory name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

std::string SkillCategoryDatafileParserXml::parseName(const std::string& name) {
	auto pos = name.find(" | ");
	if (pos != std::string::npos)
		return name.substr(pos + 3, name.length());
	else
		return name;
}