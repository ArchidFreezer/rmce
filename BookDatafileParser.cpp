#include <iostream>
#include <stdexcept>
#include "BookDatafileParser.h"


BookDatafileParser::BookDatafileParser(std::string_view filename, GameRuleDataCache& cache, FileFormat const filetype) : DatafileParser(filename, cache, filetype) {}

BookDatafileParser::BookDatafileParser(std::string_view filename, GameRuleDataCache& cache) : DatafileParser(filename, cache) {}

void BookDatafileParser::parse() {
	std::cout << "Loading Book data ..." << std::endl;

	std::string rootNode{};
	switch (filetype_) {
	case DatafileParser::kXml:
	{
		rootNode = "BookData.books";
		break;
	}
	case DatafileParser::kJson:
		rootNode = "books";
		break;
	}
	
	const pt::ptree& bookTree = ptree_.get_child(rootNode);
	for (const auto& v : bookTree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", getID(kRuleDataName, name));

		std::unique_ptr<BookData> book = std::make_unique<BookData>(id, v.second.get<std::string>("code"), name, v.second.get<std::string>("abbreviation"), v.second.get<std::string>("isbn"));
		cache_.AddRuleData<BookData>(std::move(book), id);
		std::cout << "\tBook name: " << name << std::endl;

	}
	std::cout << " done" << std::endl;
}

void BookDatafileParser::save(const std::string& filename, FileFormat const filetype)
{
	switch (filetype) {
	case DatafileParser::kXml:
	{
		pt::xml_writer_settings<std::string> settings('\t', 1);
		pt::write_xml(filename, ptree_, std::locale(), settings);
		break;
	}
	case DatafileParser::kJson:
		saveJSON(filename);
		break;
	}

}

void BookDatafileParser::save(const std::string &filename)
{
	save(filename, filetype_);
}

void BookDatafileParser::saveJSON(const std::string& filename)
{
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

	tree.add_child("books", pbooks);

	pt::write_json(filename, tree);
}
