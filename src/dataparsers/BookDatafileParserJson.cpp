#include <iostream>
#include <stdexcept>
#include "BookData.h"
#include "BookDatafileParserJson.h"


BookDatafileParserJson::BookDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Book", filename) {
	setRootNode("books");
}

BookDatafileParserJson::BookDatafileParserJson(GameRuleDataCache& cache) : BookDatafileParserJson(cache, "") {}

void BookDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading Book data ..." << std::endl;

	// Get the books to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));
		std::string code = v.second.get<std::string>("code");
		std::string abbreviation = v.second.get<std::string>("abbreviation");
		std::string isbn = v.second.get<std::string>("isbn");

		if (id_only) {
			// We create a BookData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<BookData> datum = std::make_unique<BookData>(id);
			cache().add<BookData>(std::move(datum), id);
		} else {
			BookData& ref = cache().get<BookData>(id);
			ref.setAbbreviation(abbreviation);
			ref.setCode(code);
			ref.setIsbn(isbn);
			ref.setName(name);
			std::cout << "\tBook name: " << ref.name() << std::endl;
		}

	}
	std::cout << " done" << std::endl;
}

void BookDatafileParserJson::save(const std::string& filename)
{
	if (filename.empty()) return;

	// Main tree
	pt::ptree tree;

	// Tree of books
	pt::ptree pbooks;

	std::set<std::string> keys{};
	cache().keys<BookData>(keys);

	for (std::string b : keys) {
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
