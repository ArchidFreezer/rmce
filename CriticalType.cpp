#include "CriticalType.h"

const std::string toString(CriticalType type) {
	using enum CriticalType;

	switch (type) {
	case kBrawling: return "Brawling";
	case kGrapple: return "Grapple";
	case kImpact: return "Impact";
	case kKrush: return "Krush";
	case kMartialArtsStrikes: return "Martial Arts Strikes";
	case kMartialArtsSweeps: return "Martial Arts Sweeps";
	case kPuncture: return "Puncture";
	case kSlash: return "Slash";
	case kSubdual: return "Subdual";
	case kTiny: return "Tiny";
	case kUnbalance: return "Unbalance";
	case kAether: return "Aether";
	case kCold: return "Cold";
	case kElectrical: return "Electrical";
	case kHeat: return "Heat";
	case kNether: return "Nether";
	default: return "";
	}
}

const std::optional<CriticalType> fromString(std::string_view sv) {
	using enum CriticalType;

	const std::string& val = lcase(sv);
	if (val == "brawling") return kBrawling;
	if (val == "grapple") return kGrapple;
	if (val == "impact") return kImpact;
	if (val == "krush") return kKrush;
	if (val == "martialartsstrikes") return kMartialArtsStrikes;
	if (val == "martial_arts_strikes") return kMartialArtsStrikes;
	if (val == "martial arts strikes") return kMartialArtsStrikes;
	if (val == "martialartssweeps") return kMartialArtsSweeps;
	if (val == "martial_arts_sweeps") return kMartialArtsSweeps;
	if (val == "martial arts sweeps") return kMartialArtsSweeps;
	if (val == "puncture") return kPuncture;
	if (val == "slash") return kSlash;
	if (val == "subdual") return kSubdual;
	if (val == "tiny") return kTiny;
	if (val == "unbalance") return kUnbalance;
	if (val == "aether") return kAether;
	if (val == "cold") return kCold;
	if (val == "electrical") return kElectrical;
	if (val == "heat") return kHeat;
	if (val == "nether") return kNether;

	return {};
}