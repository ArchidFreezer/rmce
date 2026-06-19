#include <SkillDifficultyType.h>

namespace rm::rule::enums {

const std::string SkillDifficultyType::toString(Type type) {
	using enum SkillDifficultyType::Type;

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
	using enum SkillDifficultyType::Type;

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

int difficultyModifier(SkillDifficultyType::Type type) {
	using enum SkillDifficultyType::Type;
	switch (type) {
	case kRoutine:
		return 30;
	case kEasy:
		return 20;
	case kLight:
		return 10;
	case kMedium:
		return 0;
	case kHard:
		return -10;
	case kVeryHard:
		return -20;
	case kExtremelyHard:
		return -30;
	case kSheerFolly:
		return -50;
	case kAbsurd:
		return -70;
	default:
		throw std::invalid_argument("Invalid SkillDifficultyType::Type value");
	}
}

int difficultyRating(SkillDifficultyType::Type type) {
	using enum SkillDifficultyType::Type;
	switch (type) {
	case kRoutine:
		return 1;
	case kEasy:
		return 2;
	case kLight:
		return 3;
	case kMedium:
		return 4;
	case kHard:
		return 5;
	case kVeryHard:
		return 6;
	case kExtremelyHard:
		return 7;
	case kSheerFolly:
		return 8;
	case kAbsurd:
		return 9;
	default:
		throw std::invalid_argument("Invalid SkillDifficultyType::Type value");
	}
}

SkillDifficultyType::Type incrementDifficulty(SkillDifficultyType::Type type, int increment) {
	using enum SkillDifficultyType::Type;
	int new_difficulty = static_cast<int>(type) + increment;
	if (new_difficulty > static_cast<int>(kAbsurd)) {
		new_difficulty = static_cast<int>(kAbsurd);
	} else if (new_difficulty < static_cast<int>(kRoutine)) {
		new_difficulty = static_cast<int>(kRoutine);
	}
	return static_cast<SkillDifficultyType::Type>(new_difficulty);
}

} // namespace rm::rule::enums