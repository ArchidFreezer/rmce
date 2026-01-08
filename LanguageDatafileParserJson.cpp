#include <iostream>
#include <stdexcept>
#include "LanguageData.h"
#include "LanguageDatafileParserJson.h"
#include "LanguageDialectData.h"


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
		std::string base_language = v.second.get<std::string>("baseLanguage", name);
		bool is_spoken = v.second.get<bool>("isSpoken");
		bool is_written = v.second.get<bool>("isWritten");
		bool is_somantic = v.second.get<bool>("isSomantic");

		std::cout << "\tLanguage name: " << name << std::endl;

		// Process the language object
		std::unique_ptr<LanguageData> datum = std::make_unique<LanguageData>(id, name, category, base_language, is_spoken, is_written, is_somantic);
		cache().add<LanguageData>(std::move(datum), id);

		// Add the language to the appropriate dialect collection
		if (cache().exists<LanguageDialectData>(base_language)) {
			std::cout << "\t\tExisting dialect: " << base_language << std::endl;
			cache().get<LanguageDialectData>(base_language).add(id);
		} else {
			std::cout << "\t\tNew dialect: " << base_language << std::endl;
			std::unique_ptr<LanguageDialectData> dialects = std::make_unique<LanguageDialectData>(base_language);
			cache().add<LanguageDialectData>(std::move(dialects), base_language);
		}
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
			if (language_data.baseLanguage() != language_data.name()) {
				datum.put("baseLanguage", language_data.baseLanguage());
			}
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
