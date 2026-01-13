#pragma once

#include "Random.h"

/**
 * @class Dice
 * @brief Represents a single die object
 * 
 * The dice may be rolled as a standard die with an equal spread of results from 1 to the number of sides.
 * 
 * Additionally the dice may be rolled as open ended which is a scenario where if the rolled number is close to the edge
 * values it is rolled again and the subsequent result add/subtracted. A typical example is for D100 rolls where the top
 * and bottom 5 results (1-5 and 96-100) casue the dice to be rolled again. In the case of an open-ended low roll the
 * resulting number is subtracted rather than added. When an open-ended roll is triggered, either high or low, rolls
 * continue to be made and summed until one results in a value less than the open-ended high value.
 * 
 * Some examples follow:
 * - Roll 1: 03 (open-ended low range triggers extra roll)
 * - Roll 2: 47 (not in open-ended high range so we stop here)
 * - Result: 03 - 47 = -44
 * .
 * - Roll 1: 03 (open-ended low range triggers extra roll)
 * - Roll 2: 97 (open-ended-high range so roll again)
 * - Roll 3: 68 (not in open-ended high range so we stop here)
 * - Result: 03 - 97 - 68 = -162
 * .
 * - Roll 1: 98 (open-ended high range triggers extra roll)
 * - Roll 2: 04 (not in open-ended high range so we stop here)
 * - Result: 98 + 04 = 102
 *
 */
class Dice {
public:
	/**
	 * @brief Constructor
	 * @param sides Number of sides on the dice
	 */
	Dice(int sides);
	/**
	 * @brief Constructor
	 * @param sides Number of sides on the dice
	 * @param open_ended_range The range of numbers at the top and bottom end that will trigger an open-ended roll
	 *                         Use a value of 0 to disable open-ended rolls
	 */
	Dice(int sides, int open_ended_range);
	/**
	 * @brief Constructor
	 * @param sides Number of sides on the dice
	 * @param open_ended_high_range The range of numbers at the top end that will trigger an open-ended high roll; 0 to disable
	 * @param open_ended_low_range The range of numbers at the top end that will trigger an open-ended low roll; 0 to disable
	 */
	Dice(int sides, int open_ended_high_range, int open_ended_low_range);

	/**
	 * @brief Gets the number at which an open-ended high roll is triggered.
	 * If the dice roll is this value or higher then the dice are re-rolled and the values summed. This continues until a roll
	 * which is below this is rolled.
	 *
	 * @return the minimum value to trigger another roll
	 */
	inline int minOpenHigh() const { return min_open_high_; }

	/**
	 * @brief Sets the number at which an open-ended high roll is triggered.
	 * If the dice roll is this value or higher then the dice are re-rolled and the values summed. This continues until a roll
	 * which is below this is rolled.
	 * 
	 * If `min_open_high` < 0 then the minimum will be set to 0
	 *
	 * @param min_open_high The minimum value to trigger another roll
	 */
	inline void setMinOpenHigh(int min_open_high) { min_open_high_ = std::min(0,min_open_high); }

	/**
	 * @brief Gets the number at which an open-ended low roll is triggered.
	 * If the dice roll is this value or lower then the dice are re-rolled open-ended high and the new roll subtracted from the
	 * existing value.
	 *
	 * @return the maximum value to trigger another roll
	 */
	inline int maxOpenLow() const { return max_open_low_; }

	/**
	 * @brief Sets the number at which an open-ended low roll is triggered.
	 * If the dice roll is this value or lower then the dice are re-rolled open-ended high and the new roll subtracted from
	 * the existing value.
	 *
	 * @param max_open_low The maximum value to trigger another roll
	 */
	inline void setMaxOpenLow(int max_open_low) { max_open_low_ = max_open_low; }

	/**
	 * @brief Returns the number of sides currently configured on the dice.
	 *
	 * When rolling the dice a number will be returned between 1 and this number.
	 *
	 * @return int number of sides
	 */
	inline int sides() const { return sides_; }

	/**
	 * @brief Sets the number of sides of the dice to use.
	 * 
	 * When rolling the dice a number will be returned between 1 and `sides`.
	 * If `sides` is < 1 then the number of sides will be set to 1.
	 *
	 * @param sides Number of sides
	 */
	inline void setSides(int sides) { sides_ = std::max(1, sides); }

	/**
	 * @brief Roll them bones
	 * 
	 * Defaults to open ended rolls, use the parameter to prevent this.
	 * 
	 * @param open_ended `true` to roll open-ended; `false` otherwse
	 * @return The result of the dice roll
	 */
	int roll(bool open_ended = true) const;

	/**
	 * @brief Roll multiple dice
	 * 
	 * Open-ended rolls are never made using this method.
	 * 
	 * @param num_dice Number of dice to roll
	 * @return The result of the dice roll
	 */
	int roll(int num_dice) const;

	/**
	 * @brief Returns an open ended high roll.
	 * 
	 * If the initial result of the roll is greater than getMinOpenHigh() then the dice is re-rolled and the value of this
	 * roll added. This continues until a roll of less than getMinOpenHigh() is made.
	 *
	 * @return int result of roll
	 * @see rollOpenLow()
	 * @see rollOpenEnded()
	 */
	int rollOpenHigh() const;

	/**
	 * @brief Returns an open ended low roll.
	 * 
	 * If the initial result of the roll is between 1 and getMaxOpenLow() then the dice is re-rolled open ended high and the
	 * value of this roll subtracted.
	 *
	 * @return int result of roll
	 * @see rollOpenHigh()
	 * @see rollOpenEnded()
	 */
	int rollOpenLow() const;

	/**
	 * @brief Returns an open ended roll.
	 *
	 * If the initial roll is greater than getMinOpenHigh() then an additional open ended high roll is made and the value
	 * added. If the initial roll is between 1 and getMaxOpenLow() then an additional open ended high roll is made and the
	 * value subtracted.
	 *
	 * @return int result of roll
	 * @see rollOpenHigh()
	 * @see rollOpenLow()
	 */
	int rollOpenEnded() const;

private:
	int sides_{}; /**< The number of sides */
	int min_open_high_{}; /**< The minimum number that triggers an open ended high roll */
	int max_open_low_{}; /**< The maximum number that triggers an open ended low roll */
};