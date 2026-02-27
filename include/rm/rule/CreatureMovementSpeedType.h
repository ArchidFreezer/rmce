#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm {

	/**
	 * @brief Contains the CreatureMovementSpeedType::Type enum and associated functions
	 *
	 * These are a measures of the spedd that a creatures moves at, which can be used for various purposes such as determining how quickly a creature can move across the map,
	 *  how quickly it can attack or defend itself, and how quickly it can react to the player's actions. It plays a part in both the base DB and also evasion DB a creature has.
	 */
	namespace CreatureMovementSpeedType {

		/**
		 * @brief Enumeration class representing the size of creatures
		 */
		enum class Type {
			kImmobile, /**< Immobile */
			kInching, /**< Inching */
			kCreeping, /**<  Creeping */
			kVerySlow, /**< Very Slow */
			kSlow, /**< Slow */
			kMedium, /**< Medium */
			kModeratelyFast, /**<  Moderately Fast */
			kFast, /**< Fast */
			kVeryFast, /**< Very Fast */
			kBlindinglyFast /**< Blindingly Fast */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The CreatureMovementSpeedType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a CreatureMovementSpeedType::Type
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
		 * @return CreatureMovementSpeedType::Type represented by the string
		 *
		 * @see toString()
		 */
		const std::optional<CreatureMovementSpeedType::Type> fromString(std::string_view sv);

		/**
		 * @brief Get the enumeration value based on a string and set it to the provided reference
		 *
		 * Accepts values that are returned by toString() in a case insensitive manner.
		 *
		 * @param sv string_view to get the enumeration for
		 * @param type reference to set the CreatureMovementSpeedType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid CreatureMovementSpeedType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, CreatureMovementSpeedType::Type& type);

	}

} // namespace rm