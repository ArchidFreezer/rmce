#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm {

	/**
	 * @brief Contains the DiseasePoisonSeverityType::Type enum and associated functions
	 *
	 * Diseases and poisons can have a range of severities that determine how bad the symptoms are and how quickly they take effect. This enum is used to represent the severity of diseases and poisons in the game.
	 */
	namespace DiseasePoisonSeverityType {

		/**
		 * @brief Enumeration class representing the size of creatures
		 */
		enum class Type {
			kMild, /**< Mild symptoms that cause discomfort but are not life threatening. */
			kModerate, /**< Moderate symptoms that cause significant discomfort and may be life threatening if left untreated. */
			kSevere, /**< Severe symptoms that cause extreme discomfort and are likely to be life threatening if left untreated. */
			kExtreme, /**< Extreme symptoms that cause unbearable discomfort and are almost certain to be life threatening if left untreated. */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param type The DiseasePoisonSeverityType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type type);

		/**
		 * @brief Teach operator<< how to print a DiseasePoisonSeverityType::Type
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
		 * @param sv string_view to get the enumeration for
		 * @return DiseasePoisonSeverityType::Type represented by the string
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
	 * @param type reference to set the DiseasePoisonSeverityType::Type represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid DiseasePoisonSeverityType::Type
	 *
	 * @see toString()
	 */
		void fromString(std::string_view sv, DiseasePoisonSeverityType::Type& type);

	}

} // namespace rm