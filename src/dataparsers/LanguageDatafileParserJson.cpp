#include <LanguageData.h>
#include <LanguageDatafileParserJson.h>
#include <LanguageDialectData.h>


LanguageDatafileParserJson::LanguageDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Language", filename) {
	setRootNode("languages");
}

LanguageDatafileParserJson::LanguageDatafileParserJson(GameRuleDataCache& cache) : LanguageDatafileParserJson(cache, "") {}

void LanguageDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading Language data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	// Get the languages to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		if (id_only) {
			// We create a LanguageData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<LanguageData> datum = std::make_unique<LanguageData>(id);
			cache().add<LanguageData>(std::move(datum), id);
		} else {
			std::string base_language = v.second.get<std::string>("baseLanguage", name);

			LanguageData& ref = cache().get<LanguageData>(id);
			ref.setBaseLanguage(base_language);
			ref.setCategory(v.second.get<std::string>("category"));
			ref.setIsSomantic(v.second.get<bool>("isSomantic"));
			ref.setIsSpoken(v.second.get<bool>("isSpoken"));
			ref.setIsWritten(v.second.get<bool>("isWritten"));
			ref.setName(name);
			std::cout << "\tLanguage name: " << ref.name() << std::endl;

			// Add the language to the appropriate dialect collection
			if (cache().exists<LanguageDialectData>(base_language)) {
				std::cout << "\t\tExisting dialect: " << base_language << std::endl;
				cache().get<LanguageDialectData>(base_language).add(ref);
			} else {
				std::cout << "\t\tNew dialect: " << base_language << std::endl;
				std::unique_ptr<LanguageDialectData> dialects = std::make_unique<LanguageDialectData>(base_language);
				dialects->add(ref);
				cache().add<LanguageDialectData>(std::move(dialects), base_language);
			}
		}

	}
	std::cout << " done" << std::endl;
}

void LanguageDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	LanguageData& game_data = cache().get<LanguageData>(id);
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
