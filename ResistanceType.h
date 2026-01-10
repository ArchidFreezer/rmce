#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file ResistanceType.h
 *
 * Contains the ResistanceType enum and associated functions
 */

 /**
	* @brief Enumeration class representing the character resistance types in a campaign
	* 
	* As part of normal game play characters will be subject to various hazards. These may come in form of combat,
	* be environmental or some other action. A character is capable of mitigating some of the effects of these hazards
	* through reistances, which may be innate, magical or otherwise gained.
	* 
	* This enum lists the various hazard types that a character may acquire resistance to.
	*
	*/
enum class ResistanceType {
	kArcane, /**< Resistance to Arcane magic */
	kChanneling, /**< Resistance to Channeling magic */
	kChannelingEssence, /**< Resistance to hybrid Channeling / Essence magic */
	kChannelingMentalism, /**< Resistance to hybrid Channeling / Mentalism magic */
	kCold, /**< Resistance to elemental cold */
	kDisease, /**< Resistance to diseases */
	kEssence, /**< Resistance to Essence magic */
	kEssenceMentalism, /**< Resistance to hybrid Essence / Mentalism magic */
	kFear, /**< Resistance to fear */
	kHeat, /**< Resistance to elemental heat */
	kMentalism, /**< Magical resistance of Mentalism */
	kPoison, /**< Resistance to poisons */
};

/**
 * @brief Get the string representation of the given enum
 * @param resistance The #ResistanceType to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(ResistanceType resistance);

/**
 * @brief Teach operator<< how to print a ResistanceType
 * @param out Output stream that the enum should be printed to
 * @param resistance Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, ResistanceType resistance) {
	return out << toString(resistance);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * For values that contain strings with a space or slash this function will accept these with the
 * character, with the character removed or with the character changed to an underscore:
 * - "channelingessence"
 * - "channeling_essence"
 * - "channeling/essence"
 * - "channeling essence"
 * - "ChannelingEssence"
 * - "Channeling_Essence"
 * - "Channeling/Essence"
 * - "Channeling Essence"

 *
 * @param sv string_view to get the enumeration for
 * @return #ResistanceType represented by the string
 *
 * @see toString()
 */
const std::optional<ResistanceType> fromString(std::string_view sv);

/**
 * @brief Checks if the resistance is magical or not
 * @param resistance ResistanceType to test
 * @return `true` if the ResistanceType is magical
 * @return `false` if the ResistanceType is not magical
 */
constexpr bool isMagical(ResistanceType resistance);