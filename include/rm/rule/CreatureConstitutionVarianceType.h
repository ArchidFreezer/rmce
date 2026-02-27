#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

	/**
	 * @brief Contains the CreatureConstitutionVarianceTypeType::Type enum and associated functions
	 *
	 * The enumeration is used to represent the variance in constitution for a type of animal.
	 */
	namespace CreatureConstitutionVarianceType {

		/**
		 * @brief Enumeration class representing the bonus constitution code for an animal
		 *
		 * This is used to determine variance in constitution for a type of animal and is used for several purposes such varaince in the number of hit points an animal may
		 * have for any given level. Where higher codes represent greater varaiance in constitution and thus a greater range of hit points an animal may have for any given level.
		 *
		 * The value is also used to deteremine if the creature has anu bonus exhgaustion points or not with kNone representing no bonus exhaustion points and the other values representing increasing bonus exhaustion points.
		 *
		 * Stamina is nother factor that this coce may impact where a lookup table may be used to give the creature either more or less that the standard stamina points for its level.
		 */
		enum class Type { kNone, kA, kB, kC, kD, kE, kF, kG, kH };
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The CreatureConstitutionVarianceTypeType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a CreatureConstitutionVarianceTypeType::Type
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
		 * @return CreatureConstitutionVarianceTypeType::Type represented by the string
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
		 * @param type reference to set the CreatureConstitutionVarianceTypeType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid CreatureConstitutionVarianceTypeType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, CreatureConstitutionVarianceType::Type& type);

	}

} // namespace rm::rule::enums