#include <iostream>
#include <stdexcept>
#include "LanguageData.h"
#include "LanguageDatafileParserJson.h"


LanguageDatafileParserJson::LanguageDatafileParserJson(GameRuleDataCache& cache) : DatafileParserJson(cache, "Language") {
	root_node_ = "languages";
}

void LanguageDatafileParserJson::parse() {
	std::cout << "Loading Language data ..." << std::endl;

	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", generateId(ruleDatatype(), name));
		std::string category = v.second.get<std::string>("category");
		bool isSpoken = v.second.get<bool>("isSpoken");
		bool isWritten = v.second.get<bool>("isWritten");
		bool isSomantic = v.second.get<bool>("isSomantic");

		std::unique_ptr<LanguageData> datum = std::make_unique<LanguageData>(id, name, category, isSpoken, isWritten, isSomantic);
		cache().add<LanguageData>(std::move(datum), id);
		std::cout << "\tLanguage name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void LanguageDatafileParserJson::save(const std::string& filename) {
	// Main tree
	pt::ptree tree;

	// Array of books
	pt::ptree plangs;

	for (std::string b : cache().keys<LanguageData>()) {
		try {
			LanguageData& language_data = cache().get<LanguageData>(b);
			// Individual language
			pt::ptree datum;
			datum.put("id", language_data.id());
			datum.put("name", language_data.name());
			datum.put("category", language_data.category());
			datum.put("isSpoken", language_data.isSpoken());
			datum.put("isWritten", language_data.isWritten());
			datum.put("isSomantic", language_data.isSomantic());
			plangs.push_back(std::make_pair("", datum));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(rootNode(), plangs);

	pt::write_json(filename, tree);
}
