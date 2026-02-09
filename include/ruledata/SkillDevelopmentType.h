#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the SkillDevelopmentType enum and associated functions
 *
 * When purchasing ranks in a skill the number of ranks actually gained depends on whether the skill is treated as special
 * for the character. Certain races and professions have affinities or aversions to specific skills which modify the
 * number of skill ranks gained for each onew purchased.
 *
 * You cannot develop part of a rank and you must spend enough points to gain a full rank.
 */
namespace SkillDevelopmentType {

	/**
	 * @brief Enumeration class representing number of skill ranks gained for each rank purchased
	 */
	enum class Type {
		kEveryman, /**< For every rank developed, the character gains two ranks */
		kOccupational, /**< For every rank developed, the character gains three ranks */
		kRestricted, /**< For every two ranks developed, the character gains only one rank */
		kStandard /**< For every rank developed, the character gains one rank */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param type The SkillDevelopmentType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type type);

	/**
	 * @brief Teach operator<< how to print a SkillDevelopmentType::Type
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
	 * @return SkillDevelopmentType::Type represented by the string
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
	 * @param type reference to set the SkillDevelopmentType::Type represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid SkillDevelopmentType::Type
	 *
	 * @see toString()
	 */
	void fromString(std::string_view sv, Type& type);
}