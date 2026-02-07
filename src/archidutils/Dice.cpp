#include <cassert>
#include <Dice.h>

Dice::Dice(int sides) : Dice(sides, 0, 0) {}
Dice::Dice(int sides, int open_ended_range) : Dice(sides, open_ended_range, open_ended_range) {}
Dice::Dice(int sides, int open_ended_high_range, int open_ended_low_range) : sides_{sides}, min_open_high_{ sides - open_ended_high_range + 1 }, max_open_low_{ open_ended_low_range } {
	if (sides_ < 0) throw std::invalid_argument("Number of sides on the dice must not be negative.");

	if (sides_ > 0) {
		if (min_open_high_ <= 1)  throw std::invalid_argument("Open ended high minimum must be greater than 1.");
		if (max_open_low_ >= sides_)  throw std::invalid_argument("Open ended low maximum must be less than the number of sides.");
	}
}

Dice& Dice::roll(bool open_ended) {
	open_ended ? rollOpenEnded() : roll(1);
	return* this;
}

Dice& Dice::roll(int num_dice) {
	if (num_dice < 1)  throw std::invalid_argument("Must roll the dice at least once.");

	if (sides_ == 0) {
		result_ = std::make_pair(0, 0);
	} else {
		int num_rolls{ std::max(1,num_dice) };
		int result{};

		for (int i = 0; i < num_rolls; i++) {
			result += Random::get(1, sides_);
		}
		result_ = std::make_pair(result, result);
	}
	return *this;
}

Dice& Dice::rollOpenHigh() {
	if (sides_ == 0) {
		result_ = std::make_pair(0, 0);
	} else {
		int unmodified = roll(1).result_.second;
		int total = unmodified;
		int val = unmodified;
		while (val >= minOpenHigh()) {
			val = roll(1).result_.first;
			total += val;
		}
		result_ = std::make_pair(total, unmodified);
	}
	return *this;
}

Dice& Dice::rollOpenLow() {
	if (sides_ == 0) {
		result_ = std::make_pair(0, 0);
	} else {
		int unmodified = roll(1).result_.second;
		int val = unmodified;
		if (val <= maxOpenLow() && maxOpenLow() > 0) {
			val -= rollOpenHigh().result_.first;
		}
		result_ = std::make_pair(val, unmodified);
	}
	return *this;
}

Dice& Dice::rollOpenEnded() {
	if (sides_ == 0) {
		result_ = std::make_pair(0, 0);
	} else {
		int unmodified = roll(1).result_.second;
		int val = unmodified;
		if (val >= minOpenHigh()) {
			val += rollOpenHigh().result_.first;
		} else if (val <= maxOpenLow() && maxOpenLow() > 0) {
			val -= rollOpenHigh().result_.first;
		}
		result_ = std::make_pair(val, unmodified);
	}
	return *this;
}
