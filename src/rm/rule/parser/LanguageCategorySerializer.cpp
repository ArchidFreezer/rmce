#include <LanguageCategorySerializer.h>

namespace rm::rule::parser {

void LanguageCategorySerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading LanguageCategory data ..." << std::endl;

	// Get the languagecategorys to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a LanguageCategoryData object from the object manager
		LanguageCategoryData& ref = manager().get<LanguageCategoryData>(id);
		ref.setName(name);
		std::cout << "\tLanguage category name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void LanguageCategorySerializer::populateDatum(std::string key, pt::ptree& datum) {
	LanguageCategoryData& game_data = manager().get<LanguageCategoryData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
}

std::string LanguageCategorySerializer::rootNode() const {
	return "language-categories";
}

} // namespace rm::rule::parser