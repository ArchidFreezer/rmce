#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

enum class CriticalType {
	brawling,
	grapple,
	impact,
	krush,
	martial_arts_strikes,
	martial_arts_sweeps,
	puncture,
	slash,
	subdual,
	tiny,
	unbalance,
	aether,
	cold,
	electrical,
	heat,
	nether
};

/**
* Get the string representation of the enum values
*/
constexpr std::string_view getCriticalType(CriticalType type) {
	using enum CriticalType;

	switch (type)
	{
	case brawling: return "Brawling";
	case grapple: return "Grapple";
	case impact: return "Impact";
	case krush: return "Krush";
	case martial_arts_strikes: return "Martial Arts Strikes";
	case martial_arts_sweeps: return "Martial Arts Sweeps";
	case puncture: return "Puncture";
	case slash: return "Slash";
	case subdual: return "Subdual";
	case tiny: return "Tiny";
	case unbalance: return "Unbalance";
	case aether: return "Aether";
	case cold: return "Cold";
	case electrical: return "Electrical";
	case heat: return "Heat";
	case nether: return "Nether";
	}
}

/**
* Teach operator<< how to print a CriticalType
*/
std::ostream& operator<<(std::ostream& out, CriticalType type) {
	return out << getCriticalType(type);
}

/**
* Get the enumeration value based on a string
* Accepts both lower case and mixed case strings
*/
constexpr std::optional<CriticalType> getCriticalTypeFromString(std::string_view sv) {
	using enum CriticalType;

	std::string val = toASCIILowerCase(sv);
	if (val == "brawling") return brawling;
	if (val == "grapple") return grapple;
	if (val == "impact") return impact;
	if (val == "krush") return krush;
	if (val == "martialartsstrikes") return martial_arts_strikes;
	if (val == "martial_arts_strikes") return martial_arts_strikes;
	if (val == "martial arts strikes") return martial_arts_strikes;
	if (val == "martialartssweeps") return martial_arts_sweeps;
	if (val == "martial_arts_sweeps") return martial_arts_sweeps;
	if (val == "martial arts sweeps") return martial_arts_sweeps;
	if (val == "puncture") return puncture;
	if (val == "slash") return slash;
	if (val == "subdual") return subdual;
	if (val == "tiny") return tiny;
	if (val == "unbalance") return unbalance;
	if (val == "aether") return aether;
	if (val == "cold") return cold;
	if (val == "electrical") return electrical;
	if (val == "heat") return heat;
	if (val == "nether") return nether;

	return {};
}
