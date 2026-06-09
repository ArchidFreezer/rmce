#include <SkillDifficultyType.h>

namespace rm::rule::enums {

const std::string SkillDifficultyType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kRoutine:
		return "Routine";
	case kEasy:
		return "Easy";
	case kLight:
		return "Light";
	case kMedium:
		return "Medium";
	case kHard:
		return "Hard";
	case kVeryHard:
		return "Very Hard";
	case kExtremelyHard:
		return "Extremely Hard";
	case kSheerFolly:
		return "Sheer Folly";
	case kAbsurd:
		return "Absurd";
	default:
		return "";
	}
}

const std::optional<SkillDifficultyType::Type> SkillDifficultyType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = archid::lcaseconcat(sv);
	if (val == "routine")
		return kRoutine;
	if (val == "easy")
		return kEasy;
	if (val == "light")
		return kLight;
	if (val == "medium")
		return kMedium;
	if (val == "hard")
		return kHard;
	if (val == "veryhard")
		return kVeryHard;
	if (val == "extremelyhard")
		return kExtremelyHard;
	if (val == "sheerfolly")
		return kSheerFolly;
	if (val == "absurd")
		return kAbsurd;

	return {};
}

void SkillDifficultyType::fromString(std::string_view sv, SkillDifficultyType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type)
		type = opt_type.value();
	else
		throw std::invalid_argument("Invalid string value for SkillDifficultyType::Type: " + std::string(sv));
}

} // namespace rm::rule::enums