#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

enum class CreatureSize {
	tiny,
	small,
	medium,
	large,
	super_large
};

/**
* Get the string representation of the enum values
*/
constexpr std::string_view getCreatureSize(CreatureSize size) {
	using enum CreatureSize;

	switch (size)
	{
	case tiny: return "Tiny";
	case small: return "Small";
	case medium: return "Medium";
	case large: return "Large";
	case super_large: return "Super Large";
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

	std::string val = toASCIILowerCase(sv);
	if (val == "tiny") return tiny;
	if (val == "small") return small;
	if (val == "medium") return medium;
	if (val == "large") return large;
	if (val == "superlarge") return super_large;
	if (val == "super_large") return super_large;
	if (val == "super large") return super_large;

	return {};
}
