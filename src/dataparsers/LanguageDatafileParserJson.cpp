#include <LanguageData.h>
#include <LanguageDatafileParserJson.h>
#include <LanguageDialectData.h>

void LanguageDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading Language data ..." << std::endl;

	// Get the languages to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		LanguageData& ref = factory().get<LanguageData>(id);

		std::string base_language = v.second.get<std::string>("baseLanguage", name);
		ref.setBaseLanguage(base_language);
		ref.setCategory(v.second.get<std::string>("category"));
		ref.setIsSomantic(v.second.get<bool>("isSomantic"));
		ref.setIsSpoken(v.second.get<bool>("isSpoken"));
		ref.setIsWritten(v.second.get<bool>("isWritten"));
		ref.setName(name);
		std::cout << "\tLanguage name: " << ref.name() << std::endl;

		// Add the language to the appropriate dialect collection
		factory().get<LanguageDialectData>(base_language).add(ref);

	}
	std::cout << " done" << std::endl;
}

void LanguageDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	LanguageData& game_data = factory().get<LanguageData>(id);
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
