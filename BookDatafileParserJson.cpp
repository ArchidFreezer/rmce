#include <iostream>
#include <stdexcept>
#include "BookData.h"
#include "BookDatafileParserJson.h"


BookDatafileParserJson::BookDatafileParserJson(GameRuleDataCache& cache) : DatafileParserJson(cache, "Book") {
	root_node_ = "books";
}

void BookDatafileParserJson::parse() {
	std::cout << "Loading Book data ..." << std::endl;

	const pt::ptree& bookTree = ptree_.get_child(root_node_);
	for (const auto& v : bookTree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", getID(getDataType(), name));

		std::unique_ptr<BookData> book = std::make_unique<BookData>(id, v.second.get<std::string>("code"), name, v.second.get<std::string>("abbreviation"), v.second.get<std::string>("isbn"));
		cache_.AddRuleData<BookData>(std::move(book), id);
		std::cout << "\tBook name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void BookDatafileParserJson::save(const std::string& filename)
{
	if (filename.empty()) return;

	// Main tree
	pt::ptree tree;

	// Array of books
	pt::ptree pbooks;

	for (std::string b : cache_.GetRuleDataIds<BookData>()) {
		try {
			BookData& book_data = cache_.GetRuleData<BookData>(b);
			// Individual book
			pt::ptree book;
			book.put("id", book_data.getID());
			book.put("code", book_data.getCode());
			book.put("name", book_data.getName());
			book.put("abbreviation", book_data.getAbbreviation());
			book.put("isbn", book_data.getISBN());
			pbooks.push_back(std::make_pair("", book));
		}
		catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(root_node_, pbooks);

	pt::write_json(filename, tree);
}
