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

	const std::string& val = lcase(sv);
	if (val == "Arcane") return kArcane;
	if (val == "Arms") return kArms;
	if (val == "Channeling") return kChanneling;
	if (val == "Channeling / Essence") return kChannelingEssence;
	if (val == "Channeling Essence") return kChannelingEssence;
	if (val == "ChannelingEssence") return kChannelingEssence;
	if (val == "Channeling_Essence") return kChannelingEssence;
	if (val == "Channeling / Mentalism") return kChannelingMentalism;
	if (val == "Channeling Mentalism") return kChannelingMentalism;
	if (val == "ChannelingMentalism") return kChannelingMentalism;
	if (val == "Channeling_Mentalism") return kChannelingMentalism;
	if (val == "Essence") return kEssence;
	if (val == "Essence / Mentalism") return kEssenceMentalism;
	if (val == "Essence Mentalism") return kEssenceMentalism;
	if (val == "EssenceMentalism") return kEssenceMentalism;
	if (val == "Essence_Mentalism") return kEssenceMentalism;
	if (val == "Mentalism") return kMentalism;
	if (val == "Mundane") return kMundane;
	if (val == "Neutral") return kNeutral;
	if (val == "Subterfuge") return kSubterfuge;

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