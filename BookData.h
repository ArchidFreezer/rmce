#pragma once

#include <string>

class BookData
{
public:
	BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn);

	const std::string_view getID() { return id_; }
	const std::string_view getCode() { return code_; }
	const std::string_view getName() { return name_; }
	const std::string_view getAbbreviation() { return abbr_; }
	const std::string_view getISBN() { return isbn_; }

private:
	std::string id_{};
	std::string code_{};
	std::string name_{};
	std::string abbr_{};
	std::string isbn_{};
};

inline BookData::BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn) :
	id_{ id },
	code_{ code },
	name_{ name },
	abbr_{ abbreviation },
	isbn_{ isbn }
{}