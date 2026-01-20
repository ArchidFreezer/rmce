#include <LanguageCategoryData.h>
#include <LanguageCategoryDatafileParserJson.h>


LanguageCategoryDatafileParserJson::LanguageCategoryDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "LanguageCategory", filename) {
	setRootNode("languageCategories");
}

LanguageCategoryDatafileParserJson::LanguageCategoryDatafileParserJson(GameRuleDataCache& cache) : LanguageCategoryDatafileParserJson(cache, "") {}

void LanguageCategoryDatafileParserJson::parse(bool id_only) {
	// We know there are no references in language catagories so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading Language category data ..." << std::endl;

	// Get the language categories to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		// We create a LanguageCategoryData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
		// to the cache when we add it
		std::unique_ptr<LanguageCategoryData> datum = std::make_unique<LanguageCategoryData>(id);
		datum->setName(name);
		std::cout << "\tLanguage category name: " << datum->name() << std::endl;
		cache().add<LanguageCategoryData>(std::move(datum), id);
	}
	std::cout << " done" << std::endl;
}

void LanguageCategoryDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	LanguageCategoryData& game_data = cache().get<LanguageCategoryData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
}
