#pragma once

#include "Random.h"

/**
 * @class Dice
 * @brief Represents a single die object
 * 
 * The dice may be rolled as a standard die with an equal spread of results from 1 to the number of sides or open ended
 * which is a scenario where if the rolled number is close to the edge values it is rolled again and the subsequent result
 * added/subtracted.
 * 
 * The methods to roll the dice return a reference to the dice object itself rather than the result as there are cases where
 * we want both the result of an open ended roll, plus the initial unmodified results. Both these values may be retrieved
 * following a roll using the result() and unmodified() methods.
 * 
 * @code
 * // Roll a single 6 sided dice (D6) and get the result
 * Dice(6).roll().result();
 * 
 * // Roll 2D6
 * Dice(6).roll(2).result();
 * 
 * // Roll an open ended D100 with the top and bottom 5 numbers triggering an open ended roll and retrieving both
 * // unmodifed roll and the total with any open ended rolls added.
 * Dice(100,5) d100{};
 * int total = d100.roll().result();
 * int unmod = d100.unmodified();
 * 
 * // Roll an open ended high D100 with the top 5 numbers triggering an open ended roll and retrieving both unmodifed roll
 * // and the total with any open ended rolls added
 * Dice(100,5) d100{};
 * int total = d100.rollOpenHigh().result();
 * int unmod = d100.unmodified();
 * 
 * // An alternative for open ended high only would be to use a different constructor which prevents any open low rolls.
 * // This would, however, prevent this d100 object from being used for open ended low rolls later
 * Dice(100,5,0) d100{};
 * int total = d100.roll().result();
 * int unmod = d100.unmodified();
 *
 * @endcode
 * 
 * A typical example is for D100 rolls where the top and bottom 5 results (1-5 and 96-100) cause the dice to be rolled
 * again. In the case of an open ended low roll the resulting number is subtracted rather than added. When an open ended
 * roll is triggered, either high or low, rolls continue to be made and summed until one results in a value less than the
 * open ended high value.
 * 
 * Some examples of open ended rolls follow:
 * - Roll 1: 03 (open ended low range triggers extra roll)
 * - Roll 2: 47 (not in open ended high range so we stop here)
 * - Result: 03 - 47 = -44
 * .
 * - Roll 1: 03 (open ended low range triggers extra roll)
 * - Roll 2: 97 (open ended high range so roll again)
 * - Roll 3: 68 (not in open ended high range so we stop here)
 * - Result: 03 - 97 - 68 = -162
 * .
 * - Roll 1: 98 (open ended high range triggers extra roll)
 * - Roll 2: 04 (not in open ended high range so we stop here)
 * - Result: 98 + 04 = 102
 *
 */
class Dice {
public:
	/**
	 * @brief Constructor that does not allow open ended rolls
	 * 
	 * @warning
	 * If @a sides < 1 it will be silently adjusted to 1 to prevent an error condition
	 * 
	 * @param sides Number of sides on the dice
	 */
	Dice(int sides);
	/**
	 * @brief Constructor that allows open ended rolls
	 * 
	 * @warning
	 * + If @a sides < 1 it will be silently adjusted to 1 to prevent an error condition
	 * + If @a open_ended_range is >= @a sides it will be silently adjusted to be (@a sides - 1) to prevent an infinite loop
	 *
	 * @param sides Number of sides on the dice
	 * @param open_ended_range The range of numbers at the top and bottom end that will trigger an open ended roll<br>
	 *                         Use a value of 0 to disable open ended rolls
	 */
	Dice(int sides, int open_ended_range);
	/**
	 * @brief Constructor that allows control of which open ended rolls may be made
	 * 
	 * @warning
	 * + If @a sides < 1 it will be silently adjusted to 1 to prevent an error condition
	 * + If @a open_ended_high_range or @a open_ended_high_range is >= @a sides they will be silently adjusted to be
	 * (@a sides - 1) to prevent an infinite loop
	 * 
	 * @param sides Number of sides on the dice
	 * @param open_ended_high_range The range of numbers at the top end that will trigger an open ended high roll; 0 to disable
	 * @param open_ended_low_range The range of numbers at the bottom end that will trigger an open ended low roll; 0 to disable
	 */
	Dice(int sides, int open_ended_high_range, int open_ended_low_range);

	/**
	 * @brief Gets the number at which an open ended high roll is triggered.
	 * If the dice roll is this value or higher then the dice are re-rolled and the values summed. This continues until a roll
	 * which is below this is rolled.
	 *
	 * @return the minimum value to trigger another roll
	 */
	int minOpenHigh() const { return min_open_high_; }

