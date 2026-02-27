#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm {

	/**
	 * @brief Contains the CriticalSizeTableType::Type enum and associated functions
	 *
	 * Normally when resolving a critical strike the type of critical determines the lookup table used, but for large creatures it is the size of the creature
	 * that determines this. This differs from the CriticalTableType in that it is used in data objects, such as animals, where there is no actual attack to reseolve
	 * but we need to define which critical table to use when the creature is hit with a critical hit. This represents the valid types of critical table to use based on the size of the creature.
	 *
	 * - kNormal means that the creature should be resolved against the standard critical tables
	 * - kLarge means that the creature should be resolved against either the kLargeCreaturePhysical or kLargeCreatureSpell critical tables based on the nature of the attack
	 * - kHuge means that the creature should be resolved against either the kHugeCreaturePhysical or kHugeCreatureSpell critical tables based on the nature of the attack
	 */
	namespace CriticalSizeTableType {

		/**
		 * @brief Enumeration class representing the type of critical tables
		 */
		enum class Type {
			kNormal, /**< Criticals should be resolved against the standard critical tables */
			kLarge, /**< Criticals should be resolved against either the kLargeCreaturePhysical or kLargeCreatureSpell critical tables */
			kHuge, /**< Criticals should be resolved against either the kHugeCreaturePhysical or kHugeCreatureSpell critical tables */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param type The CriticalSizeTableType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type type);

		/**
		 * @brief Teach operator<< how to print a CriticalSizeTableType::Type
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
		 * @param sv String to get the enumeration for
		 * @return CriticalSizeTableType::Type represented by the string
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
		 * @param type reference to set the CriticalSizeTableType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid CriticalSizeTableType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, CriticalSizeTableType::Type& type);

	}

} // namespace rm