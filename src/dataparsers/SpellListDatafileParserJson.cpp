#include <RealmType.h>
#include <SpellListData.h>
#include <SpellListDatafileParserJson.h>
#include <SpellListType.h>

void SpellListDatafileParserJson::parse(bool id_only) {
	// We know there are no references in spell lists so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading Spell List data ..." << std::endl;

	// Get the books to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));
		std::optional<RealmType::Type> realm = RealmType::fromString(v.second.get<std::string>("realm"));
		std::optional<SpellListType::Type> type = SpellListType::fromString(v.second.get<std::string>("type"));

		// We create a SpellListData object and reference it with as a unique_ptr to allow us to use move semantics to transfer
		// ownership to the cache when we add it
		std::unique_ptr<SpellListData> datum = std::make_unique<SpellListData>(id);
		datum->setName(name);
		if (realm) datum->setRealm(realm.value());
		if (type) datum->setType(type.value());
		datum->setIsEvil(v.second.get<bool>("evil"));
		datum->setIsSummoning(v.second.get<bool>("summoning"));
		std::cout << "\tSpell List name: " << datum->name() << std::endl;
		cache().add<SpellListData>(std::move(datum), id);

	}
	std::cout << " done" << std::endl;

}

void SpellListDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SpellListData& game_data = cache().get<SpellListData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("realm", RealmType::toString(game_data.realm()));
	datum.put("type", SpellListType::toString(game_data.type()));
	datum.put("evil", game_data.isEvil());
	datum.put("summoning", game_data.isSummoning());
}
