#pragma once

#include <string>

#include <GameRuleData.h>

namespace rm::rule {

	/**
	 * @class BookData
	 *
	 * @brief Class representing a Rolemaster rule book used in the campaign world
	 *
	 * There are multiple books that have been printed with Rolemaster rules and guidelines over the years, some of which have
	 * been made obsolete.
	 *
	 */
	class BookData : public GameRuleData {
	public:
		BookData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

		/**
		 * @brief Constructor
		 * @param id Unique identifier for the book
		 */
		BookData(std::string_view id) : GameRuleData(id) {}

		/**
		 * @brief Set the book reference number from the publisher
		 * @param code Book reference number
		 */
		void setCode(std::string_view code) { code_ = code; }
		/**
		 * @brief Get the book reference number from the publisher
		 * @return Book reference number as a string reference
		 */
		const std::string& code() const { return code_; }
		/**
		 * @brief Set the name of the book
		 * @param name Book name
		 */
		void setName(std::string_view name) { name_ = name; }
		/**
		 * @brief Get the name of the book
		 * @return Book name as a string reference
		 */
		const std::string& name() const { return name_; }
		/**
		 * @brief Set the abbreviation of the book as used by the publisher
		 * @param abbreviation Book abbreviation
		 */
		void setAbbreviation(std::string_view abbreviation) { abbreviation_ = abbreviation; }
		/**
		 * @brief Get the abbreviation of the book as used by the publisher
		 * @return Book abbreviation as a string reference
		 */
		const std::string& abbreviation() const { return abbreviation_; }
		/**
		 * @brief Set the ISBN book number
		 * @param isbn Book ISBN
		 */
		void setIsbn(std::string_view isbn) { isbn_ = isbn; }
		/**
		 * @brief Get the ISBN book number
		 * @return Book ISBN as a string reference
		 */
		const std::string& isbn() const { return isbn_; }

	private:
		std::string code_{}; /**< Book reference number from the publisher */
		std::string name_{}; /**< Name of the book */
		std::string abbreviation_{}; /**< Abbreviation of the book as used by the publisher */
		std::string isbn_{}; /**< ISBN book number */
	};

} // namespace rm::rule