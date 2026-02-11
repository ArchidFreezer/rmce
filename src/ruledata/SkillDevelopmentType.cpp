#include "SkillDevelopmentType.h"

const std::string SkillDevelopmentType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kEveryman: return "Everyman";
	case kOccupational: return "Occupational";
	case kRestricted: return "Restricted";
	case kStandard: return "Standard";
	default: return "";
	}
}

const std::optional<SkillDevelopmentType::Type> SkillDevelopmentType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcaseconcat(sv);
	if (val == "everyman") return kEveryman;
	if (val == "occupational") return kOccupational;
	if (val == "restricted") return kRestricted;
	if (val == "standard") return kStandard;

	return {};
}

void SkillDevelopmentType::fromString(std::string_view sv, SkillDevelopmentType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for SkillDevelopmentType::Type: " + std::string(sv));
}