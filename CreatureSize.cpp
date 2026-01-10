#include "CreatureSize.h"

const std::string toString(CreatureSize size) {
	using enum CreatureSize;

	switch (size) {
	case kTiny: return "Tiny";
	case kSmall: return "Small";
	case kMedium: return "Medium";
	case kLarge: return "Large";
	case kSuperLarge: return "Super Large";
	default: return "";
	}
}

const std::optional<CreatureSize> fromString(std::string_view sv) {
	using enum CreatureSize;

	const std::string val = lcase(sv);
	if (val == "tiny") return kTiny;
	if (val == "small") return kSmall;
	if (val == "medium") return kMedium;
	if (val == "large") return kLarge;
	if (val == "superlarge") return kSuperLarge;
	if (val == "super_large") return kSuperLarge;
	if (val == "super large") return kSuperLarge;

	return {};
}