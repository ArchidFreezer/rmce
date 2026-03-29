#include "CharacterStat.h"

namespace rm::game::character {

void Stat::setTemporary(int value) {
	temporary_ = std::max(0, value);
	potential_ = stat::getInitialPotentialValue(temporary_);
	basic_bonus_ = stat::getBasicBonus(temporary_);
}

void Stat::updateTemporary(int value) {
	temporary_ = std::max(0, temporary_ + value);
	basic_bonus_ = stat::getBasicBonus(temporary_);
}

void Stat::performStatGainRoll() {
	int gain = stat::getStatGain(potential_ - temporary_);

	if (gain < 0) {
		// If the gain is negative then we need to ensure that we don't reduce the temporary stat value below 1.
		gain = std::max(gain, 1 - temporary_);
	}
	updateTemporary(gain);
}

namespace stat {

int getBasicBonus(int temp_value) {
	if (temp_value <= 10)
		return (temp_value - 21) / 2;
	if (temp_value <= 30)
		return (temp_value - 35) / 5;
	if (temp_value <= 69)
		return 0;
	if (temp_value <= 89)
		return (temp_value - 65) / 5;
	if (temp_value <= 100)
		return (temp_value - 80) / 2;
	return (temp_value - 95) * 2;
};

int getInitialPotentialValue(int temp_value) {
	if (temp_value <= 24)
		return 20 + archid::Dice(10).roll(8).result();
	if (temp_value <= 34)
		return 30 + archid::Dice(10).roll(7).result();
	if (temp_value <= 44)
		return 40 + archid::Dice(10).roll(6).result();
	if (temp_value <= 54)
		return 50 + archid::Dice(10).roll(5).result();
	if (temp_value <= 64)
		return 60 + archid::Dice(10).roll(4).result();
	if (temp_value <= 74)
		return std::max(temp_value, 70 + archid::Dice(10).roll(3).result());
	if (temp_value <= 84)
		return std::max(temp_value, 80 + archid::Dice(10).roll(2).result());
	if (temp_value <= 91)
		return 90 + archid::Dice(10).roll().result();
	if (temp_value == 92)
		return 91 + archid::Dice(9).roll().result();
	if (temp_value == 93)
		return 92 + archid::Dice(8).roll().result();
	if (temp_value == 94)
		return 93 + archid::Dice(7).roll().result();
	if (temp_value == 95)
		return 94 + archid::Dice(6).roll().result();
	if (temp_value == 96)
		return 95 + archid::Dice(5).roll().result();
	if (temp_value == 97)
		return 96 + archid::Dice(4).roll().result();
	if (temp_value == 98)
		return 97 + archid::Dice(3).roll().result();
	if (temp_value == 99)
		return 98 + archid::Dice(2).roll().result();
	return 99 + archid::Dice(2).roll().result();
};

int getStatGain(int difference) {
	// This uses two dice rolls and the difference between the stats determines how they are used.
	int roll1 = archid::Dice(10).roll().result();
	int roll2 = archid::Dice(10).roll().result();

	// Special case if the dice rolls are the same.
	if (roll1 == roll2) {
		if (roll1 <= 5)
			return -roll1;
		return roll1 * 2;
	}

	// If we get this far then the rolls are different and we need to determine which is the high and low roll.
	int high_roll = std::max(roll1, roll2);
	int low_roll = std::min(roll1, roll2);

	if (difference <= 10)
		return low_roll;
	if (difference <= 20)
		return high_roll;
	return high_roll + low_roll;
};

} // namespace stat

} // namespace rm::game::character