#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

/**
 * @brief Contains the SkillDifficultyType::Type enum and associated functions
 *
 * A successful foraging action will provide a character with the raw resources to enable healing or poison items. These raw resources, in most cases, need further preparation to turn them into a usable form. This enumeration details the
 * various types of preparation that are used.
 */
namespace SkillDifficultyType {

/**
 * @brief Enumeration class representing types of foragable resource preparation
 */
enum class Type {
	kRoutine,
	kEasy,
	kLight,
	kMedium,
	kHard,
	kVeryHard,
	kExtremelyHard,
	kSheerFolly,
	kAbsurd
};
using enum Type;

/**
 * @brief Get the string representation of the given enum
 * @param type The SkillDifficultyType::Type to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(Type type);

/**
 * @brief Teach operator<< how to print a SkillDifficultyType::Type
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
 * Any characters in \a sv that are not letters or numbers will be removed before comparison
 *
 * @param sv string_view to get the enumeration for
 * @return PreparationType::Type represented by the string
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
 * @param type reference to set the PreparationType::Type represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid PreparationType::Type
 *
 * @see toString()
 */
void fromString(std::string_view sv, SkillDifficultyType::Type& type);

} // namespace SkillDifficultyType

} // namespace rm::rule::enums