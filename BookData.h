#pragma once

#include <string>

class BookData
{
public:
	BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn);

	const std::string_view getID() { return m_id; }
	const std::string_view getCode() { return m_code; }
	const std::string_view getName() { return m_name; }
	const std::string_view getAbbreviation() { return m_abbr; }
	const std::string_view getISBN() { return m_isbn; }

private:
	std::string m_id{};
	std::string m_code{};
	std::string m_name{};
	std::string m_abbr{};
	std::string m_isbn{};
};

inline BookData::BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn) :
	m_id{ id },
	m_code{ code },
	m_name{ name },
	m_abbr{ abbreviation },
	m_isbn{ isbn }
{}