#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

/**
 * @brief Contains the SpellUserType::Type enum and associated functions
 *
 * During combat and in other situations the size of a creature determines the lookup table to use. This defines the
 * valid sizes that there are tables for.
 */
namespace SpellUserType {

	/**
	* @brief Thrown to indicate that no SpellUserType::Type enum was found
	*
	*/
	class SpellUserTypeNotFoundException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		SpellUserTypeNotFoundException(const std::string& error) : std::runtime_error{ error }	{}
	};

	/**
	 * @brief Enumeration class representing the spell user type of characters
	 */
	enum class Type {
		kPure, /**< Characters who primary focus is on a single spell realm */
		kHybrid, /**< Characters who primary focus is on more than one spell realm */
		kSemi, /**< Characters who focus on both arms and spells */
		kChaotic, /**< Characters who have never been taught magic and learn it independently */
		kNone, /**< Characters who focus on arms alone */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param size The SpellUserType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type size);

	/**
	 * @brief Teach operator<< how to print a SpellUserType::Type
	 * @param out Output stream that the enum should be printed to
	 * @param size Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Type size) {
		return out << toString(size);
	}

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return SpellUserType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}