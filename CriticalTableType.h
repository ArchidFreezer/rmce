#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

enum class CriticalTableType {
	kNormal,
	kLargeCreaturePhysical,
	kSuperLargeCreaturePhysical,
	kLargeCreatureSpell,
	kSuperLargeCreatureSpell
};

/**
* Get the string representation of the enum values
*/
constexpr std::string_view getCriticalTableType(CriticalTableType type) {
	using enum CriticalTableType;

	switch (type)
	{
	case kNormal: return "Normal";
	case kLargeCreaturePhysical: return "Large Creature Physical";
	case kSuperLargeCreaturePhysical: return "Super Large Creature Physical";
	case kLargeCreatureSpell: return "Large Creature Spell";
	case kSuperLargeCreatureSpell: return "Super Large Creature Spell";
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

	const std::string& val = toASCIILowerCase(sv);
	if (val == "normal") return kNormal;
	if (val == "largecreaturephysical") return kLargeCreaturePhysical;
	if (val == "large_creature_physical") return kLargeCreaturePhysical;
	if (val == "large creature physical") return kLargeCreaturePhysical;
	if (val == "superlargecreaturephysical") return kSuperLargeCreaturePhysical;
	if (val == "super_large_creature_physical") return kSuperLargeCreaturePhysical;
	if (val == "super large creature physical") return kSuperLargeCreaturePhysical;
	if (val == "largecreaturespell") return kLargeCreatureSpell;
	if (val == "large_creature_spell") return kLargeCreatureSpell;
	if (val == "large creature spell") return kLargeCreatureSpell;
	if (val == "superlargecreaturespell") return kSuperLargeCreatureSpell;
	if (val == "super_large_creature_spell") return kSuperLargeCreatureSpell;
	if (val == "super large creature spell") return kSuperLargeCreatureSpell;

	return {};
}
