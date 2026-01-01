#pragma once

#include <map>
#include <vector>
#include "Book.h"
#include "DatafileParser.h"

static const std::string TYPE{ "Book" };

class BookDatafileParser : public DatafileParser
{
public:
	BookDatafileParser(std::string_view filename, FileFormat const filetype);
	BookDatafileParser(std::string_view filename);
	void parse();
	void save(const std::string& filename, FileFormat const filetype);
	void save(const std::string& filename);
	void saveJSON(const std::string& filename);

private:
	std::map<std::string, Book> m_books{};
};