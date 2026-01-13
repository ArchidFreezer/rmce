#include "CreatureSizeType.h"

const std::string CreatureSizeType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kTiny: return "Tiny";
	case kSmall: return "Small";
	case kMedium: return "Medium";
	case kLarge: return "Large";
	case kSuperLarge: return "Super Large";
	default: return "";
	}
}

const std::optional<CreatureSizeType::Type> CreatureSizeType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = lcaseconcat(sv);
	if (val == "tiny") return kTiny;
	if (val == "small") return kSmall;
	if (val == "medium") return kMedium;
	if (val == "large") return kLarge;
	if (val == "superlarge") return kSuperLarge;

	return {};
}