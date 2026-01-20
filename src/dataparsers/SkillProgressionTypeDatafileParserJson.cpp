#include <SkillProgressionTypeData.h>
#include <SkillProgressionTypeDatafileParserJson.h>

void SkillProgressionTypeDatafileParserJson::parse(bool id_only) {
	// We know there are no references in skill progression types so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading SkillProgressionTypeData data ..." << std::endl;

	// Get the books to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));
		float zero = std::stof(v.second.get<std::string>("zero"));
		float ten = std::stof(v.second.get<std::string>("ten"));
		float twenty = std::stof(v.second.get<std::string>("twenty"));
		float thirty = std::stof(v.second.get<std::string>("thirty"));
		float remaining = std::stof(v.second.get<std::string>("remaining"));

		// We create a SkillProgressionTypeData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
		// to the cache when we add it
		std::unique_ptr<SkillProgressionTypeData> datum = std::make_unique<SkillProgressionTypeData>(id);
		datum->setName(name);
		datum->setZero(zero);
		datum->setTen(ten);
		datum->setTwenty(twenty);
		datum->setThirty(thirty);
		datum->setRemaining(remaining);
		std::cout << "\tSkillProgressionTypeData name: " << datum->name() << std::endl;
		cache().add<SkillProgressionTypeData>(std::move(datum), id);

	}
	std::cout << " done" << std::endl;
}

void SkillProgressionTypeDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	SkillProgressionTypeData& game_data = cache().get<SkillProgressionTypeData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("zero", game_data.zero());
	datum.put("ten", game_data.ten());
	datum.put("twenty", game_data.twenty());
	datum.put("thirty", game_data.thirty());
	datum.put("remaining", game_data.remaining());
}
