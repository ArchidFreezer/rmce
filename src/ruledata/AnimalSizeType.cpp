#include "AttackSizeType.h"

const std::string AttackSizeType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kSmall: return "Small";
	case kMedium: return "Medium";
	case kLarge: return "Large";
	case kHuge: return "Huge";
	default: return "";
	}
}

const std::optional<AttackSizeType::Type> AttackSizeType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = lcaseconcat(sv);
	if (val == "small") return kSmall;
	if (val == "medium") return kMedium;
	if (val == "large") return kLarge;
	if (val == "huge") return kHuge;

	return {};
}