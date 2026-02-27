#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

/**
 * @brief Represents the relative wealth of a treasure, used to randomly generate treasure.
 */
namespace TreasureValueType {

	/**
	 * @brief Represents relative wealth of a treasure, used to randomly generate treasure.
	 */
	enum class Type { kVeryPoor, kPoor, kNormal, kRich, kVeryRich, kSpecial };

	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param size The TreasureValueType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type size);

	/**
	 * @brief Teach operator<< how to print a TreasureValueType::Type
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
	 * @return TreasureValueType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);

	/**
 * @brief Get the enumeration value based on a string and set it to the provided reference
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @param type reference to set the TreasureValueType::Type represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid TreasureValueType::Type
 *
 * @see toString()
 */
	void fromString(std::string_view sv, TreasureValueType::Type& type);
}
