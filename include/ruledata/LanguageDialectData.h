#pragma once

#include <set>
#include <string>

#include "GameRuleData.h"

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
 * @see #LanguageData
 */
class LanguageDialectData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param name Name of the base language of the dialects
	 */
	LanguageDialectData(std::string_view name);

	/**
	 * @brief Constructor
	 * @param name Name of the base language of the dialects
	 * @param dialects Set to initilise the object with. __WARNING__: This constructor uses move semantics so will 
   *        _invalidate_ the object referenced in the parameter
	 */
	LanguageDialectData(std::string_view name, std::set<std::string>& dialects);

	/**
	 * @brief Unique id
	 * 
	 * Typically the data id for a [GameRuleData](#GameRuleData) object would be generated from a type prefix
	 * combined with a data value, but for dialects we use the base language name.
	 * 
	 * @return Name of the base language
	 */
	inline const std::string& id() { return name(); }

	/**
	 * @brief Base language of the dialects
	 * 
	 * It should be noted that in many cases that there is no such thing as a base language and all
	 * languages are considered dialects, e.g. English could be considered a base language with many
	 * counties/regions having their own dialect, but there is no definitive english standard.
	 * 
	 * @return Name of the base language
	 */
	inline const std::string& name() { return name_; }
	
	/**
	 * @brief Collection of dialects
	 * @return Ordered set of languages that are considered dialects
	 */
	inline const std::set<std::string>& dialects() { return dialects_; }
	
	/**
	 * @brief Checks if a language is considered a dialect
	 * @param language id of the language to check
	 * @return true if the language is a dialect; false otherwise
	 */
	inline const bool isDialect(const std::string& language) const { return dialects_.count(language) >  0; }

	/**
	 * @brief Add a language as a dialect
	 * @param language id of the language to add
	 */
	inline void add(const std::string language) { dialects_.insert(language); }
	
	/**
	 * @brief Remove a language to the set of dialects
	 * @param language Identifier of the language to remove from the collection
	 */
	inline void remove(const std::string language) { dialects_.erase(language); }

private:
	/**
	 * @brief Name of the base language
	 */
	std::string name_{};
	/**
	 * @brief Collection of dialects
	 */
	std::set<std::string> dialects_{};
};

inline LanguageDialectData::LanguageDialectData(std::string_view name) :
	name_{ name } {}

inline LanguageDialectData::LanguageDialectData(std::string_view name, std::set<std::string>& dialects) :
	name_{ name },
	dialects_{ std::move(dialects) }
{}
