#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file SkillAction.h
 *
 * Contains the SkillAction enum and associated functions
 */

 /**
	* @brief Enumeration class representing action types of skills
	*
	* Each skill when used performs a type of action and this enum defines these
	*/
enum class SkillAction {
	kMoving, /**< Action involves movement of the characted performing it, e.g. running, jumping */
	kOB, /**< Action is offensive such as a weapon attack */
	kSpecial, /**< Action does not fit in the other categories such as communicating */
	kStatic /**< Action does not involve significant movement such as hiding or picking a lock */
};

/**
 * @brief Get the string representation of the given enum
 * @param action The #SkillAction to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(SkillAction action) {
	using enum SkillAction;

	switch (action) {
	case kMoving: return "Moving";
	case kOB: return "OB";
	case kSpecial: return "Special";
	case kStatic: return "Static";
	}
}

/**
 * @brief Teach operator<< how to print a SkillAction
 * @param out Output stream that the enum should be printed to
 * @param action Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, SkillAction action) {
	return out << toString(action);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @return #SkillAction represented by the string
 *
 * @see toString()
 */
constexpr std::optional<SkillAction> fromString(std::string_view sv) {
	using enum SkillAction;

	const std::string& val = lcase(sv);
	if (val == "moving") return kMoving;
	if (val == "ob") return kOB;
	if (val == "special") return kSpecial;
	if (val == "static") return kStatic;

	return {};
}
