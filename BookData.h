#pragma once

#include <string>

#include "GameRuleData.h"

class BookData : public GameRuleData
{
public:
	BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn);

	const std::string& id() { return id_; }
	const std::string& code() { return code_; }
	const std::string& name() { return name_; }
	const std::string& abbreviation() { return abbreviation_; }
	const std::string& isbn() { return isbn_; }

private:
	std::string id_{};
	std::string code_{};
	std::string name_{};
	std::string abbreviation_{};
	std::string isbn_{};
};

inline BookData::BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn) :
	id_{ id },
	code_{ code },
	name_{ name },
	abbreviation_{ abbreviation },
	isbn_{ isbn }
{}