#include "RealmType.h"

const std::string RealmType::toString(Type realm) {
	using enum Type;

	switch (realm) {
	case kArcane: return "Arcane";
	case kArms: return "Arms";
	case kChanneling: return "Channeling";
	case kEssence: return "Essence";
	case kMentalism: return "Mentalism";
	case kMundane: return "Mundane";
	case kNeutral: return "Neutral";
	case kSubterfuge: return "Subterfuge";
	default: return "";
	}
}

const std::optional<RealmType::Type> RealmType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcaseconcat(sv);
	if (val == "arcane") return kArcane;
	if (val == "arms") return kArms;
	if (val == "channeling") return kChanneling;
	if (val == "essence") return kEssence;
	if (val == "mentalism") return kMentalism;
	if (val == "mundane") return kMundane;
	if (val == "neutral") return kNeutral;
	if (val == "subterfuge") return kSubterfuge;

	return {};
}

constexpr bool RealmType::isMagical(Type realm) {
	using enum Type;

	switch (realm) {
	case kArms:
	case kSubterfuge: return false;
	default: return true;
	}
}

void RealmType::fromString(std::string_view sv, RealmType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for RealmType::Type: " + std::string(sv));
}