#include <StatLossType.h>

namespace rm::rule::enums {

	const std::string StatLossType::toString(Type type) {
		using enum Type;

		switch (type) {
		case kSlow: return "Slow";
		case kNormal: return "Normal";
		case kFast: return "Fast";
		case kVeryFast: return "Very Fast";
		case kExtreme: return "Extreme";
		default: return "";
		}
	}

	const std::optional<StatLossType::Type> StatLossType::fromString(std::string_view sv) {
		using enum Type;

		const std::string& val = archid::lcaseconcat(sv);
		if (val == "slow") return kSlow;
		if (val == "normal") return kNormal;
		if (val == "fast") return kFast;
		if (val == "veryfast") return kVeryFast;
		if (val == "extreme") return kExtreme;

		return {};
	}

} // namespace rm::rule::enums