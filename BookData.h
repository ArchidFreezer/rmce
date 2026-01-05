#pragma once

#include <string>

#include "GameRuleData.h"

class BookData : public GameRuleData
{
public:
	BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn);

	const std::string& getID() { return id_; }
	const std::string& getCode() { return code_; }
	const std::string& getName() { return name_; }
	const std::string& getAbbreviation() { return abbr_; }
	const std::string& getISBN() { return isbn_; }

private:
	std::string id_{};
	std::string code_{};
	std::string name_{};
	std::string abbr_{};
	std::string isbn_{};
};

inline BookData::BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn) :
	GameRuleData(GameRuleDataType::kBook),
	id_{ id },
	code_{ code },
	name_{ name },
	abbr_{ abbreviation },
	isbn_{ isbn }
{}