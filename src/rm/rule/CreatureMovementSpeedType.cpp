#include <CreatureMovementSpeedType.h>

namespace rm::rule::enums {

	const std::string CreatureMovementSpeedType::toString(Type size) {
		using enum Type;

		switch (size) {
		case kImmobile: return "Immobile";
		case kInching: return "Inching";
		case kCreeping: return "Creeping";
		case kVerySlow: return "Very Slow";
		case kSlow: return "Slow";
		case kMedium: return "Medium";
		case kModeratelyFast: return "Moderately Fast";
		case kFast: return "Fast";
		case kVeryFast: return "Very Fast";
		case kBlindinglyFast: return "Blindingly Fast";
		default: return "";
		}
	}

	const std::optional<CreatureMovementSpeedType::Type> CreatureMovementSpeedType::fromString(std::string_view sv) {
		using enum Type;

		const std::string val = archid::lcaseconcat(sv);
		if (val == "immobile") return kImmobile;
		if (val == "inching") return kInching;
		if (val == "creeping") return kCreeping;
		if (val == "veryslow") return kVerySlow;
		if (val == "slow") return kSlow;
		if (val == "medium") return kMedium;
		if (val == "moderatelyfast") return kModeratelyFast;
		if (val == "fast") return kFast;
		if (val == "veryfast") return kVeryFast;
		if (val == "blindinglyfast") return kBlindinglyFast;

		return {};
	}

	void CreatureMovementSpeedType::fromString(std::string_view sv, CreatureMovementSpeedType::Type& type) {
		std::optional<Type> opt_type = fromString(sv);
		if (opt_type) type = opt_type.value();
		else throw std::invalid_argument("Invalid string value for CreatureMovementSpeedType::Type: " + std::string(sv));
	}

} // namespace rm::rule::enums