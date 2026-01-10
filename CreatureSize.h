#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @file CreatureSize.h
 * 
 * Contains the CreatureSize enum and associated functions
 */

/** 
 * @brief Enumeration class representing the size of creatures
 * 
 * During combat and in other situations the size of a creature determines the lookup table to use. This enum defines the
 * valid sizes that there are tables for.
 */
enum class CreatureSize {
	kTiny, /**< Tiny creatures are insect sized */
	kSmall, /**< Small creatures are cat or dog sized */
	kMedium, /**< Medium creatures are approximately human sized such as deer or most sharks */
	kLarge, /**< Large creatures are buffalo or lions */
	kSuperLarge /**< Super Large or Huge creatures are elephant or whale sized */
};

/**
 * @brief Get the string representation of the given enum
 * @param size The #CreatureSize to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(CreatureSize size);

/**
 * @brief Teach operator<< how to print a CreatureSize
 * @param out Output stream that the enum should be printed to
 * @param size Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, CreatureSize size) {
	return out << toString(size);
}

/**
 * @brief Get the enumeration value based on a string
 * 
 * Accepts values that are returned by toString() in a case insensitive manner.
 * 
 * For values that contain strings this function will accept these with the
 * space, with the space removed or with the space changed to an underscore:
 * - "Super Large"
 * - "super large"
 * - "SuperLarge"
 * - "superlarge"
 * - "Super_Large"
 * - "super_large"
 * 
 * @param sv string_view to get the enumeration for
 * @return #CreatureSize represented by the string
 * 
 * @see toString()
 */
const std::optional<CreatureSize> fromString(std::string_view sv);
