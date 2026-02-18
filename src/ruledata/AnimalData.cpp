#include <AnimalData.h>

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

int AnimalData::hitsPerLevelDifference() const {
	switch(bonusConstitutionCode()) {
		case CreatureBonusConstitutionType::Type::kNone: return 0;
		case CreatureBonusConstitutionType::Type::kA: return 1;
		case CreatureBonusConstitutionType::Type::kB: return 2;
		case CreatureBonusConstitutionType::Type::kC: return 3;
		case CreatureBonusConstitutionType::Type::kD: return 5;
		case CreatureBonusConstitutionType::Type::kE: return 8;
		case CreatureBonusConstitutionType::Type::kF: return 10;
		case CreatureBonusConstitutionType::Type::kG: return 12;
		case CreatureBonusConstitutionType::Type::kH: return 15;
		default: return 0;
	}
}

int AnimalData::bonusExhaustionPoints() const {
	switch(bonusConstitutionCode()) {
	case CreatureBonusConstitutionType::Type::kNone:
	case CreatureBonusConstitutionType::Type::kA:
	case CreatureBonusConstitutionType::Type::kB:
	case CreatureBonusConstitutionType::Type::kC:
	case CreatureBonusConstitutionType::Type::kD: return 0;
	case CreatureBonusConstitutionType::Type::kE: return 50;
	case CreatureBonusConstitutionType::Type::kF: return 100;
	case CreatureBonusConstitutionType::Type::kG: return 150;
	case CreatureBonusConstitutionType::Type::kH: return 200;
	default: return 0;
	}
}