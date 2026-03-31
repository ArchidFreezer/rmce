#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

/**
 * @brief Contains the StatType::Type enum and associated functions
 *
 * The base mental and physical attributes of a character.
 * There are 10 of these with 5 being "primary" stats and 5 "development".
 */
namespace StatType {

/**
 * @brief Thrown when more stats than are allowed are attempted to be added
 *
 * Some game rules allow a specific number of stats to be applied to them. If more than this number are attempted to be
 * added this exception should be thrown.
 */
class TooManyStatsException : public std::runtime_error {
public:
	/**
	 * @brief Exception constructor
	 *
	 * std::runtime_error will handle the string
	 *
	 * @param error String to display for the error
	 */
	TooManyStatsException(const std::string& error) : std::runtime_error{error} {
	}
};

/**
 * @brief Enumeration class representing the type of base stats of a character
 */
enum class Type {
	kAgility,        /**<  Manual dexterity and litheness */
	kConstitution,   /**< General health and well-being, resistance to disease, and the ability to absorb more damage */
	kEmpathy,        /**< Relationship to the all-pervading force common to natural and the basis of most things supernatural */
	kIntuition,      /**< A combination of luck, genius, precognition, ESP, and the favour of the gods */
	kMemory,         /**<  The ability to retain what has previously been encountered and learned */
	kPresence,       /**< Control of one's own mind, courage, charisma, outward appearance and the ability to use these to affect and control others */
	kQuickness,      /**< A measure of raw speed, reflexes, and reaction time, this stat is often lumped with several others as dexterity */
	kReasoning,      /**< Ability to absorb, comprehend, and categorize data for future use and draw logical conclusions */
	kSelfDiscipline, /**<  The control of mind over body, the ability to push harder in pursuit of some goal, or to draw upon the inner reserves of strength */
	kStrength        /**<  Not brute musculature, but the ability to use existing muscles to their greatest advantage */
};
using enum Type;

/**
 * @brief Get the string representation of the given enum
 * @param stat The StatType::Type to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(Type stat);

/**
 * @brief Teach operator<< how to print a StatType::Type
 * @param out Output stream that the enum should be printed to
 * @param stat Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, Type stat) {
	return out << toString(stat);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * Any characters in \a sv that are not letters or numbers will be removed before comparison
 *
 * @param sv string_view to get the enumeration for
 * @return StatType::Type represented by the string
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
 * @param type reference to set the StatType::Type represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid StatType::Type
 *
 * @see toString()
 */
void fromString(std::string_view sv, StatType::Type& type);

/**
 * @brief Checks if the stat is a primary stat or not
 * @param stat StatType::Type to test
 * @return `true` if the StatType::Type is a primary stat
 * @return `false` if the StatType::Type is not a primary stat
 */
constexpr bool isPrimary(Type stat) {
	using enum Type;

	switch (stat) {
	case kEmpathy:
	case kIntuition:
	case kPresence:
	case kQuickness:
	case kStrength:
		return true;
	default:
		return false;
	}
}

/**
 * @brief Checks if the stat is a development stat or not
 * @param stat StatType::Type to test
 * @return `true` if the StatType::Type is a development stat
 * @return `false` if the StatType::Type is not a development stat
 */
constexpr bool isDevelopment(Type stat) {
	using enum Type;

	switch (stat) {
	case kAgility:
	case kConstitution:
	case kMemory:
	case kReasoning:
	case kSelfDiscipline:
		return true;
	default:
		return false;
	}
}

} // namespace StatType

} // namespace rm::rule::enums