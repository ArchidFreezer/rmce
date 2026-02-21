#include <DiseaseType.h>

const std::string DiseaseType::toString(Type type) {
	switch (type) {
	case kBubonic: return "Bubonic";
	case kChemical: return "Chemical";
	case kGenetic: return "Genetic";
	case kPneumonic: return "Pneumonic";
	case kPsychiatric: return "Psychiatric";
	default:
		throw std::invalid_argument("Invalid DiseaseType::Type value");
	}
}

const std::optional<DiseaseType::Type> DiseaseType::fromString(std::string_view sv) {
	const std::string val = lcaseconcat(sv);
	if (val == "bubonic") return kBubonic;
	if (val == "chemical") return kChemical;
	if (val == "genetic") return kGenetic;
	if (val == "pneumonic") return kPneumonic;
	if (val == "psychiatric") return kPsychiatric;
	return {};
}

void DiseaseType::fromString(std::string_view sv, DiseaseType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for DiseaseType::Type: " + std::string(sv));
}