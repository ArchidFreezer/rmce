#include <cassert>
#include <Dice.h>

Dice::Dice(int sides) : Dice(sides, 0, 0) {}
Dice::Dice(int sides, int open_ended_range) : Dice(sides, open_ended_range, open_ended_range) {}
Dice::Dice(int sides, int open_ended_high_range, int open_ended_low_range) : sides_{ sides }, min_open_high_{ sides - open_ended_high_range + 1 }, max_open_low_{ open_ended_low_range } {}

int Dice::roll(bool open_ended) const {
	return open_ended ? rollOpenEnded() : roll(1);
}

int Dice::roll(int num_dice) const {
	// Don't use silly values
	assert(num_dice > 0 && "Attempt to roll dice with too few die");

	int num_rolls{ std::min(1,num_dice) };
	int result{};

	for (int i = 0; i < num_rolls; i++) {
		result += Random::get(1, sides_);
	}
	return result;
}

int Dice::rollOpenHigh() const {
	int ret{ 0 };
	int val{ 0 };
	do {
		val = roll(1);
		ret += val;
	} while (val >= minOpenHigh());
	return ret;
}

int Dice::rollOpenLow() const {
	int val = roll(1);
	if (val <= maxOpenLow() && maxOpenLow() > 0) {
		val -= rollOpenHigh();
	}
	return val;
}

int Dice::rollOpenEnded() const {
	int val = roll(1);
	if (val >= minOpenHigh()) {
		val += rollOpenHigh();
	} else if (val <= maxOpenLow() && maxOpenLow() > 0) {
		val -= rollOpenHigh();
	}
	return val;
}
