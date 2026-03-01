#include <LanguageCategoryData.h>
#include <LanguageCategoryDatafileParserJson.h>

namespace rm::rule::parser {

	void LanguageCategoryDatafileParserJson::parse() {
		std::cout << "Loading Language category data ..." << std::endl;

		// Get the language categories to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", factory().generateId(ruleDatatype(), name));

			LanguageCategoryData& datum = factory().get<LanguageCategoryData>(id);
			datum.setName(name);
			std::cout << "\tLanguage category name: " << datum.name() << std::endl;
		}
		std::cout << " done" << std::endl;
	}

	void LanguageCategoryDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		LanguageCategoryData& game_data = factory().get<LanguageCategoryData>(id);
		datum.put("id", game_data.id());
		datum.put("name", game_data.name());
	}

} // namespace rm::rule::parser