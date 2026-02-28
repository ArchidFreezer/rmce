#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

	/**
	 * @brief Contains the DiseaseType::Type enum and associated functions
	 *
	 * There are several different types of diseases in the game, each with different symptoms and areas of the body that they affect. This defines these types and provides functions for converting between the enum values and their string representations.
	 */
	namespace DiseaseType {

		/**
		 * @brief Enumeration class representing the size of creatures
		 */
		enum class Type {
			kBubonic, /**< includes any viral or bacterial disease resulting from organisms that reside in the blood or other bodily fluids */
			kChemical, /**<  includes all forms of chemical dependence on herbs, drugs, alcohol, etc. or slow physical deterioration as a result of chemical alteration */
			kGenetic, /**< includes hereditary diseases (e.g., dyslexia, haemophilia, etc.) */
			kPneumonic, /**< includes any viral or bacterial disease carried in the air (i.e., transmitted by mist or breath) */
			kPsychiatric, /**< includes neuroses (e.g., anxieties, obsessions, phobias, etc.) and psychoses (e.g., multiple personality, psychopathy, etc.) */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param type The DiseaseType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type type);

		/**
		 * @brief Teach operator<< how to print a DiseaseType::Type
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
		 * @return DiseaseType::Type represented by the string
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
	 * @param type reference to set the DiseaseType::Type represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid DiseaseType::Type
	 *
	 * @see toString()
	 */
		void fromString(std::string_view sv, DiseaseType::Type& type);

	}

} // namespace rm::rule::enums