#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file Stat.h
 *
 * Contains the Stat enum and associated functions
 */

 /**
	* @brief Enumeration class representing the type of base stats of a character
	*
	* The base mental and physical attributes of a character.
	* There are 10 of these with 5 being "primary" stats and 5 "development".
	*/
enum class Stat {
	kAgility, /**<  Manual dexterity and litheness */
	kConstitution, /**< General health and well-being, resistance to disease, and the ability to absorb more damage */
	kEmpathy, /**< Relationship to the all-pervading force common to natural and the basis of most things supernatural */
	kIntuition, /**< A combination of luck, genius, precognition, ESP, and the favour of the gods */
	kMemory, /**<  The ability to retain what has previously been encountered and learned */
	kPresence, /**< Control of one's own mind, courage, charisma, outward appearance and the ability to use these to affect and control others */
	kQuickness, /**< A measure of raw speed, reflexes, and reaction time, this stat is often lumped with several others as dexterity */
	kReasoning, /**< Ability to absorb, comprehend, and categorize data for future use and draw logical conclusions */
	kSelfDiscipline, /**<  The control of mind over body, the ability to push harder in pursuit of some goal, or to draw upon the inner reserves of strength */
	kStrength /**<  Not brute musculature, but the ability to use existing muscles to their greatest advantage */
};

/**
 * @brief Get the string representation of the given enum
 * @param stat The #Stat to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(Stat stat) {
	using enum Stat;

	switch (stat) {
	case kAgility: return "Agility";
	case kConstitution: return "Constitution";
	case kEmpathy: return "Empathy";
	case kIntuition: return "Intuition";
	case kMemory: return "Memory";
	case kPresence: return "Presence";
	case kQuickness: return "Quickness";
	case kReasoning: return "Reasoning";
	case kSelfDiscipline : return "Self Discipline";
	case kStrength: return "Strength";
	}
}

/**
 * @brief Teach operator<< how to print a Stat
 * @param out Output stream that the enum should be printed to
 * @param stat Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, Stat stat) {
	return out << toString(stat);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * For values that contain strings this function will accept these with the
 * space, with the space removed or with the space changed to an underscore:
 * - "SelfDiscipline"
 * - "Self_Discipline"
 * - "Self Discipline"
 * - "selfdiscipline"
 * - "self_discipline"
 * - "self discipline"
 *
 * @param sv string_view to get the enumeration for
 * @return #Stat represented by the string
 *
 * @see toString()
 */
constexpr std::optional<Stat> fromString(std::string_view sv) {
	using enum Stat;

	const std::string& val = lcase(sv);
	if (val == "agility") return kAgility;
	if (val == "constitution") return kConstitution;
	if (val == "empathy") return kEmpathy;
	if (val == "intuition") return kIntuition;
	if (val == "memory") return kMemory;
	if (val == "presence") return kPresence;
	if (val == "quickness") return kQuickness;
	if (val == "reasoning") return kReasoning;
	if (val == "selfdiscipline") return kSelfDiscipline;
	if (val == "self_discipline") return kSelfDiscipline;
	if (val == "self discipline") return kSelfDiscipline;
	if (val == "strength") return kStrength;

	return {};
}

/**
 * @brief Checks if the stat is a primary stat or not
 * @param stat Stat to test
 * @return `true` if the Stat is a primary stat
 * @return `false` if the Stat is not a primary stat
 */
constexpr bool isPrimary(Stat stat) {
	using enum Stat;

	switch (stat) {
	case kEmpathy:
	case kIntuition:
	case kPresence:
	case kQuickness:
	case kStrength: return true;
	default: return false;
	}
}

/**
 * @brief Checks if the stat is a development stat or not
 * @param stat Stat to test
 * @return `true` if the Stat is a development stat
 * @return `false` if the Stat is not a development stat
 */
constexpr bool isDevelopment(Stat stat) {
	using enum Stat;

	switch (stat) {
	case kAgility:
	case kConstitution:
	case kMemory:
	case kReasoning:
	case kSelfDiscipline: return true;
	default: return false;
	}
}
