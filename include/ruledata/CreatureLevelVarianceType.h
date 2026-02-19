#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the CreatureLevelVarianceTypeType::Type enum and associated functions
 *
 * The enumeration is used to represent the variance in level for a type of animal.
 */
namespace CreatureLevelVarianceType {

	/**
	 * @brief Enumeration class representing the bonus constitution code for an animal
	 *
	 * This is used to determine variance in levels for a type of animal and is used for several purposes such varaince in the number of hit points an animal may
	 *
	 */
	enum class Type { kNone, kA, kB, kC, kD, kE, kF, kG, kH };
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param size The CreatureLevelVarianceTypeType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type size);

	/**
	 * @brief Teach operator<< how to print a CreatureLevelVarianceTypeType::Type
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
	 * @return CreatureLevelVarianceTypeType::Type represented by the string
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
	 * @param type reference to set the CreatureLevelVarianceTypeType::Type represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid CreatureLevelVarianceTypeType::Type
	 *
	 * @see toString()
	 */
	void fromString(std::string_view sv, CreatureLevelVarianceType::Type& type);

}