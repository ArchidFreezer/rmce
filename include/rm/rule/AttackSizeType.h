#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

	/**
	 * @brief Contains the AttackSizeType::Type enum and associated functions
	 *
	 * Rolls against a SpecialAttackTable require a size element that this provides.
	 */
	namespace AttackSizeType {

		/**
		 * @brief Enumeration class representing the attack size
		 */
		enum class Type {
			kSmall, /**< Small creatures are cat or dog sized */
			kMedium, /**< Medium creatures are approximately human sized such as deer or most sharks */
			kLarge, /**< Large creatures are buffalo or lions */
			kHuge /**< Huge creatures are elephant or whale sized */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The AttackSizeType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a AttackSizeType::Type
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
		 * @return AttackSizeType::Type represented by the string
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
		 * @param type reference to set the AttackSizeType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid AttackSizeType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, AttackSizeType::Type& type);
	}

} // namespace rm::rule::enums