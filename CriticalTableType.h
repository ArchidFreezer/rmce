#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file CriticalTableType.h
 *
 * Contains the CriticalTableType enum and associated functions
 */

/**
 * @brief Enumeration class representing the type of critical tables
 *
 * Normally when resolving a critical strike the type of criticalk determines the lookup table used, but for large
 * creatures it is the size of the creature and the nature of the damage that determines this, with Physical and Spell
 * damage being the only factors. This class represents the valid types of critical table to use.
 */
enum class CriticalTableType {
	kNormal, /**< Normal criticals are those that do not fit in the other categories */
	kLargeCreaturePhysical, /**< These are non-magical attacks against creatures with a #CreatureSize::kLarge */
	kSuperLargeCreaturePhysical, /**< These are non-magical attacks against creatures with a #CreatureSize::kSuperLarge */
	kLargeCreatureSpell, /**< These are magical attacks against creatures with #CreatureSize::kLarge */
	kSuperLargeCreatureSpell /**< These are magical attacks against creatures with #CreatureSize::kSuperLarge */
};

/**
 * @brief Get the string representation of the given enum
 * @param type The #CriticalTableType to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(CriticalTableType type);

/**
 * @brief Teach operator<< how to print a CriticalTableType
 * @param out Output stream that the enum should be printed to
 * @param type Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, CriticalTableType type) {
	return out << toString(type);
}

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * For values that contain strings this function will accept these with the
 * space, with the space removed or with the space changed to an underscore:
 * - "Large Creature Spell"
 * - "large creature spell"
 * - "LargeCreatureSpell"
 * - "largecreaturespell"
 * - "Large_Creature_Spell"
 * - "large_creature_spell"
 *
 * @param sv String to get the enumeration for
 * @return  #CriticalTableType represented by the string
 *
 * @see toString()
 */
const std::optional<CriticalTableType> fromString(std::string_view sv);