#include <iostream>
#include <stdexcept>
#include "LanguageData.h"
#include "LanguageDatafileParserJson.h"


LanguageDatafileParserJson::LanguageDatafileParserJson(GameRuleDataCache& cache) : DatafileParserJson(cache, "Language") {
	root_node_ = "languages";
}

void LanguageDatafileParserJson::parse() {
	std::cout << "Loading Language data ..." << std::endl;

	const pt::ptree& tree = ptree_.get_child(root_node_);
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", getID(getDataType(), name));
		std::string category = v.second.get<std::string>("category");
		bool is_spoken = v.second.get<bool>("isSpoken");
		bool is_written = v.second.get<bool>("isWritten");
		bool is_somantic = v.second.get<bool>("isSomantic");

		std::unique_ptr<LanguageData> datum = std::make_unique<LanguageData>(id, name, category, is_spoken, is_written, is_somantic);
		cache_.AddRuleData<LanguageData>(std::move(datum), id);
		std::cout << "\tLanguage name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void LanguageDatafileParserJson::save(const std::string& filename) {
	// Main tree
	pt::ptree tree;

	// Array of books
	pt::ptree plangs;

	for (std::string b : cache_.GetRuleDataIds<LanguageData>()) {
		try {
			LanguageData& language_data = cache_.GetRuleData<LanguageData>(b);
			// Individual language
			pt::ptree datum;
			datum.put("id", language_data.getID());
			datum.put("name", language_data.getName());
			datum.put("category", language_data.getCategory());
			datum.put("isSpoken", language_data.getIsSpoken());
			datum.put("isWritten", language_data.getIsWritten());
			datum.put("isSomantic", language_data.getIsSomantic());
			plangs.push_back(std::make_pair("", datum));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(root_node_, plangs);

	pt::write_json(filename, tree);
}
