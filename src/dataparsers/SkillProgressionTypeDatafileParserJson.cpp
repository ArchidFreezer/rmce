#include <iostream>
#include <stdexcept>
#include "SkillProgressionTypeData.h"
#include "SkillProgressionTypeDatafileParserJson.h"

SkillProgressionTypeDatafileParserJson::SkillProgressionTypeDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "SkillProgressionType", filename) {
	setRootNode("skillProgressions");
}

SkillProgressionTypeDatafileParserJson::SkillProgressionTypeDatafileParserJson(GameRuleDataCache& cache) : SkillProgressionTypeDatafileParserJson(cache, "") {}

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

void SkillProgressionTypeDatafileParserJson::save(const std::string& filename) {
	if (filename.empty()) return;

	// Main tree
	pt::ptree tree;

	// Tree of progressions
	pt::ptree progressions;

	std::set<std::string> keys{};
	cache().keys<SkillProgressionTypeData>(keys);

	for (std::string key : keys) {
		try {
			SkillProgressionTypeData& progression_data = cache().get<SkillProgressionTypeData>(key);
			// Individual book
			pt::ptree datum;
			datum.put("id", progression_data.id());
			datum.put("name", progression_data.name());
			datum.put("zero", progression_data.zero());
			datum.put("ten", progression_data.ten());
			datum.put("twenty", progression_data.twenty());
			datum.put("thirty", progression_data.thirty());
			datum.put("remaining", progression_data.remaining());
			progressions.push_back(std::make_pair("", datum));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(rootNode(), progressions);

	pt::write_json(filename, tree);
}