#include <iostream>
#include <stdexcept>
#include "LanguageCategoryData.h"
#include "LanguageCategoryDatafileParserJson.h"


LanguageCategoryDatafileParserJson::LanguageCategoryDatafileParserJson(GameRuleDataCache& cache) : DatafileParserJson(cache, "LanguageCategory") {
	root_node_ = "languageCategories";
}

void LanguageCategoryDatafileParserJson::parse() {
	std::cout << "Loading Language category data ..." << std::endl;

	const pt::ptree& tree = ptree_.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", generateId(get_data_type(), name));

		std::unique_ptr<LanguageCategoryData> datum = std::make_unique<LanguageCategoryData>(id, name);
		cache_.add<LanguageCategoryData>(std::move(datum), id);
		std::cout << "\tLanguage category name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void LanguageCategoryDatafileParserJson::save(const std::string& filename) {
	// Main tree
	pt::ptree tree;

	// Array of language categories
	pt::ptree plangs;

	for (std::string b : cache_.keys<LanguageCategoryData>()) {
		try {
			LanguageCategoryData& lang_data = cache_.get<LanguageCategoryData>(b);
			// Individual book
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
