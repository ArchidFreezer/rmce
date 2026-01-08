#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file CriticalType.h
 *
 * Contains the CriticalType enum and associated functions
 */

/**
 * @brief Enumeration class representing types of critical damage
 *
 * During combat and in other situations a characte may receive critical damage with the actual damage taken determined
 * by a lookup table. There are different types of critical based on the source of the damage and this enum lists all
 * the possible types.
 */
enum class CriticalType {
	kBrawling, /**< Combat without weapons */
	kGrapple, /**< Combat where the attacker is trying to grab the target */
	kImpact, /**< Certain spell attacks such a bolts, or the use of pure force */
	kKrush, /**< Weapons that use blunt force */
	kMartialArtsStrikes, /**< Soft martial arts such as Judo or Wrestling */
	kMartialArtsSweeps, /**< Hard martial atrs such as Karate, Boxing or Tackling */
	kPuncture, /**< Weapons that use a point to pierce the target such as arrows or animal teeth/horns */
	kSlash, /**< Weapons that use an edge to attack */
	kSubdual, /**< The point is immobilize or render a target unconscious */
	kTiny, /**< Creatures of #CreatureSize::kTiny or other irritants */
	kUnbalance, /**< Attempt to knock the target over or off-balance them */
	kAether, /**< Elemental force of creation */
	kCold, /**< Elemental cold damage */
	kElectrical,  /**< Elemental electrical damage */
	kHeat,  /**< Elemental heat damage */
	kNether /**< Elemental force of destruction */
};

/**
 * @brief Get the string representation of the given enum
 * @param type The #CriticalType to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(CriticalType type) {
	using enum CriticalType;

	switch (type)
	{
	case kBrawling: return "Brawling";
	case kGrapple: return "Grapple";
	case kImpact: return "Impact";
	case kKrush: return "Krush";
	case kMartialArtsStrikes: return "Martial Arts Strikes";
	case kMartialArtsSweeps: return "Martial Arts Sweeps";
	case kPuncture: return "Puncture";
	case kSlash: return "Slash";
	case kSubdual: return "Subdual";
	case kTiny: return "Tiny";
	case kUnbalance: return "Unbalance";
	case kAether: return "Aether";
	case kCold: return "Cold";
	case kElectrical: return "Electrical";
	case kHeat: return "Heat";
	case kNether: return "Nether";
	}
}

/**
 * @brief Teach operator<< how to print a CriticalType
 * @param out Output stream that the enum should be printed to
 * @param type Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, CriticalType type) {
	return out << toString(type);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * For values that contain strings thie function will accept these with the
 * space, with the space removed or with the space changed to an underscore:
 * - "Martial Arts Strikes"
 * - "martial arts strikes"
 * - "MartialArtsStrikes"
 * - "martialartsstrikes"
 * - "SMartial_Arts_Strikes"
 * - "martial_arts_strikes"
 *
 * @param sv string_view to get the enumeration for
 * @return #CriticalType represented by the string
 *
 * @see toString()
 */
constexpr std::optional<CriticalType> fromString(std::string_view sv) {
	using enum CriticalType;

	const std::string& val = lcase(sv);
	if (val == "brawling") return kBrawling;
	if (val == "grapple") return kGrapple;
	if (val == "impact") return kImpact;
	if (val == "krush") return kKrush;
	if (val == "martialartsstrikes") return kMartialArtsStrikes;
	if (val == "martial_arts_strikes") return kMartialArtsStrikes;
	if (val == "martial arts strikes") return kMartialArtsStrikes;
	if (val == "martialartssweeps") return kMartialArtsSweeps;
	if (val == "martial_arts_sweeps") return kMartialArtsSweeps;
	if (val == "martial arts sweeps") return kMartialArtsSweeps;
	if (val == "puncture") return kPuncture;
	if (val == "slash") return kSlash;
	if (val == "subdual") return kSubdual;
	if (val == "tiny") return kTiny;
	if (val == "unbalance") return kUnbalance;
	if (val == "aether") return kAether;
	if (val == "cold") return kCold;
	if (val == "electrical") return kElectrical;
	if (val == "heat") return kHeat;
	if (val == "nether") return kNether;

	return {};
}
