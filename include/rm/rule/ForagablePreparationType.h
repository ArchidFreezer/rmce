#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

/**
 * @brief Contains the ForagablePreparationType::Type enum and associated functions
 *
 * A successful foraging action will provide a character with the raw resources to enable healing or poison items. These raw resources, in most cases, need further preparation to turn them into a usable form. This enumeration details the
 * various types of preparation that are used.
 */
namespace ForagablePreparationType {

/**
 * @brief Enumeration class representing types of foragable resource preparation
 */
enum class Type {
	kApply,  /**< Requires 1-10 rounds to prepare; herb is then applied directly onto injured area */
	kBrew,   /**< Effective when drunk 20 rounds after water is boiled */
	kIngest, /**< Immediately usable and may be eaten, chewed, drunk, or inhaled, whichever is appropriate */
	kLiquid, /**< Raw matter is made into a paste which may be put in food or drink or be applied to tools or weapons and stays effective up to 1 hour, or until an object or person is struck; if a poison paste is used and a critical is
	            obtained, the foe must make a RR or he receives the effect; if the attack result is merely hits (no crit), the poison is gone */
	kPaste,  /**< Raw matter is made into a paste which may be put in food or drink or be applied to tools or weapons and stays effective up to 1 week, or until an object or person is struck; if a poison paste is used and a critical is
	            obtained, the foe must make a RR or he receives the effect; if the attack result is merely hits (no crit), the poison is gone */
	kPowder  /**< May not be applied to weapons; only effective in food or drink */
};
using enum Type;

/**
 * @brief Get the string representation of the given enum
 * @param type The ForagablePreparationType::Type to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(Type type);

/**
 * @brief Teach operator<< how to print a ForagablePreparationType::Type
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
void fromString(std::string_view sv, ForagablePreparationType::Type& type);

} // namespace ForagablePreparationType

} // namespace rm::rule::enums