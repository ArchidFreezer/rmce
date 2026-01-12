#pragma once

#include <string>

#include "GameRuleData.h"

/**
 * @class BookData
 *
 * @brief Class representing a Rolemaster rule book used in the campaign world
 * 
 * There are multiple books that have been printed with Rolemaster rules and guidelines over the years, some of which have
 * been made obsolete.
 *
 */
class BookData : public GameRuleData
{
public:
	/**
	 * @brief Constructor
	 * @param id Unique identifier for the book
	 * @param code Book reference number from the publisher
	 * @param name Name of the book
	 * @param abbreviation Abbreviation of the book as used by the publisher
	 * @param isbn ISBN book number
	 */
	BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn);

	/**
	 * @brief Constructor
	 * @param id Unique identifier for the book
	 */
	BookData(std::string_view id);

	/**
	 * @brief Get the unique identifier for the book
	 * @return Unique identifier as a string reference
	 */
	inline const std::string& id() { return id_; }

	/**
	 * @brief Set the book reference number from the publisher
	 * @param id Book reference number
	 */
	inline void setCode(std::string_view code) { code_ = code; }
	/**
	 * @brief Get the book reference number from the publisher
	 * @return Book reference number as a string reference
	 */
	inline const std::string& code() { return code_; }
	/**
	 * @brief Set the name of the book
	 * @param name Book name
	 */
	inline void setName(std::string_view name) { name_ = name; }
	/**
	 * @brief Get the name of the book
	 * @return Book name as a string reference
	 */
	inline const std::string& name() { return name_; }
	/**
	 * @brief Set the abbreviation of the book as used by the publisher
	 * @param abbreviation Book abbreviation
	 */
	inline void setAbbreviation(std::string_view abbreviation) { abbreviation_ = abbreviation; }
	/**
	 * @brief Get the abbreviation of the book as used by the publisher
	 * @return Book abbreviation as a string reference
	 */
	inline const std::string& abbreviation() { return abbreviation_; }
	/**
	 * @brief Set the ISBN book number
	 * @param isbn Book ISBN
	 */
	inline void setIsbn(std::string_view isbn) { isbn_ = isbn; }
	/**
	 * @brief Get the ISBN book number
	 * @return Book ISBN as a string reference
	 */
	inline const std::string& isbn() { return isbn_; }

private:
	std::string id_{}; /**< Unique identifier for the book */
	std::string code_{}; /**< Book reference number from the publisher */
	std::string name_{}; /**< Name of the book */
	std::string abbreviation_{}; /**< Abbreviation of the book as used by the publisher */
	std::string isbn_{}; /**< ISBN book number */
};

inline BookData::BookData(std::string_view id) : id_{ id } {}

inline BookData::BookData(std::string_view id, std::string_view code, std::string_view name, std::string_view abbreviation, std::string_view isbn) :
	id_{ id },
	code_{ code },
	name_{ name },
	abbreviation_{ abbreviation },
	isbn_{ isbn }
{}