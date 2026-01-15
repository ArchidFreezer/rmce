#pragma once

#include <string>

#include "GameRuleData.h"

/**
 * @class LanguageCategoryData
 * 
 * @brief Class to represent language families as described in Spell Law - Mentalism Companion (BOOK_MENTALISM_COMPANION) section 10.5.
 * 
 * In game these are represented in a similar manner to weapon skills with each individual language being under a language
 * category. However, players will not ordinarily have access to these root language and they are typically only available
 * via academic study. Improving a skill category represents knowledge of the grammar of the language and can therefore
 * benefit both written and verbal forms of any language that are in the category.
 * 
 * Knowledge of a root language improves the skill classification of descendant languages by 1 step (i.e. Restricted ->
 * Normal, Normal -> Everyman, Everyman -> Occupational, etc.) as long as the descendant language is of lower skill level
 * than the root.
 * 
 * @see #LanguageData
 */
class LanguageCategoryData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param id Unique identifier of the language category
	 * @param name Name of the category as seen in-game
	 */
	LanguageCategoryData(std::string_view id, std::string_view name);
	/**
	 * @brief Constructor
	 * @param id Unique identifier of the language category
	 */
	LanguageCategoryData(std::string_view id);
	/**
	 * @brief Set the name of the category
	 * @param name Category name
	 */
	void setName(std::string_view name) { name_ = name; }
	/**
	 * @brief Get the name of the category as seen in-game
	 * @return Name as a string reference
	 */
	const std::string& name() const { return name_; }

private:
	std::string name_{}; /**< Name of the category as seen in-game */

};

inline LanguageCategoryData::LanguageCategoryData(std::string_view id) : GameRuleData(id) {}

inline LanguageCategoryData::LanguageCategoryData(std::string_view id, std::string_view name) : GameRuleData(id), name_{ name } {}

