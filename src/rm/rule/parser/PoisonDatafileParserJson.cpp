#include <PoisonData.h>
#include <PoisonDatafileParserJson.h>
#include <PoisonTypeData.h>

namespace rm::rule::parser {

	void PoisonDatafileParserJson::parse() {
		std::cout << "Loading Poison data ..." << std::endl;

		// Get the poisons to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

			PoisonData& ref = factory().get<PoisonData>(id);

			ref.setName(name);

			// Set the type of pioson based on the string value in the json file
			std::string type_str = v.second.get<std::string>("type");
			ref.setType(factory().get<PoisonTypeData>(type_str));

			// Set the average level of the poison
			ref.setAverageLevel(v.second.get<int>("level"));

			// Get the level variance type based on the string value in the json file and set it
			rule::enums::LevelVarianceType::Type level_variance_type{};
			rule::enums::LevelVarianceType::fromString(v.second.get<std::string>("level-variance"), level_variance_type);
			ref.setLevelVarianceType(level_variance_type);


			std::cout << "\tPoison name: " << ref.name() << std::endl;

		}
		std::cout << " done" << std::endl;
	}

	void PoisonDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		PoisonData& game_data = factory().get<PoisonData>(id);
		datum.put("id", game_data.id());
		datum.put("name", game_data.name());
		datum.put("type", game_data.type().id());
		datum.put("level", game_data.averageLevel());
		datum.put("level-variance", rule::enums::LevelVarianceType::toString(game_data.levelVarianceType()));
	}

} // namespace rm::rule::parser