	/**
	 * @brief Sets the number at which an open ended high roll is triggered.
	 * If the dice roll is this value or higher then the dice are re-rolled and the values summed. This continues until a roll
	 * which is below this is rolled.
	 * 
	 * If `min_open_high` < 2 then the minimum will be set to 2 to prevent an infinite loop
	 *
	 * @param min_open_high The minimum value to trigger another roll
	 */
	void setMinOpenHigh(int min_open_high) { min_open_high_ = std::max(2,min_open_high); }

	/**
	 * @brief Gets the number at which an open ended low roll is triggered.
	 * If the dice roll is this value or lower then the dice are re-rolled open ended high and the new roll subtracted from the
	 * existing value.
	 *
	 * @return the maximum value to trigger another roll
	 */
	int maxOpenLow() const { return max_open_low_; }

	/**
	 * @brief Sets the number at which an open ended low roll is triggered.
	 * If the dice roll is this value or lower then the dice are re-rolled open ended high and the new roll subtracted from
	 * the existing value.
	 *
	 * If `max_open_low` >= the number of sides then the minimum will be set to (sides - 1) to prevent an infinite loop
	 *
	 * @param max_open_low The maximum value to trigger another roll
	 */
	void setMaxOpenLow(int max_open_low) { max_open_low_ = std::min(max_open_low, sides_ - 1); }

	/**
	 * @brief Returns the number of sides currently configured on the dice.
	 *
	 * When rolling the dice a number will be returned between 1 and this number.
	 *
	 * @return int number of sides
	 */
	int sides() const { return sides_; }

	/**
	 * @brief Sets the number of sides of the dice to use.
	 * 
	 * When rolling the dice a number will be returned between 1 and `sides`.
	 * If `sides` is < 1 then the number of sides will be set to 1.
	 *
	 * @param sides Number of sides
	 */
	void setSides(int sides) { sides_ = std::max(1, sides); }

	/**
	 * @brief Gets the total value of the last dice roll
	 * @return int Dice roll value, including any open ended rolls
	 */
	int result() const { return result_.first; }

	/**
	 * @brief Gets the unmodified first roll value of the last dice roll
	 * 
	 * This function gets the initial dice roll value before any open ended rolls are applied
	 * @return int Dice roll value, excluding any open ended rolls
	 */
	int unmodified() const { return result_.second; }

	/**
	 * @brief Roll them bones
	 * 
	 * Defaults to open ended rolls, use the parameter to prevent this.
	 * 
	 * @param open_ended `true` to roll open ended; `false` otherwse
	 * @return Reference to this dice object
	 */
	Dice& roll(bool open_ended = true);

	/**
	 * @brief Roll multiple dice
	 * 
	 * Open-ended rolls are never made using this method.
	 * 
	 * @param num_dice Number of dice to roll
	 * @return Reference to this dice object
	 */
	Dice& roll(int num_dice);

	/**
	 * @brief Returns an open ended high roll.
	 * 
	 * If the initial result of the roll is greater than getMinOpenHigh() then the dice is re-rolled and the value of this
	 * roll added. This continues until a roll of less than getMinOpenHigh() is made.
	 *
	 * @return Reference to this dice object
	 * @see rollOpenLow()
	 * @see rollOpenEnded()
	 */
	Dice& rollOpenHigh();

	/**
	 * @brief Returns an open ended low roll.
	 * 
	 * If the initial result of the roll is between 1 and getMaxOpenLow() then the dice is re-rolled open ended high and the
	 * value of this roll subtracted.
	 *
	 * @return Reference to this dice object
	 * @see rollOpenHigh()
	 * @see rollOpenEnded()
	 */
	Dice& rollOpenLow();

	/**
	 * @brief Returns an open ended roll.
	 *
	 * If the initial roll is greater than getMinOpenHigh() then an additional open ended high roll is made and the value
	 * added. If the initial roll is between 1 and getMaxOpenLow() then an additional open ended high roll is made and the
	 * value subtracted.
	 *
	 * @return Reference to this dice object
	 * @see rollOpenHigh()
	 * @see rollOpenLow()
	 */
	Dice& rollOpenEnded();

private:
	int sides_{}; /**< The number of sides */
	int min_open_high_{}; /**< The minimum number that triggers an open ended high roll */
	int max_open_low_{}; /**< The maximum number that triggers an open ended low roll */
	std::pair<int, int> result_{}; /**< The result of the roll
	                                   result_.first contains the total value
																		 result_.second contains the first unmodified roll */
};