#pragma once

#include <string>

#include "GameRuleData.h"

class LanguageData : public GameRuleData {
public:
	LanguageData(std::string_view id, std::string_view name, std::string_view category, bool is_spoken, bool is_written, bool is_somantic);

	const std::string& getID() { return id_; }
	const std::string& getName() { return name_; }
	const std::string& getCategory() { return category_; }
	const bool getIsSpoken() { return is_spoken_;	}
	const bool getIsWritten() { return is_written_; }
	const bool getIsSomantic() { return is_somantic_; }

private:
	std::string id_{};
	std::string name_{};
	std::string category_{};
	bool is_spoken_{};
	bool is_written_{};
	bool is_somantic_{};
};

inline LanguageData::LanguageData(std::string_view id, std::string_view name, std::string_view category, bool is_spoken, bool is_written, bool is_somantic) :
	id_{ id },
	name_{ name },
	category_{ category },
	is_spoken_{ is_spoken },
	is_written_{ is_written },
	is_somantic_{ is_somantic }
{}