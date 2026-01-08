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
	* @brief Enumeration class representing the realms in a campaign
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
 * @param size The #ResistanceType to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(ResistanceType resistance) {
	using enum ResistanceType;

	switch (resistance) {
	case kArcane: return "Arcane";
	case kChanneling: return "Channeling";
	case kChannelingEssence: return "Channeling / Essence";
	case kChannelingMentalism: return "Channeling / Mentalism";
	case kCold: return "Cold";
	case kDisease: return "Disease";
	case kEssence: return "Essence";
	case kEssenceMentalism: return "Essence / Mentalism";
	case kFear: return "Fear";
	case kHeat: return "Heat";
	case kMentalism: return "Mentalism";
	case kPoison: return "Poison";
	}
}

/**
 * @brief Teach operator<< how to print a ResistanceType
 * @param out Output stream that the enum should be printed to
 * @param size Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, ResistanceType resistance) {
	return out << toString(resistance);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by the toString() in a case insensitive manner.
 *
 * For values that contain strings this function will accept these with the
 * space, with the space removed or with the space changed to an underscore:
 * - "ChannelingEssence"
 * - "Channeling_Essence"
 * - "Channeling / Essence"
 * - "Channeling Essence"

 *
 * @param sv string_view to get the enumeration for
 * @return #ResistanceType represented by the string
 *
 * @see toString()
 */
constexpr std::optional<ResistanceType> fromString(std::string_view sv) {
	using enum ResistanceType;

	const std::string& val = lcase(sv);
	if (val == "Arcane") return kArcane;
	if (val == "Channeling") return kChanneling;
	if (val == "Channeling / Essence") return kChannelingEssence;
	if (val == "Channeling Essence") return kChannelingEssence;
	if (val == "ChannelingEssence") return kChannelingEssence;
	if (val == "Channeling_Essence") return kChannelingEssence;
	if (val == "Channeling / Mentalism") return kChannelingMentalism;
	if (val == "Channeling Mentalism") return kChannelingMentalism;
	if (val == "ChannelingMentalism") return kChannelingMentalism;
	if (val == "Channeling_Mentalism") return kChannelingMentalism;
	if (val == "Cold") return kCold;
	if (val == "Disease") return kDisease;
	if (val == "Essence") return kEssence;
	if (val == "Essence / Mentalism") return kEssenceMentalism;
	if (val == "Essence Mentalism") return kEssenceMentalism;
	if (val == "EssenceMentalism") return kEssenceMentalism;
	if (val == "Essence_Mentalism") return kEssenceMentalism;
	if (val == "Fear") return kFear;
	if (val == "Heat") return kHeat;
	if (val == "Mentalism") return kMentalism;
	if (val == "Poison") return kPoison;

	return {};
}

/**
 * @brief Checks if the resistance is magical or not
 * @param resistance ResistanceType to test
 * @return `true` if the ResistanceType is magical
 * @return `false` if the ResistanceType is not magical
 */
constexpr bool isMagical(ResistanceType resistance) {
	using enum ResistanceType;

	switch (resistance) {
	case kCold:
	case kDisease:
	case kFear:
	case kHeat:
	case kPoison: return false;
	default: return true;
	}
}
