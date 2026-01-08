#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file Realm.h
 *
 * Contains the Realm enum and associated functions
 */

 /**
	* @brief Enumeration class representing the realms in a campaign
	*
	* The majority of realms are realms of magic, with only two, Arms and Subterfuge, being non-magical
	*/
enum class Realm {
	kArcane, /**< Arcane magic realm */
	kArms, /**< Non-magical realm of Arms */
	kChanneling, /**< Magical realm of Channeling */
	kChannelingEssence, /**< Magical hybrid realm of Channeling and Essence */
	kChannelingMentalism,  /**< Magical hybrid realm of Channeling and Mentalism */
	kEssence, /**< Magical realm of Essence */
	kEssenceMentalism,  /**< Magical hybrid realm of Essence and Mentalism */
	kMentalism, /**< Magical realm of Mentalism */
	kMundane, /**< Magical realm of Mundane - not suitable for all campaigns */
	kNeutral, /**< Magical realm of Neutral - not suitable for all campaigns */
	kSubterfuge  /**< Non-magical realm of Arms  - not suitable for all campaigns */
};

/**
 * @brief Get the string representation of the given enum
 * @param size The #Realm to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(Realm realm) {
	using enum Realm;

	switch (realm) {
	case kArcane: return "Arcane";
	case kArms: return "Arms";
	case kChanneling: return "Channeling";
	case kChannelingEssence: return "Channeling / Essence";
	case kChannelingMentalism: return "Channeling / Mentalism";
	case kEssence: return "Essence";
	case kEssenceMentalism: return "Essence / Mentalism";
	case kMentalism: return "Mentalism";
	case kMundane: return "Mundane";
	case kNeutral: return "Neutral";
	case kSubterfuge: return "Subterfuge";
	}
}

/**
 * @brief Teach operator<< how to print a Realm
 * @param out Output stream that the enum should be printed to
 * @param size Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, Realm realm) {
	return out << toString(realm);
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
 * - "Super_Large"

 *
 * @param sv string_view to get the enumeration for
 * @return #Realm represented by the string
 *
 * @see toString()
 */
constexpr std::optional<Realm> fromString(std::string_view sv) {
	using enum Realm;

	const std::string& val = lcase(sv);
	if (val == "Arcane") return kArcane;
	if (val == "Arms") return kArms;
	if (val == "Channeling") return kChanneling;
	if (val == "Channeling / Essence") return kChannelingEssence;
	if (val == "Channeling Essence") return kChannelingEssence;
	if (val == "ChannelingEssence") return kChannelingEssence;
	if (val == "Channeling_Essence") return kChannelingEssence;
	if (val == "Channeling / Mentalism") return kChannelingMentalism;
	if (val == "Channeling Mentalism") return kChannelingMentalism;
	if (val == "ChannelingMentalism") return kChannelingMentalism;
	if (val == "Channeling_Mentalism") return kChannelingMentalism;
	if (val == "Essence") return kEssence;
	if (val == "Essence / Mentalism") return kEssenceMentalism;
	if (val == "Essence Mentalism") return kEssenceMentalism;
	if (val == "EssenceMentalism") return kEssenceMentalism;
	if (val == "Essence_Mentalism") return kEssenceMentalism;
	if (val == "Mentalism") return kMentalism;
	if (val == "Mundane") return kMundane;
	if (val == "Neutral") return kNeutral;
	if (val == "Subterfuge") return kSubterfuge;

	return {};
}

/**
 * @brief Checks if the realm is magical or not
 * @param realm Realm to test
 * @return `true` if the Realm is magical
 * @return `false` if the Realm is not magical
 */
constexpr bool isMagical(Realm realm) {
	using enum Realm;

	switch (realm) {
	case kArms: return false;
	case kSubterfuge: return false;
	default: return true;
	}
}
