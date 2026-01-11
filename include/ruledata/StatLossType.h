#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the StatLossType::Type enum and associated functions
 */
namespace StatLossType {
	/**
	 * @brief Enumeration class representing the racial type regarding mental stat loss after death
	 *
	 * Once a character dies their mental stats start to deteriorate with the rate of deterioration being a function of their
	 * race. This enumeration contains the set of rate loss types.
	 */
	enum class Type {
		kSlow, /**< Rate for hardy races such as Dwarves and Halflings */
		kNormal, /**< Rate for average races such as Humans and Orcs */
		kFast, /**< Rate for Wood and Half Elves */
		kVeryFast, /**< Rate for Grey Elves */
		kExtreme, /**< Rate for High Elves */
	};

	/**
	 * @brief Get the string representation of the given enum
	 * @param type The StatLossType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type type);

	/**
	 * @brief Teach operator<< how to print a StatLossType::Type
	 * @param out Output stream that the enum should be printed to
	 * @param type Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Type type) {
		return out << toString(type);
	}

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return StatLossType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}