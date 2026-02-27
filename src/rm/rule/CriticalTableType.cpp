#include "CriticalTableType.h"

const std::string CriticalTableType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kNormal: return "Normal";
	case kLargeCreaturePhysical: return "Large Creature Physical";
	case kHugeCreaturePhysical: return "Super Large Creature Physical";
	case kLargeCreatureSpell: return "Large Creature Spell";
	case kHugeCreatureSpell: return "Super Large Creature Spell";
	default: return "";
	}
}

const std::optional<CriticalTableType::Type> CriticalTableType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = archid::lcaseconcat(sv);
	if (val == "normal") return kNormal;
	if (val == "largecreaturephysical") return kLargeCreaturePhysical;
	if (val == "superlargecreaturephysical") return kHugeCreaturePhysical;
	if (val == "largecreaturespell") return kLargeCreatureSpell;
	if (val == "superlargecreaturespell") return kHugeCreatureSpell;

	return {};
}

void CriticalTableType::fromString(std::string_view sv, CriticalTableType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for CriticalTableType::Type: " + std::string(sv));
}