#include <iostream>
#include <stdexcept>
#include "LanguageCategoryDatafileParser.h"


LanguageCategoryDatafileParser::LanguageCategoryDatafileParser(GameRuleDataCache& cache, FileFormat const filetype) : DatafileParser(cache, filetype) {
	datatype_ = "LanguageCategory";
}

LanguageCategoryDatafileParser::LanguageCategoryDatafileParser(GameRuleDataCache& cache) : LanguageCategoryDatafileParser(cache, DatafileParser::kJson) {}

void LanguageCategoryDatafileParser::parse() {
	std::cout << "Loading Language category data ..." << std::endl;

	std::string rootNode{};
	switch (filetype_) {
	case DatafileParser::kXml:
	{
		rootNode = "LanguageCategoryData.books";
		break;
	}
	case DatafileParser::kJson:
		rootNode = "languageCategories";
		break;
	}

	const pt::ptree& tree = ptree_.get_child(rootNode);
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", getID(getDataType(), name));

		std::unique_ptr<LanguageCategoryData> datum = std::make_unique<LanguageCategoryData>(id, name);
		cache_.AddRuleData<LanguageCategoryData>(std::move(datum), id);
		std::cout << "\tLanguage category name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void LanguageCategoryDatafileParser::saveJson(const std::string& filename) {
	// Main tree
	pt::ptree tree;

	// Array of language categories
	pt::ptree plangs;

	for (std::string b : cache_.GetRuleDataIds<LanguageCategoryData>()) {
		try {
			LanguageCategoryData& lang_data = cache_.GetRuleData<LanguageCategoryData>(b);
			// Individual book
			pt::ptree plangcat;
			plangcat.put("id", lang_data.getID());
			plangcat.put("name", lang_data.getName());
			plangs.push_back(std::make_pair("", plangcat));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child("languageCategories", plangs);

	pt::write_json(filename, tree);
}
