#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the SkillActionType::Type enum and associated functions
 *
 * Each skill when used performs a type of action and this enum defines these
 */
namespace SkillActionType {

	/**
	 * @brief Enumeration class representing action types of skills
	 */
	enum class Type {
		kMoving, /**< Action involves movement of the characted performing it, e.g. running, jumping */
		kOB, /**< Action is offensive such as a weapon attack */
		kSpecial, /**< Action does not fit in the other categories such as communicating */
		kStatic /**< Action does not involve significant movement such as hiding or picking a lock */
	};

	/**
	 * @brief Get the string representation of the given enum
	 * @param action The SkillActionType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type action);

	/**
	 * @brief Teach operator<< how to print a SkillActionType::Type
	 * @param out Output stream that the enum should be printed to
	 * @param action Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Type action) {
		return out << toString(action);
	}

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return SkillActionType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}