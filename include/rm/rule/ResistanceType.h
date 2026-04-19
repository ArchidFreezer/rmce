#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

/**
 * @brief Contains the ResistanceType::Type enum and associated functions
 *
 * As part of normal game play characters will be subject to various hazards. These may come in form of combat,
 * be environmental or some other action. A character is capable of mitigating some of the effects of these hazards
 * through reistances, which may be innate, magical or otherwise gained.
 *
 * This enum lists the various hazard types that a character may acquire resistance to.
 *
 */
namespace ResistanceType {
/**
 * @brief Enumeration class representing the character resistance types in a campaign
 */
enum class Type {
	kArcane,              /**< Resistance to Arcane magic */
	kChanneling,          /**< Resistance to Channeling magic */
	kChannelingEssence,   /**< Resistance to hybrid Channeling / Essence magic */
	kChannelingMentalism, /**< Resistance to hybrid Channeling / Mentalism magic */
	kCold,                /**< Resistance to elemental cold */
	kDisease,             /**< Resistance to diseases */
	kEssence,             /**< Resistance to Essence magic */
	kEssenceMentalism,    /**< Resistance to hybrid Essence / Mentalism magic */
	kFear,                /**< Resistance to fear */
	kHeat,                /**< Resistance to elemental heat */
	kMentalism,           /**< Magical resistance of Mentalism */
	kPoison,              /**< Resistance to poisons */
};
using enum Type;

/**
 * @brief Get the string representation of the given enum
 * @param resistance The ResistanceType::Type to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(Type resistance);

/**
 * @brief Teach operator<< how to print a ResistanceType::Type
 * @param out Output stream that the enum should be printed to
 * @param resistance Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, Type resistance) {
	return out << toString(resistance);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * Any characters in \a sv that are not letters or numbers will be removed before comparison
 *
 * @param sv string_view to get the enumeration for
 * @return ResistanceType::Type represented by the string
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
 * @param type reference to set the ResistanceType::Type represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid ResistanceType::Type
 *
 * @see toString()
 */
void fromString(std::string_view sv, ResistanceType::Type& type);

/**
 * @brief Checks if the resistance is magical or not
 * @param resistance ResistanceType::Type to test
 * @return `true` if the ResistanceType::Type is magical
 * @return `false` if the ResistanceType::Type is not magical
 */
constexpr bool isMagical(Type resistance);
} // namespace ResistanceType

} // namespace rm::rule::enums