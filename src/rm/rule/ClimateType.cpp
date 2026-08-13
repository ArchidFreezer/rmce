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
	case kHighland:
		return "Highland";
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
	if (val == "highland")
		return kHighland;
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
	case kAridDesertCold:
		return "Arid Desert Cold";
	case kAridDesertHot:
		return "Arid Desert Hot";
	case kAridSteppeCold:
		return "Arid Steppe Cold";
	case kAridSteppeHot:
		return "Arid Steppe Hot";
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
	case kHighland:
		return "Highland";
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
	if (val == "ariddesertcold")
		return kAridDesertCold;
	if (val == "ariddeserthot")
		return kAridDesertHot;
	if (val == "aridsteppecold")
		return kAridSteppeCold;
	if (val == "aridsteppehot")
		return kAridSteppeHot;
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
	if (val == "highland")
		return kHighland;
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
	case KoppenSubGroup::kAridDesertCold:
	case KoppenSubGroup::kAridDesertHot:
	case KoppenSubGroup::kAridSteppeCold:
	case KoppenSubGroup::kAridSteppeHot:
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
	case KoppenSubGroup::kHighland:
		return KoppenGroup::kHighland;
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