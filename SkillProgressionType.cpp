#include "SkillProgressionType.h"

const std::string SkillProgressionType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kCombined: return "Combined";
	case kLimited: return "Limited";
	case kNone: return "None";
	case kSpecial: return "Special";
	case kStandard: return "Standard";
	default: return "";
	}
}

const std::optional<SkillProgressionType::Type> SkillProgressionType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcase(sv);
	if (val == "combined") return kCombined;
	if (val == "limited") return kLimited;
	if (val == "none") return kNone;
	if (val == "special") return kSpecial;
	if (val == "standard") return kStandard;

	return {};
}