#include "RealmType.h"

const std::string RealmType::toString(Type realm) {
	using enum Type;

	switch (realm) {
	case kArcane: return "Arcane";
	case kArms: return "Arms";
	case kChanneling: return "Channeling";
	case kChannelingEssence: return "Channeling / Essence";
	case kChannelingMentalism: return "Channeling / Mentalism";
	case kEssence: return "Essence";
	case kEssenceMentalism: return "Essence / Mentalism";
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
	if (val == "channelingessence") return kChannelingEssence;
	if (val == "channelingmentalism") return kChannelingMentalism;
	if (val == "essence") return kEssence;
	if (val == "essencementalism") return kEssenceMentalism;
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