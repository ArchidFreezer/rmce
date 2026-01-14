#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the SkillGroupType::Type enum and associated functions
 *
 * Skill groups are used to group together skill categories (SkillCategory) and have minimal in-game impact. The one area
 * where there is the possibility for their use is in training packages (TrainingPackageData) where a pack could allow a
 * character to allocate some ranks into a skill category or skill of the players choice in the group, though this is rare.
 *
 */
namespace SkillGroupType {
	/**
	 * @brief Enumeration class representing groups of skill categories
	 */
	enum class Type {
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
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param group The SkillGroupType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type group);

	/**
	 * @brief Teach operator<< how to print a SkillGroupType::Type
	 * @param out Output stream that the enum should be printed to
	 * @param group Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Type group) {
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
	 * @return SkillGroupType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);
}