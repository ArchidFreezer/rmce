#include <iostream>
#include <stdexcept>
#include "BookData.h"
#include "BookDatafileParserJson.h"


BookDatafileParserJson::BookDatafileParserJson(GameRuleDataCache& cache) : DatafileParserJson(cache, "Book") {
	root_node_ = "books";
}

void BookDatafileParserJson::parse() {
	std::cout << "Loading Book data ..." << std::endl;

	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", generateId(get_data_type(), name));
		std::string code = v.second.get<std::string>("code");
		std::string abbreviation = v.second.get<std::string>("abbreviation");
		std::string isbn = v.second.get<std::string>("isbn");

		std::unique_ptr<BookData> datum = std::make_unique<BookData>(id, code, name, abbreviation, isbn);
		cache().add<BookData>(std::move(datum), id);
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

	for (std::string b : cache().keys<BookData>()) {
		try {
			BookData& book_data = cache().get<BookData>(b);
			// Individual book
			pt::ptree book;
			book.put("id", book_data.id());
			book.put("code", book_data.code());
			book.put("name", book_data.name());
			book.put("abbreviation", book_data.abbreviation());
			book.put("isbn", book_data.isbn());
			pbooks.push_back(std::make_pair("", book));
		}
		catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(rootNode(), pbooks);

	pt::write_json(filename, tree);
}
