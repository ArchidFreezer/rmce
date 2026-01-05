#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

enum class CreatureSize {
	kTiny,
	kSmall,
	kMedium,
	kLarge,
	kSuperLarge
};

/**
* Get the string representation of the enum values
*/
constexpr std::string_view getCreatureSize(CreatureSize size) {
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
* Teach operator<< how to print a CreatureSize
*/
std::ostream& operator<<(std::ostream& out, CreatureSize size) {
	return out << getCreatureSize(size);
}

/**
* Get the enumeration value based on a string
* Accepts both lower case and mixed case strings
*/
constexpr std::optional<CreatureSize> getCreatureSizeFromString(std::string_view sv) {
	using enum CreatureSize;

	const std::string& val = toASCIILowerCase(sv);
	if (val == "tiny") return kTiny;
	if (val == "small") return kSmall;
	if (val == "medium") return kMedium;
	if (val == "large") return kLarge;
	if (val == "superlarge") return kSuperLarge;
	if (val == "super_large") return kSuperLarge;
	if (val == "super large") return kSuperLarge;

	return {};
}
