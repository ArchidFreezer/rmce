#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>
/**
 * @brief Contains the CriticalModifierType::Type enum and associated functions
 *
 * When resolving criticals against some creatures the roll and/or the result may be modifed, whith this enum defining the modifications that may be applied.
 * It is possible that multiple of these modifiers may impact a single critical result, for example a creature may have both the kDecreaseSeverity1 and kNoBleed
 * modifiers, in which case the severity of the critical would be decreased by 1 and then any bleed results would be ignored.
 */
namespace CriticalModifierType {

	/**
	 * @brief Enumeration class representing the type of critical tables
	 */
	enum class Type {
		kDecreaseSeverityI, /**< Decrease the severity of any critical by 1. 'A' is modified by -20; 'B' becomes 'A'; 'C' become 'B' etc, */
		kDecreaseSeverityII, /**< Decrease the severity of any critical by 2. 'A' is modified by -50; 'B'is modified by -20; 'C' become 'A' etc, */
		kNoBleed, /**< Stun and bled results do not affect the target */
		kNoStun /**< Stun results do not affect the target */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param type The CriticalModifierType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type type);

	/**
	 * @brief Teach operator<< how to print a CriticalModifierType::Type
	 * @param out Output stream that the enum should be printed to
	 * @param type Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Type type) {
		return out << toString(type);
	};

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * Any characters in \a sv that are not letters or numbers will be removed before comparison
	 *
	 * @param sv String to get the enumeration for
	 * @return CriticalModifierType::Type represented by the string
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
	 * @param type reference to set the CriticalModifierType::Type represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid CriticalModifierType::Type
	 *
	 * @see toString()
	 */
	void fromString(std::string_view sv, CriticalModifierType::Type& type);

}