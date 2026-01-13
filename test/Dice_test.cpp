#include <gtest/gtest.h>

#include <Dice.h>

namespace {

	int get_max_roll(Dice& die, int num_rolls, bool open) {
		int result{ 0 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::max(result, die.roll(open));
		}
		return result;
	}

	int get_min_roll(Dice& die, int num_rolls, bool open) {
		int result{ die.sides() + 1 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::min(result, die.roll(open));
		}
		return result;
	}

	int get_max_roll_oe(Dice& die, int num_rolls) {
		int result{ 0 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::max(result, die.rollOpenEnded());
		}
		return result;
	}

	int get_min_roll_oe(Dice& die, int num_rolls) {
		int result{ die.sides() + 1 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::min(result, die.rollOpenEnded());
		}
		return result;
	}

	int get_max_roll_oh(Dice& die, int num_rolls) {
		int result{ 0 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::max(result, die.rollOpenHigh());
		}
		return result;
	}

	int get_min_roll_oh(Dice& die, int num_rolls) {
		int result{ die.sides() + 1 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::max(result, die.rollOpenHigh());
		}
		return result;
	}

	int get_max_roll_ol(Dice& die, int num_rolls) {
		int result{ 0 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::max(result, die.rollOpenLow());
		}
		return result;
	}

	int get_min_roll_ol(Dice& die, int num_rolls) {
		int result{ die.sides() + 1 };

		for (int i{ 0 }; i < num_rolls; i++) {
			result = std::min(result, die.rollOpenLow());
		}
		return result;
	}

	TEST(Dice, Constructor) {
		Dice d6(6);
		EXPECT_EQ(d6.sides(), 6);
		EXPECT_EQ(d6.maxOpenLow(), 0);
		EXPECT_EQ(d6.minOpenHigh(), 7);
	}

	TEST(Dice, StandardRoll) {
		Dice d6(6, 1);
		EXPECT_LE(get_max_roll(d6, 50, false), 6);
		EXPECT_GE(get_min_roll(d6, 50, false), 1);
	}

	TEST(Dice, StandardRollOpen) {
		Dice d6(6, 1);
		EXPECT_GT(get_max_roll(d6, 50, true), 6);
		EXPECT_LT(get_min_roll(d6, 50, true), 1);
	}

	TEST(Dice, OpenRoll) {
		Dice d6(6, 1);
		EXPECT_GT(get_max_roll_oe(d6, 50), 6);
		EXPECT_LT(get_min_roll_oe(d6, 50), 1);
	}

	TEST(Dice, OpenHigh) {
		Dice d6(6, 1);
		EXPECT_GT(get_max_roll_oh(d6, 50), 6);
		EXPECT_GE(get_min_roll_oh(d6, 50), 1);
	}

	TEST(Dice, OpenLow) {
		Dice d6(6, 1);
		EXPECT_LE(get_max_roll_ol(d6, 50), 6);
		EXPECT_LT(get_min_roll_ol(d6, 50), 1);
	}
}