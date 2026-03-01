#include <SkillProgressionTypeData.h>
#include <SkillProgressionTypeDatafileParserJson.h>

namespace rm::rule::parser {

	void SkillProgressionTypeDatafileParserJson::parse() {
		std::cout << "Loading SkillProgressionTypeData data ..." << std::endl;

		// Get the books to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", factory().generateId(ruleDatatype(), name));

			SkillProgressionTypeData& datum = factory().get<SkillProgressionTypeData>(id);
			datum.setName(name);
			datum.setZero(std::stof(v.second.get<std::string>("zero")));
			datum.setTen(std::stof(v.second.get<std::string>("ten")));
			datum.setTwenty(std::stof(v.second.get<std::string>("twenty")));
			datum.setThirty(std::stof(v.second.get<std::string>("thirty")));
			datum.setRemaining(std::stof(v.second.get<std::string>("remaining")));
			std::cout << "\tSkillProgressionTypeData name: " << datum.name() << std::endl;

		}
		std::cout << " done" << std::endl;
	}

	void SkillProgressionTypeDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		SkillProgressionTypeData& game_data = factory().get<SkillProgressionTypeData>(id);
		datum.put("id", game_data.id());
		datum.put("name", game_data.name());
		datum.put("zero", game_data.zero());
		datum.put("ten", game_data.ten());
		datum.put("twenty", game_data.twenty());
		datum.put("thirty", game_data.thirty());
		datum.put("remaining", game_data.remaining());
	}

} // namespace rm::rule::parser