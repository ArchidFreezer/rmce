#pragma once

#include <string>

#include "GameRuleData.h"

class LanguageData : public GameRuleData {
public:
	LanguageData(std::string_view id, std::string_view name, std::string_view category, std::string_view base_language, bool isSpoken, bool isWritten, bool isSomantic);

	const std::string& id() { return id_; }
	const std::string& name() { return name_; }
	const std::string& category() { return category_; }
	const std::string& baseLanguage() { return base_language_; }
	const bool isSpoken() const { return is_spoken_;	}
	const bool isWritten() const { return is_written_; }
	const bool isSomantic() const { return is_somantic_; }

private:
	std::string id_{};
	std::string name_{};
	std::string category_{};
	std::string base_language_{};
	bool is_spoken_{};
	bool is_written_{};
	bool is_somantic_{};
};

inline LanguageData::LanguageData(std::string_view id, std::string_view name, std::string_view category, std::string_view base_language, bool isSpoken, bool isWritten, bool isSomantic) :
	id_{ id },
	name_{ name },
	category_{ category },
	base_language_{ base_language },
	is_spoken_{ isSpoken },
	is_written_{ isWritten },
	is_somantic_{ isSomantic }
{}