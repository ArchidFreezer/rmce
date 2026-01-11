#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the SkillProgressionType::Type enum and associated functions
 * 
 * Contains the rules for the bonus skill ranks provide
 *
 * When a character developes either a skillcategory or a skill they spend a development point and gain a certain bonus to
 * the use of applicable skills. The size of the bonus gained depends on two things: the number of development points
 * spent and also the progression type.
 *
 * The progression types are applied at both the skill category and skill level. A skill may have any of the progression
 * type, but the only one available to categories is SkillProgressionType::Type::kStandard and only then if the skill being
 * used has the saame progression. If the skill has any other progression type the category adds nothing.
 */
namespace SkillProgressionType {

	/**
	 * @brief Enumeration class representing progression types of skills
	 * 
	 * Contains the set of valid skill progression types
	 */
	enum class Type {
		kCombined, /**< Skill and skill category progressions are combined into one */
		kLimited, /**< Skills bonus develops much slower and have no category bonus */
		kNone, /**< No skill development for any number of ranks */
		kSpecial, /**< Skills develop faster but have no category bonus */
		kStandard /**< Both skills and category use the same progression with the sum being used */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param type The SkillProgressionType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type type);

	/**
	 * @brief Teach operator<< how to print a SkillProgressionType::Type
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
	 * @return SkillProgressionType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}