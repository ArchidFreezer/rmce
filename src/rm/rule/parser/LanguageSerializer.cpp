#include <LanguageSerializer.h>

namespace rm::rule::parser {

void LanguageSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading Language data ..." << std::endl;

	// Get the languages to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a LanguageData object from the object manager
		LanguageData& ref = manager().get<LanguageData>(id);

		std::string base_language = v.second.get<std::string>("baseLanguage", name);
		ref.setBaseLanguage(base_language);
		ref.setCategory(v.second.get<std::string>("category"));
		ref.setIsSomantic(v.second.get<bool>("isSomantic"));
		ref.setIsSpoken(v.second.get<bool>("isSpoken"));
		ref.setIsWritten(v.second.get<bool>("isWritten"));
		ref.setName(name);
		std::cout << "\tLanguage name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void LanguageSerializer::populateDatum(std::string key, pt::ptree& datum) {
	LanguageData& game_data = manager().get<LanguageData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("category", game_data.category());
	if (game_data.baseLanguage() != game_data.name()) {
		datum.put("baseLanguage", game_data.baseLanguage());
	}
	datum.put("isSpoken", game_data.isSpoken());
	datum.put("isWritten", game_data.isWritten());
	datum.put("isSomantic", game_data.isSomantic());
}

std::string LanguageSerializer::rootNode() const {
	return "languages";
}

} // namespace rm::rule::parser