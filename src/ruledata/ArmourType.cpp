#include <ArmourType.h>

const std::string ArmourType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kAT1: return "AT 1";
	case kAT2: return "AT 2";
	case kAT3: return "AT 3";
	case kAT4: return "AT 4";
	case kAT5: return "AT 5";
	case kAT6: return "AT 6";
	case kAT7: return "AT 7";
	case kAT8: return "AT 8";
	case kAT9: return "AT 9";
	case kAT10: return "AT 10";
	case kAT11: return "AT 11";
	case kAT12: return "AT 12";
	case kAT13: return "AT 13";
	case kAT14: return "AT 14";
	case kAT15: return "AT 15";
	case kAT16: return "AT 16";
	case kAT17: return "AT 17";
	case kAT18: return "AT 18";
	case kAT19: return "AT 19";
	case kAT20: return "AT 20";
	default: return "";
	}
}

const std::optional<ArmourType::Type> ArmourType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = lcaseconcat(sv);
	if (val == "at1") return kAT1;
	if (val == "at2") return kAT2;
	if (val == "at3") return kAT3;
	if (val == "at4") return kAT4;
	if (val == "at5") return kAT5;
	if (val == "at6") return kAT6;
	if (val == "at7") return kAT7;
	if (val == "at8") return kAT8;
	if (val == "at9") return kAT9;
	if (val == "at10") return kAT10;
	if (val == "at11") return kAT11;
	if (val == "at12") return kAT12;
	if (val == "at13") return kAT13;
	if (val == "at14") return kAT14;
	if (val == "at15") return kAT15;
	if (val == "at16") return kAT16;
	if (val == "at17") return kAT17;
	if (val == "at18") return kAT18;
	if (val == "at19") return kAT19;
	if (val == "at20") return kAT20;

	return {};
}
