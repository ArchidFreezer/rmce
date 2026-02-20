#include <PoisonType.h>

const std::string PoisonType::toString(Type type) {
	switch (type) {
	case kCirculatory: return "Circulatory";
	case kConversion:	return "Conversion";
	case kMuscle:	return "Muscle";
	case kNerve: return "Nerve";
	case kReduction: return "Reduction";
	case kRespiratory: return "Respiratory";
	default:
		throw std::invalid_argument("Invalid PoisonType::Type value");
	}
}

const std::optional<PoisonType::Type> PoisonType::fromString(std::string_view sv) {
	const std::string val = lcaseconcat(sv);
	if (val == "circulatory") return kCirculatory;
	if (val == "conversion") return kConversion;
	if (val == "muscle") return kMuscle;
	if (val == "nerve") return kNerve;
	if (val == "reduction") return kReduction;
	if (val == "respiratory") return kRespiratory;
	return {};
}