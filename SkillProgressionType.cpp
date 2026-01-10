#include "SkillProgressionType.h"

const std::string toString(SkillProgressionType type) {
	using enum SkillProgressionType;

	switch (type) {
	case kCombined: return "Combined";
	case kLimited: return "Limited";
	case kNone: return "None";
	case kSpecial: return "Special";
	case kStandard: return "Standard";
	default: return "";
	}
}

const std::optional<SkillProgressionType> fromString(std::string_view sv) {
	using enum SkillProgressionType;

	const std::string& val = lcase(sv);
	if (val == "combined") return kCombined;
	if (val == "limited") return kLimited;
	if (val == "none") return kNone;
	if (val == "special") return kSpecial;
	if (val == "standard") return kStandard;

	return {};
}