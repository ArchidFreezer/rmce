#include <iostream>
#include <stdexcept>
#include "LanguageCategoryData.h"
#include "LanguageCategoryDatafileParserJson.h"


LanguageCategoryDatafileParserJson::LanguageCategoryDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "LanguageCategory", filename) {
	setRootNode("languageCategories");
}

LanguageCategoryDatafileParserJson::LanguageCategoryDatafileParserJson(GameRuleDataCache& cache) : LanguageCategoryDatafileParserJson(cache, "") {}

void LanguageCategoryDatafileParserJson::parse() {
	std::cout << "Loading Language category data ..." << std::endl;

	// Get the language categories to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", generateId(ruleDatatype(), name));

		// We create a LanguageCategoryData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
		// to the cache when we add it
		std::unique_ptr<LanguageCategoryData> datum = std::make_unique<LanguageCategoryData>(id, name);
		cache().add<LanguageCategoryData>(std::move(datum), id);
		std::cout << "\tLanguage category name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void LanguageCategoryDatafileParserJson::save(const std::string& filename) {
	// Main tree
	pt::ptree tree;

	// Array of language categories
	pt::ptree plangs;

	std::set<std::string> keys{};
	cache().keys<LanguageCategoryData>(keys);

	for (std::string b : keys) {
		try {
			LanguageCategoryData& lang_data = cache().get<LanguageCategoryData>(b);
			// Individual category
			pt::ptree plangcat;
			plangcat.put("id", lang_data.id());
			plangcat.put("name", lang_data.name());
			plangs.push_back(std::make_pair("", plangcat));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(rootNode(), plangs);

	pt::write_json(filename, tree);
}
