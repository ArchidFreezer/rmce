#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

	/**
	 * @brief Contains the ManoeuvreDifficultyType::Type enum and associated functions
	 *
	 * These are a general measure of the difficultly in performing a manoeuvre that is out of the ordinary. These are typically moving manoeuvres such as climbing, jumping, swimming, etc.
	 * but could also be used for other types of manoeuvres such as performing a complex or unusual movement (e.g., acrobatics, dodging, tumbling, diving, etc.).
	 *
	 * Normally, a moving maneuver is limited to covering a distance no more than half of the character’s normal (not running) movement.
	 */
	namespace ManoeuvreDifficultyType {

		/**
		 * @brief Enumeration class representing the size of creatures
		 */
		enum class Type {
			kNormal, /**< Normal - does not require any manoeuvre roll */
			kRoutine, /**< Routine */
			kEasy, /**< Easy */
			kLight, /**< Light */
			kMedium, /**< Medium */
			kHard, /**< Hard */
			kVeryHard, /**< Very Hard */
			kExtremelyHard, /**< Extremely Hard */
			kSheerFolly, /**< Sheer Folly */
			kAbsurd /**< Absurd */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The ManoeuvreDifficultyType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a ManoeuvreDifficultyType::Type
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
		 * @return ManoeuvreDifficultyType::Type represented by the string
		 *
		 * @see toString()
		 */
		const std::optional<ManoeuvreDifficultyType::Type> fromString(std::string_view sv);

		/**
		 * @brief Get the enumeration value based on a string and set it to the provided reference
		 *
		 * Accepts values that are returned by toString() in a case insensitive manner.
		 *
		 * @param sv string_view to get the enumeration for
		 * @param type reference to set the ManoeuvreDifficultyType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid ManoeuvreDifficultyType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, ManoeuvreDifficultyType::Type& type);

	}

} // namespace rm::rule::enums