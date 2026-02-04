#include <RealmType.h>
#include <SpellListData.h>
#include <SpellListDatafileParserXml.h>
#include <SpellListType.h>


void SpellListDatafileParserXml::parse() {
	std::cout << "Loading SpellListData data ..." << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));
		std::optional<RealmType::Type> realm = RealmType::fromString(v.second.get<std::string>("realm"));
		std::optional<SpellListType::Type> type = SpellListType::fromString(v.second.get<std::string>("type"));

		SpellListData& datum = factory().get<SpellListData>(id);
		datum.setName(name);
		if (type) datum.setType(type.value());
		datum.setIsEvil(v.second.get<bool>("evil"));
		datum.setIsSummoning(v.second.get<bool>("summoning"));
		std::cout << "\tSpell List name: " << datum.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}