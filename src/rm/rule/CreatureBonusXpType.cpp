#include <CreatureBonusXpType.h>

const std::string CreatureBonusXpType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kNone: return "None";
	case kA: return "A";
	case kB: return "B";
	case kC: return "C";
	case kD: return "D";
	case kE: return "E";
	case kF: return "F";
	case kG: return "G";
	case kH: return "H";
	case kI: return "I";
	case kJ: return "J";
	case kK: return "K";
	case kL: return "L";
	default: throw std::invalid_argument("Invalid CreatureBonusXpType::Type enumeration value");
	}
}

const std::optional<CreatureBonusXpType::Type> CreatureBonusXpType::fromString(std::string_view sv) {
	using enum Type;

	if (sv.empty() || sv == "None" || sv == "-") { return kNone; } 
	else if (sv.size() == 1 && sv[0] == 'A') { return kA; } 
	else if (sv.size() == 1 && sv[0] == 'B') { return kB; }
	else if (sv.size() == 1 && sv[0] == 'C') { return kC; } 
	else if (sv.size() == 1 && sv[0] == 'D') { return kD; } 
	else if (sv.size() == 1 && sv[0] == 'E') { return kE; }
	else if (sv.size() == 1 && sv[0] == 'F') { return kF; }
	else if (sv.size() == 1 && sv[0] == 'G') { return kG; }
	else if (sv.size() == 1 && sv[0] == 'H') { return kH; }
	else if (sv.size() == 1 && sv[0] == 'I') { return kI; }
	else if (sv.size() == 1 && sv[0] == 'J') { return kJ; }
	else if (sv.size() == 1 && sv[0] == 'K') { return kK; }
	else if (sv.size() == 1 && sv[0] == 'L') { return kL; }
	else throw std::invalid_argument("Invalid string value for CreatureBonusXpType::Type: " + std::string(sv));

	return {};
}

void CreatureBonusXpType::fromString(std::string_view sv, CreatureBonusXpType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for CreatureBonusXpType::Type: " + std::string(sv));
}