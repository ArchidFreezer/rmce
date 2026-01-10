#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file SpellListType.h
 *
 * Contains the SpellListType enum and associated functions
 */

 /**
	* @brief Enumeration class representing the type of a spell list
	*
	* Each spell list has a classification that determines the difficulty in accessing and learning them. This classification
	* is used to determine whether a character has access to the spell list at all, and if they do the cost in development
	* points to learn any spells it contains.
	*/
enum class SpellListType {
	kBase, /**< The spells on these lists represent specialties of magical professions */
	kClosed, /**< Deepest and most powerful concepts common to each realm of magic */
	kOpen, /**< Easiest concepts to master in each realm of power, */
	kTrainingPackage, /**< Only available through the association with a training package */
	kRacial /**< Only available to members of specific races */
};

/**
 * @brief Get the string representation of the given enum
 * @param type The #SpellListType to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(SpellListType type);

/**
 * @brief Teach operator<< how to print a SpellListType
 * @param out Output stream that the enum should be printed to
 * @param type Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, SpellListType type) {
	return out << toString(type);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * For values that contain strings with a space or slash this function will accept these with the
 * character, with the character removed or with the character changed to an underscore:
 * 
 * - "TrainingPackage"
 * - "Training_Package"
 * - "Training Package"
 * - "trainingpackage"
 * - "training_package"
 * - "training package"
 *
 * @param sv string_view to get the enumeration for
 * @return #SpellListType represented by the string
 *
 * @see toString()
 */
const std::optional<SpellListType> fromString(std::string_view sv);