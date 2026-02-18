#include <AnimalData.h>
#include <Dice.h>
#include <NumberMatcherFactory.h>

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
	switch(constitutionVarianceType()) {
		case CreatureConstitutionVarianceType::Type::kNone: return 0;
		case CreatureConstitutionVarianceType::Type::kA: return 1;
		case CreatureConstitutionVarianceType::Type::kB: return 2;
		case CreatureConstitutionVarianceType::Type::kC: return 3;
		case CreatureConstitutionVarianceType::Type::kD: return 5;
		case CreatureConstitutionVarianceType::Type::kE: return 8;
		case CreatureConstitutionVarianceType::Type::kF: return 10;
		case CreatureConstitutionVarianceType::Type::kG: return 12;
		case CreatureConstitutionVarianceType::Type::kH: return 15;
		default: return 0;
	}
}

int AnimalData::bonusExhaustionPoints() const {
	switch(constitutionVarianceType()) {
	case CreatureConstitutionVarianceType::Type::kNone:
	case CreatureConstitutionVarianceType::Type::kA:
	case CreatureConstitutionVarianceType::Type::kB:
	case CreatureConstitutionVarianceType::Type::kC:
	case CreatureConstitutionVarianceType::Type::kD: return 0;
	case CreatureConstitutionVarianceType::Type::kE: return 50;
	case CreatureConstitutionVarianceType::Type::kF: return 100;
	case CreatureConstitutionVarianceType::Type::kG: return 150;
	case CreatureConstitutionVarianceType::Type::kH: return 200;
	default: return 0;
	}
}

