#include "StatLossType.h"

const std::string toString(StatLossType type) {
	using enum StatLossType;

	switch (type) {
	case kSlow: return "Slow";
	case kNormal: return "Normal";
	case kFast: return "Fast";
	case kVeryFast: return "Very Fast";
	case kExtreme: return "Extreme";
	default: return "";
	}
}

const std::optional<StatLossType> fromString(std::string_view sv) {
	using enum StatLossType;

	const std::string& val = lcase(sv);
	if (val == "slow") return kSlow;
	if (val == "normal") return kNormal;
	if (val == "fast") return kFast;
	if (val == "veryfast") return kVeryFast;
	if (val == "very_fast") return kVeryFast;
	if (val == "very fast") return kVeryFast;
	if (val == "extreme") return kExtreme;

	return {};
}