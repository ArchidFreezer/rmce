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

	const std::string& val = archid::lcaseconcat(sv);
	if (val == "arcane") return kArcane;
	if (val == "channeling") return kChanneling;
	if (val == "channelingessence") return kChannelingEssence;
	if (val == "channelingmentalism") return kChannelingMentalism;
	if (val == "cold") return kCold;
	if (val == "disease") return kDisease;
	if (val == "essence") return kEssence;
	if (val == "essencementalism") return kEssenceMentalism;
	if (val == "fear") return kFear;
	if (val == "heat") return kHeat;
	if (val == "mentalism") return kMentalism;
	if (val == "poison") return kPoison;

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