int AnimalData::staminaBonus() const {

	NumberMatcherFactory number_matcher{};

	int d100_roll = Dice(100).roll(false).result();
	switch(constitutionVarianceType()) {
	case CreatureConstitutionVarianceType::Type::kNone: return 0;
	case CreatureConstitutionVarianceType::Type::kA: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(2, 5)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(6, 9)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(10, 17)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(18, 25)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(26, 74)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(75, 83)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(84, 91)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(92, 95)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(96, 99)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 5;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kB: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(2, 2)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(3, 4)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(5, 7)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(8, 11)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(12, 21)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(22, 31)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(32, 69)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(70, 79)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(80, 89)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(90, 93)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(94, 96)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(97, 98)->matches(d100_roll)) return 5;
		else if (number_matcher.matcher(99, 99)->matches(d100_roll)) return 6;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 7;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kC: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -8;
		else if (number_matcher.matcher(2, 3)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(4, 5)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(6, 8)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(9, 15)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(16, 23)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(24, 47)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(48, 74)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(75, 85)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(86, 89)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(90, 94)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(95, 97)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(98, 99)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 5;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kD: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -8;
		else if (number_matcher.matcher(2, 2)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(3, 3)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(4, 4)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(5, 6)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(7, 9)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(10, 16)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(17, 24)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(25, 74)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(75, 82)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(83, 89)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(90, 92)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(93, 94)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(95, 96)->matches(d100_roll)) return 5;
		else if (number_matcher.matcher(97, 98)->matches(d100_roll)) return 6;
		else if (number_matcher.matcher(99, 99)->matches(d100_roll)) return 7;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 8;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kE: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -8;
		else if (number_matcher.matcher(2, 2)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(3, 3)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(4, 4)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(5, 6)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(7, 9)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(10, 16)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(17, 24)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(25, 72)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(73, 80)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(81, 87)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(88, 90)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(91, 92)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(93, 93)->matches(d100_roll)) return 5;
		else if (number_matcher.matcher(94, 94)->matches(d100_roll)) return 6;
		else if (number_matcher.matcher(95, 95)->matches(d100_roll)) return 7;
		else if (number_matcher.matcher(96, 96)->matches(d100_roll)) return 8;
		else if (number_matcher.matcher(97, 97)->matches(d100_roll)) return 9;
		else if (number_matcher.matcher(98, 98)->matches(d100_roll)) return 10;
		else if (number_matcher.matcher(99, 99)->matches(d100_roll)) return 11;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 12;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kF: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -8;
		else if (number_matcher.matcher(2, 2)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(3, 3)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(4, 4)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(5, 5)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(6, 6)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(7, 8)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(9, 10)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(11, 25)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(26, 48)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(49, 72)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(73, 80)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(81, 87)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(88, 90)->matches(d100_roll)) return 5;
		else if (number_matcher.matcher(91, 92)->matches(d100_roll)) return 6;
		else if (number_matcher.matcher(93, 94)->matches(d100_roll)) return 7;
		else if (number_matcher.matcher(95, 95)->matches(d100_roll)) return 8;
		else if (number_matcher.matcher(96, 96)->matches(d100_roll)) return 9;
		else if (number_matcher.matcher(97, 97)->matches(d100_roll)) return 10;
		else if (number_matcher.matcher(98, 98)->matches(d100_roll)) return 11;
		else if (number_matcher.matcher(99, 99)->matches(d100_roll)) return 12;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 15;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kG: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -8;
		else if (number_matcher.matcher(2, 2)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(3, 3)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(4, 4)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(5, 5)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(6, 6)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(7, 7)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(8, 8)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(9, 11)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(12, 19)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(20, 26)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(27, 49)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(50, 71)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(72, 79)->matches(d100_roll)) return 5;
		else if (number_matcher.matcher(80, 86)->matches(d100_roll)) return 6;
		else if (number_matcher.matcher(87, 91)->matches(d100_roll)) return 7;
		else if (number_matcher.matcher(92, 93)->matches(d100_roll)) return 8;
		else if (number_matcher.matcher(94, 95)->matches(d100_roll)) return 9;
		else if (number_matcher.matcher(96, 96)->matches(d100_roll)) return 10;
		else if (number_matcher.matcher(97, 97)->matches(d100_roll)) return 11;
		else if (number_matcher.matcher(98, 98)->matches(d100_roll)) return 12;
		else if (number_matcher.matcher(99, 99)->matches(d100_roll)) return 15;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 20;
		else return 0; // Should never be hit
	}
	case CreatureConstitutionVarianceType::Type::kH: {
		if (number_matcher.matcher(0, 1)->matches(d100_roll)) return -8;
		else if (number_matcher.matcher(2, 2)->matches(d100_roll)) return -7;
		else if (number_matcher.matcher(3, 3)->matches(d100_roll)) return -6;
		else if (number_matcher.matcher(4, 4)->matches(d100_roll)) return -5;
		else if (number_matcher.matcher(5, 5)->matches(d100_roll)) return -4;
		else if (number_matcher.matcher(6, 6)->matches(d100_roll)) return -3;
		else if (number_matcher.matcher(7, 7)->matches(d100_roll)) return -2;
		else if (number_matcher.matcher(8, 8)->matches(d100_roll)) return -1;
		else if (number_matcher.matcher(9, 9)->matches(d100_roll)) return 0;
		else if (number_matcher.matcher(10, 10)->matches(d100_roll)) return 1;
		else if (number_matcher.matcher(11, 12)->matches(d100_roll)) return 2;
		else if (number_matcher.matcher(13, 20)->matches(d100_roll)) return 3;
		else if (number_matcher.matcher(21, 27)->matches(d100_roll)) return 4;
		else if (number_matcher.matcher(38, 50)->matches(d100_roll)) return 5;
		else if (number_matcher.matcher(51, 72)->matches(d100_roll)) return 6;
		else if (number_matcher.matcher(73, 88)->matches(d100_roll)) return 7;
		else if (number_matcher.matcher(89, 93)->matches(d100_roll)) return 8;
		else if (number_matcher.matcher(94, 95)->matches(d100_roll)) return 9;
		else if (number_matcher.matcher(96, 96)->matches(d100_roll)) return 10;
		else if (number_matcher.matcher(97, 97)->matches(d100_roll)) return 11;
		else if (number_matcher.matcher(98, 98)->matches(d100_roll)) return 12;
		else if (number_matcher.matcher(99, 99)->matches(d100_roll)) return 15;
		else if (number_matcher.matcher(100, 100)->matches(d100_roll)) return 20;
		else return 0; // Should never be hit
	}
	default: return 0;
	}
}

int AnimalData::hits(int level) const {
	return baseHits() + staminaBonus() + (hitsPerLevelDifference() * (level-averageLevel()) );
}


int AnimalData::exhaustionPoints() const {
	return bonusExhaustionPoints() + (staminaBonus() * 3) + 40;
}