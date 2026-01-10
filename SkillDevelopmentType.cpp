#include "SkillDevelopmentType.h"

const std::string toString(SkillDevelopmentType type) {
	using enum SkillDevelopmentType;

	switch (type) {
	case kEveryman: return "Everyman";
	case kOccupational: return "Occupational";
	case kRestricted: return "Restricted";
	case kStandard: return "Standard";
	default: return "";
	}
}

const std::optional<SkillDevelopmentType> fromString(std::string_view sv) {
	using enum SkillDevelopmentType;

	const std::string& val = lcase(sv);
	if (val == "everyman") return kEveryman;
	if (val == "occupational") return kOccupational;
	if (val == "restricted") return kRestricted;
	if (val == "standard") return kStandard;

	return {};
}