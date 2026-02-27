#include "SkillActionType.h"

const std::string SkillActionType::toString(Type action) {
	using enum Type;

	switch (action) {
	case kMoving: return "Moving";
	case kOB: return "OB";
	case kSpecial: return "Special";
	case kStatic: return "Static";
	default: return "";
	}
}

const std::optional<SkillActionType::Type> SkillActionType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = archid::lcaseconcat(sv);
	if (val == "moving") return kMoving;
	if (val == "ob") return kOB;
	if (val == "special") return kSpecial;
	if (val == "static") return kStatic;

	return {};
}


void SkillActionType::fromString(std::string_view sv, SkillActionType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for SkillActionType::Type: " + std::string(sv));
}