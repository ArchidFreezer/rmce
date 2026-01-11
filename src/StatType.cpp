#include "StatType.h"

const std::string StatType::toString(Type stat) {
	using enum Type;

	switch (stat) {
	case kAgility: return "Agility";
	case kConstitution: return "Constitution";
	case kEmpathy: return "Empathy";
	case kIntuition: return "Intuition";
	case kMemory: return "Memory";
	case kPresence: return "Presence";
	case kQuickness: return "Quickness";
	case kReasoning: return "Reasoning";
	case kSelfDiscipline: return "Self Discipline";
	case kStrength: return "Strength";
	default: return "";
	}
}

const std::optional<StatType::Type> StatType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcase(sv);
	if (val == "agility") return kAgility;
	if (val == "constitution") return kConstitution;
	if (val == "empathy") return kEmpathy;
	if (val == "intuition") return kIntuition;
	if (val == "memory") return kMemory;
	if (val == "presence") return kPresence;
	if (val == "quickness") return kQuickness;
	if (val == "reasoning") return kReasoning;
	if (val == "selfdiscipline") return kSelfDiscipline;
	if (val == "self_discipline") return kSelfDiscipline;
	if (val == "self discipline") return kSelfDiscipline;
	if (val == "strength") return kStrength;

	return {};
}

constexpr bool StatType::isPrimary(Type stat) {
	using enum Type;

	switch (stat) {
	case kEmpathy:
	case kIntuition:
	case kPresence:
	case kQuickness:
	case kStrength: return true;
	default: return false;
	}
}

constexpr bool StatType::isDevelopment(Type stat) {
	using enum Type;

	switch (stat) {
	case kAgility:
	case kConstitution:
	case kMemory:
	case kReasoning:
	case kSelfDiscipline: return true;
	default: return false;
	}
}
