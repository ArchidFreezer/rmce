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
	kTiny,
	kSmall,
	kMedium,
	kLarge,
	kSuperLarge
};

/**
 * @brief Get the string representation of the given enum
 * @param size The #CreatureSize to get the string of
 * @return Game display form of the enum value as a string_view
 */
constexpr std::string_view toString(CreatureSize size) {
	using enum CreatureSize;

	switch (size)
	{
	case kTiny: return "Tiny";
	case kSmall: return "Small";
	case kMedium: return "Medium";
	case kLarge: return "Large";
	case kSuperLarge: return "Super Large";
	}
}

/**
 * @brief Teach operator<< how to print a CreatureSize
 * @param out Output stream that the enum should be printed to
 * @param size Enum value to output
 * @return Output stream reference containing the output enum value
 */
std::ostream& operator<<(std::ostream& out, CreatureSize size) {
	return out << toString(size);
}

/**
 * @brief Get the enumeration value based on a string
 * 
 * Accepts values that are returned by the toString() in a case insensitive manner.
 * 
 * For values that contain strings thie function will accept these with the
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
constexpr std::optional<CreatureSize> fromString(std::string_view sv) {
	using enum CreatureSize;

	const std::string& val = lcase(sv);
	if (val == "tiny") return kTiny;
	if (val == "small") return kSmall;
	if (val == "medium") return kMedium;
	if (val == "large") return kLarge;
	if (val == "superlarge") return kSuperLarge;
	if (val == "super_large") return kSuperLarge;
	if (val == "super large") return kSuperLarge;

	return {};
}
