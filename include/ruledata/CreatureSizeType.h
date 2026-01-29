#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

/**
 * @brief Contains the CreatureSizeType::Type enum and associated functions
 *
 * During combat and in other situations the size of a creature determines the lookup table to use. This defines the
 * valid sizes that there are tables for.
 */
namespace CreatureSizeType {

	/**
	* @brief Thrown to indicate that no CreatureSizeType::Type enum was found
	*
	*/
	class CreatureSizeTypeNotFoundException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		CreatureSizeTypeNotFoundException(const std::string& error) : std::runtime_error{ error }	{}
	};

	/**
	 * @brief Enumeration class representing the size of creatures
	 */
	enum class Type {
		kTiny, /**< Tiny creatures are insect sized */
		kSmall, /**< Small creatures are cat or dog sized */
		kMedium, /**< Medium creatures are approximately human sized such as deer or most sharks */
		kLarge, /**< Large creatures are buffalo or lions */
		kHuge /**< Super Large or Huge creatures are elephant or whale sized */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param size The CreatureSizeType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type size);

	/**
	 * @brief Teach operator<< how to print a CreatureSizeType::Type
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
	 * @return CreatureSizeType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}