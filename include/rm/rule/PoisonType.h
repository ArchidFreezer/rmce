#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

/**
 * @brief Contains the PoisonType::Type enum and associated functions
 *
 * There are several different types of poisons in the game, each with different symptoms and areas of the body that they affect. This defines these types and provides functions for converting between the enum values and their string representations.
 */
namespace PoisonType {

	/**
	 * @brief Enumeration class representing the size of creatures
	 */
	enum class Type {
		kCirculatory, /**< Affects: feet, legs, hands, arms, abdomen, chest, neck, head */
		kConversion, /**< Affects: kidneys, bowels, intestines, stomach, liver, lungs, heart, throat, brain, legs, arms */
		kMuscle, /**< Affects: hands, arms, shoulders, feet, legs, hips, jaw, nose, ears, eyes, bowels, heart */
		kNerve, /**< Affects: extremities, eyes, mouth, brain */
		kReduction, /**< Affects: kidneys, bowels, intestines, stomach, liver, lungs, heart, throat, brain, legs, arms */
		kRespiratory, /**< Affects: lungs, throat, brain */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param type The PoisonType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type type);

	/**
	 * @brief Teach operator<< how to print a PoisonType::Type
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
	 * @return PoisonType::Type represented by the string
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
 * @param type reference to set the PoisonType::Type represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid PoisonType::Type
 *
 * @see toString()
 */
	void fromString(std::string_view sv, PoisonType::Type& type);


}