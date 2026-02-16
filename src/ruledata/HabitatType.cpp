#include "HabitatType.h"

const std::string HabitatType::toString(HabitatType::Temperature temperature) {
	using enum HabitatType::Temperature;

	switch (temperature) {
	case kHot: return "Hot";
	case kWarm: return "Warm";
	case kTemperate: return "Temperate";
	case kCool: return "Cool";
	case kCold: return "Cold";
	default: return "";
	}
}

const std::optional<HabitatType::Temperature> HabitatType::temperature(std::string_view sv) {
	using enum HabitatType::Temperature;

	const std::string val = lcaseconcat(sv);
	if (val == "hot") return kHot;
	if (val == "warm") return kWarm;
	if (val == "temperate") return kTemperate;
	if (val == "cool") return kCool;
	if (val == "cold") return kCold;

	return {};
}

void HabitatType::fromString(std::string_view sv, HabitatType::Temperature& type) {
	std::optional<Temperature> opt_type = temperature(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for HabitatType::Temperature: " + std::string(sv));
}

const std::string HabitatType::toString(HabitatType::Precipitation precipitation) {
	using enum HabitatType::Precipitation;

	switch (precipitation) {
	case kRainy: return "Rainy";
	case kHumid: return "Humid";
	case kTemperate: return "Temperate";
	case kDry: return "Dry";
	case kArid: return "Arid";
	default: return "";
	}
}

const std::optional<HabitatType::Precipitation> HabitatType::precipitation(std::string_view sv) {
	using enum HabitatType::Precipitation;

	const std::string val = lcaseconcat(sv);
	if (val == "rainy") return kRainy;
	if (val == "humid") return kHumid;
	if (val == "temperate") return kTemperate;
	if (val == "dry") return kDry;
	if (val == "arid") return kArid;

	return {};
}

void HabitatType::fromString(std::string_view sv, HabitatType::Precipitation& type) {
	std::optional<Precipitation> opt_type = precipitation(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for HabitatType::Precipitation: " + std::string(sv));
}
