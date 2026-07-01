#include <ClimateType.h>

namespace rm::rule::enums::ClimateType {
const std::string toString(KoppenGroup type) {
	using enum KoppenGroup;
	switch (type) {
	case kTropical:
		return "Tropical";
	case kDry:
		return "Dry";
	case kTemperate:
		return "Temperate";
	case kContinental:
		return "Continental";
	case kPolar:
		return "Polar";
	default:
		return "";
	}
}
const std::optional<KoppenGroup> koppenGroup(std::string_view sv) {
	using enum KoppenGroup;
	const std::string val = archid::lcaseconcat(sv);
	if (val == "tropical")
		return kTropical;
	if (val == "dry")
		return kDry;
	if (val == "temperate")
		return kTemperate;
	if (val == "continental")
		return kContinental;
	if (val == "polar")
		return kPolar;
	return {};
}
void fromString(std::string_view sv, KoppenGroup& type) {
	std::optional<KoppenGroup> opt_type = koppenGroup(sv);
	if (opt_type)
		type = opt_type.value();
	else
		throw std::invalid_argument("Invalid string value for KoppenGroup: " + std::string(sv));
}

const std::string toString(KoppenSubGroup type) {
	using enum KoppenSubGroup;
	switch (type) {
	case kRainforest:
		return "Rainforest";
	case kMonsoon:
		return "Monsoon";
	case kSavanna:
		return "Savanna";
	case kAridDesert:
		return "Arid Desert";
	case kAridSteppe:
		return "Arid Steppe";
	case kMediterranean:
		return "Mediterranean";
	case kHumidSubtropical:
		return "Humid Subtropical";
	case kMarineWestCoast:
		return "Marine West Coast";
	case kHumidContinentalHotSummer:
		return "Humid Continental Hot Summer";
	case kHumidContinentalWarmSummer:
		return "Humid Continental Warm Summer";
	case kSubarctic:
		return "Subarctic";
	case kTundra:
		return "Tundra";
	case kIceCap:
		return "Ice Cap";
	default:
		return "";
	}
}

const std::optional<KoppenSubGroup> koppenSubGroup(std::string_view sv) {
	using enum KoppenSubGroup;
	const std::string val = archid::lcaseconcat(sv);
	if (val == "rainforest")
		return kRainforest;
	if (val == "monsoon")
		return kMonsoon;
	if (val == "savanna")
		return kSavanna;
	if (val == "ariddesert")
		return kAridDesert;
	if (val == "aridsteppe")
		return kAridSteppe;
	if (val == "mediterranean")
		return kMediterranean;
	if (val == "humidsubtropical")
		return kHumidSubtropical;
	if (val == "marinewestcoast")
		return kMarineWestCoast;
	if (val == "humidcontinentalhotsummer")
		return kHumidContinentalHotSummer;
	if (val == "humidcontinentalwarmsummer")
		return kHumidContinentalWarmSummer;
	if (val == "subarctic")
		return kSubarctic;
	if (val == "tundra")
		return kTundra;
	if (val == "icecap")
		return kIceCap;
	return {};
}

void fromString(std::string_view sv, KoppenSubGroup& type) {
	std::optional<KoppenSubGroup> opt_type = koppenSubGroup(sv);
	if (opt_type)
		type = opt_type.value();
	else
		throw std::invalid_argument("Invalid string value for KoppenSubGroup: " + std::string(sv));
}

const std::optional<KoppenGroup> koppenGroup(KoppenSubGroup& type) {
	switch (type) {
	case KoppenSubGroup::kRainforest:
	case KoppenSubGroup::kMonsoon:
	case KoppenSubGroup::kSavanna:
		return KoppenGroup::kTropical;
	case KoppenSubGroup::kAridDesert:
	case KoppenSubGroup::kAridSteppe:
		return KoppenGroup::kDry;
	case KoppenSubGroup::kMediterranean:
	case KoppenSubGroup::kHumidSubtropical:
	case KoppenSubGroup::kMarineWestCoast:
		return KoppenGroup::kTemperate;
	case KoppenSubGroup::kHumidContinentalHotSummer:
	case KoppenSubGroup::kHumidContinentalWarmSummer:
	case KoppenSubGroup::kSubarctic:
		return KoppenGroup::kContinental;
	case KoppenSubGroup::kTundra:
	case KoppenSubGroup::kIceCap:
		return KoppenGroup::kPolar;
	default:
		return {};
	}
}

bool operator==(KoppenGroup lhs, KoppenSubGroup rhs) {
	std::optional<KoppenGroup> group = koppenGroup(rhs);
	if (group)
		return lhs == group.value();
	return false;
}

} // namespace rm::rule::enums::ClimateType