#include <CriticalModifierType.h>

const std::string CriticalModifierType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kDecreaseSeverityI: return "Decrease Seeverity I";
	case kDecreaseSeverityII: return "Decrease Seeverity II";
	case kNoBleed: return "No Bleed";
	case kNoStun: return "No Stun";
	default: return "";
	}
}

const std::optional<CriticalModifierType::Type> CriticalModifierType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = archid::lcaseconcat(sv);
	if (val == "decreaseseverityi") return kDecreaseSeverityI;
	if (val == "decreaseseverityii") return kDecreaseSeverityII;
	if (val == "nobleed") return kNoBleed;
	if (val == "nostun") return kNoStun;

	return {};
}

void CriticalModifierType::fromString(std::string_view sv, CriticalModifierType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for CriticalModifierType::Type: " + std::string(sv));
}