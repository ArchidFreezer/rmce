#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file SkillDevelopmentType.h
 *
 * Contains the SkillDevelopmentType enum and associated functions
 */

 /**
	* @brief Enumeration class representing number of skill ranks gained for each rank purchased
	*
	* When purchasing ranks in a skill the number of ranks actually gained depends on whether the skill is treated as special
	* for the character. Certain races and professions have affinities or aversions to specific skills which modify the
	* number of skill ranks gained for each onew purchased.
	* 
	* You cannot develop part of a rank and you must spend enough points to gain a full rank.
	*/
enum class SkillDevelopmentType {
	kEveryman, /**< For every rank developed, the character gains two ranks */
	kOccupational, /**< For every rank developed, the character gains three ranks */
	kRestricted, /**< For every two ranks developed, the character gains only one rank */
	kStandard /**< For every rank developed, the character gains one rank */
};

/**
 * @brief Get the string representation of the given enum
 * @param type The #SkillDevelopmentType to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(SkillDevelopmentType type);

/**
 * @brief Teach operator<< how to print a SkillDevelopmentType
 * @param out Output stream that the enum should be printed to
 * @param type Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, SkillDevelopmentType type) {
	return out << toString(type);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @return #SkillDevelopmentType represented by the string
 *
 * @see toString()
 */
const std::optional<SkillDevelopmentType> fromString(std::string_view sv);
