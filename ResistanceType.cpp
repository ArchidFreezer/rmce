#include "ResistanceType.h"

const std::string ResistanceType::toString(Type resistance) {
	using enum Type;

	switch (resistance) {
	case kArcane: return "Arcane";
	case kChanneling: return "Channeling";
	case kChannelingEssence: return "Channeling / Essence";
	case kChannelingMentalism: return "Channeling / Mentalism";
	case kCold: return "Cold";
	case kDisease: return "Disease";
	case kEssence: return "Essence";
	case kEssenceMentalism: return "Essence / Mentalism";
	case kFear: return "Fear";
	case kHeat: return "Heat";
	case kMentalism: return "Mentalism";
	case kPoison: return "Poison";
	default: return "";
	}
}

const std::optional<ResistanceType::Type> ResistanceType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcase(sv);
	if (val == "Arcane") return kArcane;
	if (val == "Channeling") return kChanneling;
	if (val == "Channeling/Essence") return kChannelingEssence;
	if (val == "Channeling Essence") return kChannelingEssence;
	if (val == "ChannelingEssence") return kChannelingEssence;
	if (val == "Channeling_Essence") return kChannelingEssence;
	if (val == "Channeling/Mentalism") return kChannelingMentalism;
	if (val == "Channeling Mentalism") return kChannelingMentalism;
	if (val == "ChannelingMentalism") return kChannelingMentalism;
	if (val == "Channeling_Mentalism") return kChannelingMentalism;
	if (val == "Cold") return kCold;
	if (val == "Disease") return kDisease;
	if (val == "Essence") return kEssence;
	if (val == "Essence/Mentalism") return kEssenceMentalism;
	if (val == "Essence Mentalism") return kEssenceMentalism;
	if (val == "EssenceMentalism") return kEssenceMentalism;
	if (val == "Essence_Mentalism") return kEssenceMentalism;
	if (val == "Fear") return kFear;
	if (val == "Heat") return kHeat;
	if (val == "Mentalism") return kMentalism;
	if (val == "Poison") return kPoison;

	return {};
}

constexpr bool ResistanceType::isMagical(Type resistance) {
	using enum Type;

	switch (resistance) {
	case kCold:
	case kDisease:
	case kFear:
	case kHeat:
	case kPoison: return false;
	default: return true;
	}
}