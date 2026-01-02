#include <iostream>
#include "BookDatafileParser.h"


BookDatafileParser::BookDatafileParser(std::string_view filename, FileFormat const filetype) : DatafileParser(filename, filetype) {}

BookDatafileParser::BookDatafileParser(std::string_view filename) : DatafileParser(filename) {}

void BookDatafileParser::parse() {
	std::cout << "Loading Book data ...";

	std::string rootNode{};
	switch (filetype_) {
	case DatafileParser::xml:
	{
		rootNode = "BookData.books";
		break;
	}
	case DatafileParser::json:
		rootNode = "books";
		break;
	}
	
	const pt::ptree& bookTree = ptree_.get_child(rootNode);
	for (const auto& v : bookTree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", getID(TYPE, name));

		BookData book(id, v.second.get<std::string>("code"), name, v.second.get<std::string>("abbr"), v.second.get<std::string>("isbn"));
		books_.insert({ id, book });
		std::cout << "\tBook name: " << book.getName() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void BookDatafileParser::save(const std::string& filename, FileFormat const filetype)
{
	switch (filetype) {
	case DatafileParser::xml:
	{
		pt::xml_writer_settings<std::string> settings('\t', 1);
		pt::write_xml(filename, ptree_, std::locale(), settings);
		break;
	}
	case DatafileParser::json:
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

	for (auto b : books_) {
		// Individual book
		pt::ptree book;
		book.put("id", b.second.getID());
		book.put("code", b.second.getCode());
		book.put("name", b.second.getName());
		book.put("abbr", b.second.getAbbreviation());
		book.put("isbn", b.second.getISBN());
		pbooks.push_back(std::make_pair("", book));
	}

	tree.add_child("books", pbooks);

	pt::write_json(filename, tree);
}
