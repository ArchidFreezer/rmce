#include <AnimalData.h>

void AnimalData::fromString(std::string_view sv, BonusXpCode& type) {
	if (sv.empty() || sv == "None" || sv == "-") { type = BonusXpCode::kNone; }
	else if (sv.size() == 1 && sv[0] == 'A') { type = BonusXpCode::kA; }
	else if (sv.size() == 1 && sv[0] == 'B') { type = BonusXpCode::kB; }
	else if (sv.size() == 1 && sv[0] == 'C') { type = BonusXpCode::kC; }
	else if (sv.size() == 1 && sv[0] == 'D') { type = BonusXpCode::kD; }
	else if (sv.size() == 1 && sv[0] == 'E') { type = BonusXpCode::kE; }
	else if (sv.size() == 1 && sv[0] == 'F') { type = BonusXpCode::kF; }
	else if (sv.size() == 1 && sv[0] == 'G') { type = BonusXpCode::kG; }
	else if (sv.size() == 1 && sv[0] == 'H') { type = BonusXpCode::kH; }
	else if (sv.size() == 1 && sv[0] == 'I') { type = BonusXpCode::kI; }
	else if (sv.size() == 1 && sv[0] == 'J') { type = BonusXpCode::kJ; }
	else if (sv.size() == 1 && sv[0] == 'K') { type = BonusXpCode::kK; }
	else if (sv.size() == 1 && sv[0] == 'L') { type = BonusXpCode::kL; }
	else throw std::invalid_argument("Invalid string value for CriticalTableType::Type: " + std::string(sv));
}

std::string AnimalData::bonusXpCodeToString() const {
	switch(bonus_xp_code_) {
	case BonusXpCode::kNone: return "None";
	case BonusXpCode::kA: return "A";
	case BonusXpCode::kB: return "B";
	case BonusXpCode::kC: return "C";
	case BonusXpCode::kD: return "D";
	case BonusXpCode::kE: return "E";
	case BonusXpCode::kF: return "F";
	case BonusXpCode::kG: return "G";
	case BonusXpCode::kH: return "H";
	case BonusXpCode::kI: return "I";
	case BonusXpCode::kJ: return "J";
	case BonusXpCode::kK: return "K";
	case BonusXpCode::kL: return "L";
	default: throw std::invalid_argument("Invalid BonusXpCode enumeration value");
	}

}

void AnimalData::setFrequencyFactor(int frequency_factor) {
	if (frequency_factor < 1) frequency_factor_ = 1;
	else if (frequency_factor > 9) frequency_factor_ = 9;
	else frequency_factor_ = frequency_factor;
}

int AnimalData::huntingModifier() const {
	if (frequencyFactor() == 1) return 30;
	else if (frequencyFactor() == 2) return 20;
	else if (frequencyFactor() == 3) return 10;
	else if (frequencyFactor() == 4) return 0;
	else if (frequencyFactor() == 5) return -10;
	else if (frequencyFactor() == 6) return -20;
	else if (frequencyFactor() == 7) return -30;
	else if (frequencyFactor() == 8) return -50;
	else if (frequencyFactor() == 9) return -70;
	else return -200;
}

int AnimalData::existencePercentageChance() const {
	if (frequencyFactor() == 1) return 100;
	else if (frequencyFactor() == 2) return 90;
	else if (frequencyFactor() == 3) return 75;
	else if (frequencyFactor() == 4) return 50;
	else if (frequencyFactor() == 5) return 40;
	else if (frequencyFactor() == 6) return 30;
	else if (frequencyFactor() == 7) return 20;
	else if (frequencyFactor() == 8) return 9;
	else if (frequencyFactor() == 9) return 4;
	else return 0;
}
