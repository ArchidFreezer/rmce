#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

enum class CriticalTableType {
	normal,
	large_creature_physical,
	super_large_creature_physical,
	large_creature_spell,
	super_large_creature_spell
};

/**
* Get the string representation of the enum values
*/
constexpr std::string_view getCriticalTableType(CriticalTableType type) {
	using enum CriticalTableType;

	switch (type)
	{
	case normal: return "Normal";
	case large_creature_physical: return "Large Creature Physical";
	case super_large_creature_physical: return "Super Large Creature Physical";
	case large_creature_spell: return "Large Creature Spell";
	case super_large_creature_spell: return "Super Large Creature Spell";
	}
}

/**
* Teach operator<< how to print a CriticalTableType
*/
std::ostream& operator<<(std::ostream& out, CriticalTableType type) {
	return out << getCriticalTableType(type);
}

/**
* Get the enumeration value based on a string
* Accepts both lower case and mixed case strings
*/
constexpr std::optional<CriticalTableType> getCriticalTableTypeFromString(std::string_view sv) {
	using enum CriticalTableType;

	std::string val = toASCIILowerCase(sv);
	if (val == "normal") return normal;
	if (val == "largecreaturephysical") return large_creature_physical;
	if (val == "large_creature_physical") return large_creature_physical;
	if (val == "large creature physical") return large_creature_physical;
	if (val == "superlargecreaturephysical") return super_large_creature_physical;
	if (val == "super_large_creature_physical") return super_large_creature_physical;
	if (val == "super large creature physical") return super_large_creature_physical;
	if (val == "largecreaturespell") return large_creature_spell;
	if (val == "large_creature_spell") return large_creature_spell;
	if (val == "large creature spell") return large_creature_spell;
	if (val == "superlargecreaturespell") return super_large_creature_spell;
	if (val == "super_large_creature_spell") return super_large_creature_spell;
	if (val == "super large creature spell") return super_large_creature_spell;

	return {};
}
