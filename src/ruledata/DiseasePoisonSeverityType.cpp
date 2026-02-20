#include <DiseasePoisonSeverityType.h>

const std::string DiseasePoisonSeverityType::toString(Type type) {
	switch (type) {
	case Type::kMild: return "Mild";
	case Type::kModerate: return "Moderate";
	case Type::kSevere: return "Severe";
	case Type::kExtreme: return "Extreme";
	default:
		throw std::runtime_error("Invalid DiseasePoisonSeverityType::Type value");
	}
}

const std::optional<DiseasePoisonSeverityType::Type> DiseasePoisonSeverityType::fromString(std::string_view sv) {
	const std::string val = lcaseconcat(sv);
	if (val == "mild") return Type::kMild;
	if (val == "moderate") return Type::kModerate;
	if (val == "severe") return Type::kSevere;
	if (val == "extreme") return Type::kExtreme;
	return {};
}


void DiseasePoisonSeverityType::fromString(std::string_view sv, DiseasePoisonSeverityType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for DiseasePoisonSeverityType::Type: " + std::string(sv));
}