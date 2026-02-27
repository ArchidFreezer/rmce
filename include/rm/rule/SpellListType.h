#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the SpellListType::Type enum and associated functions
 *
 * Each spell list has a classification that determines the difficulty in accessing and learning them. This classification
 * is used to determine whether a character has access to the spell list at all, and if they do the cost in development
 * points to learn any spells it contains.
 */
namespace SpellListType {

	/**
	 * @brief Enumeration class representing the type of a spell list
	 */
	enum class Type {
		kBase, /**< The spells on these lists represent specialties of magical professions */
		kClosed, /**< Deepest and most powerful concepts common to each realm of magic */
		kOpen, /**< Easiest concepts to master in each realm of power, */
		kTrainingPackage, /**< Only available through the association with a training package */
		kRacial /**< Only available to members of specific races */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param type The SpellListType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type type);

	/**
	 * @brief Teach operator<< how to print a SpellListType::Type
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
	 * Any characters in \a sv that are not letters or numbers will be removed before comparison
	 *
	 * @param sv string_view to get the enumeration for
	 * @return SpellListType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}