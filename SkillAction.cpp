#include "SkillAction.h"

const std::string toString(SkillAction action) {
	using enum SkillAction;

	switch (action) {
	case kMoving: return "Moving";
	case kOB: return "OB";
	case kSpecial: return "Special";
	case kStatic: return "Static";
	default: return "";
	}
}

const std::optional<SkillAction> fromString(std::string_view sv) {
	using enum SkillAction;

	const std::string& val = lcase(sv);
	if (val == "moving") return kMoving;
	if (val == "ob") return kOB;
	if (val == "special") return kSpecial;
	if (val == "static") return kStatic;

	return {};
}