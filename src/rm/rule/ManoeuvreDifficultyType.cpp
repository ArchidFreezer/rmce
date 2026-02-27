#include <ManoeuvreDifficultyType.h>

const std::string ManoeuvreDifficultyType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kNormal: return "Normal";
	case kRoutine: return "Routine";
	case kEasy: return "Easy";
	case kLight: return "Light";
	case kMedium: return "Medium";
	case kHard: return "Hard";
	case kVeryHard: return "Very Hard";
	case kExtremelyHard: return "Extremely Hard";
	case kSheerFolly: return "Sheer Folly";
	case kAbsurd: return "Absurd";
	default: return "";
	}
}

const std::optional<ManoeuvreDifficultyType::Type> ManoeuvreDifficultyType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = lcaseconcat(sv);
	if (val == "normal") return kNormal;
	if (val == "routine") return kRoutine;
	if (val == "easy") return kEasy;
	if (val == "light") return kLight;
	if (val == "medium") return kMedium;
	if (val == "hard") return kHard;
	if (val == "veryhard") return kVeryHard;
	if (val == "extremelyhard") return kExtremelyHard;
	if (val == "sheerfolly") return kSheerFolly;
	if (val == "absurd") return kAbsurd;

	return {};
}

void ManoeuvreDifficultyType::fromString(std::string_view sv, ManoeuvreDifficultyType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for ManoeuvreDifficultyType::Type: " + std::string(sv));
}