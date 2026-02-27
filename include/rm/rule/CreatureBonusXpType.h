#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm {

	/**
	 * @brief Contains the CreatureBonusXpTypeType::Type enum and associated functions
	 *
	 * The enumeration is used to represent the bonus XP code for an animal, which is used to determine how much bonus XP is awarded for killing the animal.
	 * The actual bonus XP is determined by a lookup table that maps the bonus XP code to a bonus XP value.
	 */
	namespace CreatureBonusXpType {

		/**
		 * @brief Enumeration class representing the bonus XP code for an animal
		 *
		 * This is used to determine how much bonus XP is awarded for killing the animal. The actual bonus XP is determined by a lookup table that maps the bonus XP code to a bonus XP value.
		 */
		enum class Type { kNone, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL };
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The CreatureBonusXpTypeType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a CreatureBonusXpTypeType::Type
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
		 * @return CreatureBonusXpTypeType::Type represented by the string
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
		 * @param type reference to set the CreatureBonusXpTypeType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid CreatureBonusXpTypeType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, CreatureBonusXpType::Type& type);

	}

} // namespace rm