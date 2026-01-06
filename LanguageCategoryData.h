#pragma once

#include <string>

#include "GameRuleData.h"

// Class to represent language families as described in Spell Law - Mentalism Companion (BOOK_MENTALISM_COMPANION) section 10.5.
// 
// In game these are represented in a similar manner to weapon skills with each individual language being under a language
// category. However, players will not ordinarily have access to these root language and they are typically only available
// via academic study. Improving a skill category represents knowledge of the grammar of the language and can therefore
// benefit both written and verbal forms of any language that are in the category.
// 
// Knowledge of a root language improves the skill classification of descendant languages by 1 step (i.e. Restricted ->
// Normal, Normal -> Everyman, Everyman -> Occupational, etc.) as long as the descendant language is of lower skill level
// than the root.
//
class LanguageCategoryData : public GameRuleData {
public:
	LanguageCategoryData(std::string_view id, std::string_view name);
	const std::string& getID() { return id_; }
  const std::string& getName() { return name_; }

private:
	std::string id_{};
	std::string name_{};

};

inline LanguageCategoryData::LanguageCategoryData(std::string_view id, std::string_view name) :
	id_{ id },
	name_{ name }
{}