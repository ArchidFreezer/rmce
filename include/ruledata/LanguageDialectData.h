#pragma once

#include <set>
#include <string>

#include "GameRuleData.h"
#include <LanguageData.h>

/**
 * @class LanguageDialectData
 * 
 * @brief Class representing the dialects that exist for a language
 * 
 * In game dialects come into play more strongly in verbal communication more than written.
 * 
 * When reading a piece of text that includes dialect it is assumed that the reader can
 * understand the text, but it will take longer as they need to work some owrds out from context.
 * 
 * For verbal communication where understanding needs to be quicker, unless the speaker deliberately
 * slows down their speaking the listeners skill ranks should be considered as 3/4 (round up).
 * 
 * Typically the data id for a GameRuleData object would be generated from a type prefix
 * combined with a data value, but for dialects we use the base language name to allow it to be
 * retrievd more easily.
 * 
 * @see #LanguageData
 */
class LanguageDialectData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param name Name of the base language of the dialects
	 */
	LanguageDialectData(std::string_view name) :
		GameRuleData(name),
		name_{ name } {}

	/**
	 * @brief Constructor
	 * @param name Name of the base language of the dialects
	 * @param dialects Set to initilise the object with. __WARNING__: This constructor uses move semantics so will 
   *        _invalidate_ the object referenced in the parameter
	 */
	LanguageDialectData(std::string_view name, std::set<const LanguageData*>& dialects) :
		GameRuleData(name),
		name_{ name },
		dialects_{ std::move(dialects) } {}


	/**
	 * @brief Base language of the dialects
	 * 
	 * It should be noted that in many cases that there is no such thing as a base language and all
	 * languages are considered dialects, e.g. English could be considered a base language with many
	 * counties/regions having their own dialect, but there is no definitive english standard.
	 * 
	 * @return Name of the base language
	 */
	const std::string& name() const { return name_; }
	
	/**
	 * @brief Collection of dialects
	 * @return Ordered set of languages that are considered dialects
	 */
	const std::set<const LanguageData*>& dialects() const { return dialects_; }
	
	/**
	 * @brief Checks if a language is considered a dialect
	 * @param language id of the language to check
	 * @return true if the language is a dialect; false otherwise
	 */
	bool isDialect(const LanguageData& language) const { 
		for (auto& dialect : dialects_) {
			if (dialect->id() == language.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Add a language as a dialect
	 * @param language id of the language to add
	 */
	void add(const LanguageData& language) { 
		dialects_.insert(&language);
	}
	
	/**
	 * @brief Remove a language from the set of dialects
	 * @param language Identifier of the language to remove from the collection
	 */
	void remove(const LanguageData& language) { 
		for (auto& dialect : dialects_) {
			if (dialect->id() == language.id()) {
				dialects_.erase(dialect);
				return;
			}
		}
	}

private:
	/**
	 * @brief Name of the base language
	 */
	std::string name_{};
	/**
	 * @brief Collection of dialects
	 */
	std::set<const LanguageData*> dialects_{};
};