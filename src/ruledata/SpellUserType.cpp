#include "SpellUserType.h"

const std::string SpellUserType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kPure: return "Pure";
	case kHybrid: return "Hybrid";
	case kSemi: return "Semi";
	case kNone: return "None";
	case kChaotic: return "Chaotic";
	default: return "";
	}
}

const std::optional<SpellUserType::Type> SpellUserType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = lcaseconcat(sv);
	if (val == "pure") return kPure;
	if (val == "hybrid") return kHybrid;
	if (val == "semi") return kSemi;
	if (val == "none") return kNone;
	if (val == "chaotic") return kChaotic;

	return {};
}