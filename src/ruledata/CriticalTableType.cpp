#include "CriticalTableType.h"

const std::string CriticalTableType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kNormal: return "Normal";
	case kLargeCreaturePhysical: return "Large Creature Physical";
	case kSuperLargeCreaturePhysical: return "Super Large Creature Physical";
	case kLargeCreatureSpell: return "Large Creature Spell";
	case kSuperLargeCreatureSpell: return "Super Large Creature Spell";
	default: return "";
	}
}

const std::optional<CriticalTableType::Type> CriticalTableType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcaseconcat(sv);
	if (val == "normal") return kNormal;
	if (val == "largecreaturephysical") return kLargeCreaturePhysical;
	if (val == "superlargecreaturephysical") return kSuperLargeCreaturePhysical;
	if (val == "largecreaturespell") return kLargeCreatureSpell;
	if (val == "superlargecreaturespell") return kSuperLargeCreatureSpell;

	return {};
}