#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file SkillGroup.h
 *
 * Contains the SkillGroup enum and associated functions
 */

 /**
	* @brief Enumeration class representing groups of skill categories
	*
	* Skill groups are used to group together skill categories (#SkillCategory) and have minimal in-game impact. The one area
	* where there is the possibility for their use is in training packages (#TrainingPackageData) where a pack could allow a
	* character to allocate some ranks into a skill category or skill of the players choice in the group, though this is rare.
	* 
	*/
enum class SkillGroup {
	kArmour, /**< Manoeuvering in armour */
	kArtistic, /**< Creative expression skills */
	kAthletic, /**< Application of physical attributes (Str, Ag, End) in a coordinated manner */
	kAwareness, /**< Ability to derive information about an environment or situation */
	kBodyDevelopment, /**< Develop concussion hits */
	kCombatManoeuvres, /**< Specialised manoeuvers in combat situations */
	kCommunications, /**< Coordinated transfer of information */
	kCrafts, /**< Physical application of craftsman skills */
	kDirectedSpells, /**< Direct spell attacks, e.g. Bolts */
	kInfluence, /**< Intentional and applied effort at influencing, manipulating, or deceiving others */
	kLore, /**< Use of informational and academic skills */
	kMartialArts, /**< Unarmed combat skills involving blows dealt with the hands, feet, elbows, etc. */
	kOutdoor, /**< Animal handling plus outdoor information gathering and survival skills */
	kPowerAwareness, /**< Perception and interpretation of magical power, information, and writings */
	kPowerManipulation, /**< Manipulation and alteration of spells and magical energies */
	kPowerPointDevelopment, /**< Development of power points */
	kScienceAnalytic, /**< Scientific and analytical skills (theoretical) */
	kSelfControl, /**< Exercise of self-control and willpower */
	kSpecialAttacks, /**< Skills involving specialized attacks */
	kSpecialDefences, /**< Unusual defensive skills, typically available only through special training */
	kSpells, /**< Spells that are not directed */
	kSubterfuge, /**< Stealth and misdirection */
	kTechnicalTrade, /**< Professional skills that do not involve the creation of physical objects (crafts) */
	kUrban, /**< Skills pertaining to an urban environment */
	kWeapon /**< Use of weapons in combat */
};

/**
 * @brief Get the string representation of the given enum
 * @param group The #SkillGroup to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(SkillGroup group) {
	using enum SkillGroup;

	switch (group) {
	case kArmour: return "Armour";
	case kArtistic: return "Artistic";
	case kAthletic: return "Athletic";
	case kAwareness: return "Awareness";
	case kBodyDevelopment: return "Body Development";
	case kCombatManoeuvres: return "Combat Manoeuvres";
	case kCommunications: return "Communications";
	case kCrafts: return "Crafts";
	case kDirectedSpells: return "Directed Spells";
	case kInfluence: return "Influence";
	case kLore: return "Lore";
	case kMartialArts: return "Martial Arts";
	case kOutdoor: return "Outdoor";
	case kPowerAwareness: return "Power Awareness";
	case kPowerManipulation: return "Power Manipulation";
	case kPowerPointDevelopment: return "Power Point Development";
	case kScienceAnalytic: return "Science/Analytic";
	case kSelfControl: return "Self Control";
	case kSpecialAttacks: return "Special Attacks";
	case kSpecialDefences: return "Special Defences";
	case kSpells: return "Spells";
	case kSubterfuge: return "Subterfuge";
	case kTechnicalTrade: return "Technical/Trade";
	case kUrban: return "Urban";
	case kWeapon: return "Weapon";
	}
}

/**
 * @brief Teach operator<< how to print a SkillGroup
 * @param out Output stream that the enum should be printed to
 * @param group Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, SkillGroup group) {
	return out << toString(group);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * For values that contain strings with a space or slash this function will accept these with the
 * character, with the character removed or with the character changed to an underscore:
 * - "scienceanalytic"
 * - "science analytic"
 * - "science_analytic"
 * - "science/analytic"
 * - "ScienceAnalytic"
 * - "Science Analytic"
 * - "Science_Analytic"
 * - "Science/Analytic"
 *
 * @param sv string_view to get the enumeration for
 * @return #SkillGroup represented by the string
 *
 * @see toString()
 */
constexpr std::optional<SkillGroup> fromString(std::string_view sv) {
	using enum SkillGroup;

	const std::string& val = lcase(sv);
	if (val == "armour") return kArmour;
	if (val == "artistic") return kArtistic;
	if (val == "athletic") return kAthletic;
	if (val == "awareness") return kAwareness;
	if (val == "bodydevelopment") return kBodyDevelopment;
	if (val == "body_development") return kBodyDevelopment;
	if (val == "body_development") return kBodyDevelopment;
	if (val == "combatmanoeuvres") return kCombatManoeuvres;
	if (val == "combat_manoeuvres") return kCombatManoeuvres;
	if (val == "combat manoeuvres") return kCombatManoeuvres;
	if (val == "communications") return kCommunications;
	if (val == "crafts") return kCrafts;
	if (val == "directedspells") return kDirectedSpells;
	if (val == "directed_spells") return kDirectedSpells;
	if (val == "directed spells") return kDirectedSpells;
	if (val == "influence") return kInfluence;
	if (val == "lore") return kLore;
	if (val == "martialarts") return kMartialArts;
	if (val == "martial_arts") return kMartialArts;
	if (val == "martial arts") return kMartialArts;
	if (val == "outdoor") return kOutdoor;
	if (val == "powerawareness") return kPowerAwareness;
	if (val == "power_awareness") return kPowerAwareness;
	if (val == "power awareness") return kPowerAwareness;
	if (val == "powermanipulation") return kPowerManipulation;
	if (val == "power_manipulation") return kPowerManipulation;
	if (val == "power manipulation") return kPowerManipulation;
	if (val == "powerpointdevelopment") return kPowerPointDevelopment;
	if (val == "power_point_development") return kPowerPointDevelopment;
	if (val == "power point development") return kPowerPointDevelopment;
	if (val == "scienceanalytic") return kScienceAnalytic;
	if (val == "science_analytic") return kScienceAnalytic;
	if (val == "science analytic") return kScienceAnalytic;
	if (val == "science/analytic") return kScienceAnalytic;
	if (val == "selfcontrol") return kSelfControl;
	if (val == "self_control") return kSelfControl;
	if (val == "self control") return kSelfControl;
	if (val == "specialattacks") return kSpecialAttacks;
	if (val == "special_attacks") return kSpecialAttacks;
	if (val == "special attacks") return kSpecialAttacks;
	if (val == "specialdefences") return kSpecialDefences;
	if (val == "special_defences") return kSpecialDefences;
	if (val == "special defences") return kSpecialDefences;
	if (val == "spells") return kSpells;
	if (val == "subterfuge") return kSubterfuge;
	if (val == "technicaltrade") return kTechnicalTrade;
	if (val == "technical_trade") return kTechnicalTrade;
	if (val == "technical trade") return kTechnicalTrade;
	if (val == "technical/trade") return kTechnicalTrade;
	if (val == "urban") return kUrban;
	if (val == "weapon") return kWeapon;

	return {};
}
