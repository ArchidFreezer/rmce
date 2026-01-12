#pragma once

#include <string>

#include "GameRuleData.h"

/**
 * @class LanguageData
 * 
 * @brief Class representing an individual language used in the campaign world
 * 
 * Languages can take multiple forms, written, spoken and somantic (body). For simplicity languages such as sign-language
 * for the deaf are considered a true somantic language.
 * Players can develop in a language as with other skills using development points.
 * 
 * Some languages contain dialects where users of the same language from different areas or professions may add additional
 * words or grammar. This can hinder communication, though typically does not prevent it. For some languages there is no
 * true base, or the base is no longer in use so all languages may be considered dialects.
 * 
 * Languages are also grouped in categories that represent their development from a common root language that is often a
 * dead language that has fallen out of use.
 * 
 * @see #LanguageCategoryData
 * @see #LanguageDialectData
 */
class LanguageData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param id Unique id
	 * @param name Name as seen in-game
	 * @param category Language category the lasnguage belongs to
	 * @param base_language Language this is considered to be a dialect of, may be the same
	 * @param isSpoken Whether the language has a spoken form
	 * @param isWritten Whether the language has a written form
	 * @param isSomantic Whether the language has a somantic form
	 */
	LanguageData(std::string_view id, std::string_view name, std::string_view category, std::string_view base_language, bool isSpoken, bool isWritten, bool isSomantic);
	/**
	 * @brief Constructor
	 * @param id Unique identifier of the language
	 */
	LanguageData(std::string_view id);

	/**
	 * @brief Set the name of the language
	 * @param name Language name
	 */
	inline void setName(std::string_view name) { name_ = name; }
	/**
	 * @brief In-game name of the language as used by players and NPCs
	 * @return Name of the language
	 */
	inline const std::string& name() const { return name_; }
	/**
	 * @brief Set the language category representing the root language this has developed from
	 * @param category Language category
	 */
	inline void setCategory(std::string_view category) { category_ = category; }
	/**
	 * @brief get the language category representing the root language this has developed from
	 * @return Category as a string reference
	 */
	inline const std::string& category() const { return category_; }
	/**
	 * @brief For dialects get the language it is a dialect of.
	 *
	 * For some languages it may be the same as the language name, indicating it is the most prevalent dialect.
	 *
	 * @param base_language Dialects base language
	 */
	inline void setBaseLanguage(std::string base_language) { base_language_ = base_language; }
	/**
	 * @brief For dialects set the language it is a dialect of.
	 * 
	 * For some languages it may be the same as the language name, indicating it is the most prevalent dialect.
	 * 
	 * @return Dialects base language as a string reference
	 */
	inline const std::string& baseLanguage() const { return base_language_; }
	/**
	 * @brief Sets if the language has a spoken component
	 * @param is_spoken `true` if the langauge is even spoken; `false` otherwise
	 */
	inline void setIsSpoken(bool is_spoken) { is_spoken_ = is_spoken; }
	/**
	 * @brief Whether the language has a spoken component
	 * @return `true` if the language is spoken; `false` otherwise
	 */
	inline bool isSpoken() const { return is_spoken_;	}
	/**
	 * @brief Sets if the language has a written component
	 * @param is_written `true` if the langauge is written; `false` otherwise
	 */
	inline void setIsWritten(bool is_written) { is_written_ = is_written; }
	/**
	 * @brief Whether the language has a written component
	 * @return `true` if the language is written; `false` otherwise
	 */
	inline bool isWritten() const { return is_written_; }
	/**
	 * @brief Sets if the language has a somantic component
	 * @param is_somantic `true` if the langauge is somantic; `false` otherwise 
	 */
	inline void setIsSomantic(bool is_somantic) { is_somantic_ = is_somantic; }
	/**
	 * @brief Whether the language has a somantic component
	 * @return `true` if the language is somantic; `false` otherwise
	 */
	inline bool isSomantic() const { return is_somantic_; }

private:
	std::string name_{}; /**< Name as used in-game */
	std::string category_{}; /**< Language category this belongs to */
	std::string base_language_{}; /**< Language this language is a dialect of */
	bool is_spoken_{}; /**< Whether the language has a spoken form */
	bool is_written_{}; /**< Whether the language has a written form */
	bool is_somantic_{}; /**< Whether the language has a somantic form */
};

inline LanguageData::LanguageData(std::string_view id) : GameRuleData(id) {}

inline LanguageData::LanguageData(std::string_view id, std::string_view name, std::string_view category, std::string_view base_language, bool isSpoken, bool isWritten, bool isSomantic) :
	GameRuleData(id),
	name_{ name },
	category_{ category },
	base_language_{ base_language },
	is_spoken_{ isSpoken },
	is_written_{ isWritten },
	is_somantic_{ isSomantic } {}
