#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file SkillProgressionType.h
 *
 * Contains the SkillProgressionType enum and associated functions
 */

 /**
	* @brief Enumeration class representing progression types of skills
	*
	* When a character developes either a skillcategory or a skill they spend a development point and gain a certain bonus to
	* the use of applicable skills. The size of the bonus gained depends on two things: the number of development points
	* spent and also the progression type.
	* 
	* The progression types are applied at both the skill category and skill level. A skill may have any of the progression
	* type, but the only one available to categories is #SkillProgressionType::kStandard and only then if the skill being
	* used has the saame progression. If the skill has any other progression type the category adds nothing.
	*/
enum class SkillProgressionType {
	kCombined, /**< Skill and skill category progressions are combined into one */
	kLimited, /**< Skills bonus develops much slower and have no category bonus */
	kSpecial, /**< Skills develop faster but have no category bonus */
	kStandard /**< Both skills and category use the same progression with the sum being used */
};

/**
 * @brief Get the string representation of the given enum
 * @param action The #SkillProgressionType to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(SkillProgressionType action) {
	using enum SkillProgressionType;

	switch (action) {
	case kCombined: return "Combined";
	case kLimited: return "Limited";
	case kSpecial: return "Special";
	case kStandard: return "Standard";
	}
}

/**
 * @brief Teach operator<< how to print a SkillProgressionType
 * @param out Output stream that the enum should be printed to
 * @param action Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, SkillProgressionType action) {
	return out << toString(action);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @return #SkillProgressionType represented by the string
 *
 * @see toString()
 */
constexpr std::optional<SkillProgressionType> fromString(std::string_view sv) {
	using enum SkillProgressionType;

	const std::string& val = lcase(sv);
	if (val == "combined") return kCombined;
	if (val == "limited") return kLimited;
	if (val == "special") return kSpecial;
	if (val == "standard") return kStandard;

	return {};
}
