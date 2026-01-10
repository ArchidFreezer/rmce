#include "CriticalTableType.h"

const std::string toString(CriticalTableType type) {
	using enum CriticalTableType;

	switch (type) {
	case kNormal: return "Normal";
	case kLargeCreaturePhysical: return "Large Creature Physical";
	case kSuperLargeCreaturePhysical: return "Super Large Creature Physical";
	case kLargeCreatureSpell: return "Large Creature Spell";
	case kSuperLargeCreatureSpell: return "Super Large Creature Spell";
	default: return "";
	}
}

const std::optional<CriticalTableType> fromString(std::string_view sv) {
	using enum CriticalTableType;

	const std::string& val = lcase(sv);
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