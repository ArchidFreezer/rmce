#pragma once

#include <memory>
#include <vector>
#include <Dice.h>
#include <GameObject.h>

namespace rm::game::character {

namespace stat {
/**
 * @brief Calculates the basic bonus based on a temporary value.
 *
 * The bonus that a stat provides is based on the temporary value of the stat, which can be modified by various effects in the game. The basic bonus is calculated using a specific formula that takes into account different ranges of
 * temporary values to determine the appropriate bonus.
 *
 * @param temp_value The temporary value used to calculate the bonus.
 * @return The calculated basic bonus as an integer.
 */
int getBasicBonus(int temp_value);

/**
 * @brief Calculates the initial potential stat value based on a temporary value.
 *
 * The initial potential stat value is determined based on the temporary value of the stat. This value is used to set the potential stat at character creation and is rarely modified during the game.
 * @param temp_value The temporary value used to calculate the potential stat value.
 * @return The calculated initial potential stat value as an integer.
 */
int getInitialPotentialValue(int temp_value);

/**
 * @brief Calculates the stat gain based on the difference between the potential and temporary stat values.
 *
 * The stat gain is determined by rolling two dice and using the difference between the potential and temporary stat values to decide how the rolls are applied. Special rules apply if the dice rolls are the same.
 * Stat gain rolls are typically only performed during the levelling up process and/or the completion of a training package, and the resulting temporary stat can never be lowered below 1 through this process, even if the gain is negative.
 *
 * @param difference The difference between the potential and temporary stat values used to calculate the stat gain.
 * @return The calculated stat gain as an integer.
 */
int getStatGain(int difference);

} // namespace rm::game::character::stat

/**
 * @class Stat
 * @brief Represents a character stat in the game, such as strength or dexterity and contains the logic to determine any bonus the stat applies to rolls based on the temporary stat value.
 *
 * Each character will have 10 stats, each of which is represented by this class.
 *
 */
class Stat {
public:
	/**
	 * @brief Sets the racial bonus value.
	 *
	 * This is typically only set at character creation when the race is set.
	 *
	 * @param bonus The racial bonus value to set.
	 */
	void setRacialBonus(int bonus) {
		racial_bonus_ = bonus;
	}

	/**
	 * @brief Gets the potential stat value.
	 * 
	 * This value is calculated at character creation based on the temporary stat value and is very rarely modified during the game. It represents the maximum potential of the stat and is used to determine stat gains during levelling up and
	 * training.
	 * 
	 * @return The potential stat value.
	 */
	int potential() const {
		return potential_;
	}

	/**
	 * @brief Sets the potential stat value.
	 *
	 * This method is used to set the potential stat value at character creation time instead of using the value created automatically when the temporary stat is set. This is to allow customization of the initial potential value or
	 * importing of characters from other sources where the potential value is already determined.
	 * 
	 * @param value The potential stat value to set.
	 * 
	 * @see updatePotential
	 */
	void setPotential(int value) {
		potential_ = std::max(0,value);
	}

	/**
	 * @brief Updates the potential stat value.
	 *
	 * The stat potential should only be modified in extremely rare circumstances, such as the use of very powerful magic or a life changing event in the game.
	 *
	 * @param value The potential stat value to set.
	 */
	void updatePotential(int value) {
		potential_ = std::max(0, potential_ + value);
	}

	/**
	 * @brief Gets the temporary stat value.
	 * @return The temporary stat value.
	 */
	int temporary() const {
		return temporary_;
	}

	/**
	 * @brief Sets the temporary stat value and calculates the initial potential stat value based on it.
	 *
	 * This method is used to set the temporary stat value at character creation time.
	 * After setting the temporary value, it calls the `getInitialPotentialStatValue` function to calculate the initial potential stat value based on the new temporary stat value.
	 * 
	 * To change the value after character creation, the `updateTemporary` method should be used instead, which will also recalculate the basic bonus accordingly.
	 *
	 * @param value The temporary stat value to set.
	 * 
	 * @see updateTemporary
	 */
	void setTemporary(int value);

	/**
	 * @brief Change the temporary stat value and recalculate the basic bonus accordingly.
	 *
	 * This method is used to update the temporary stat value, which may be modified by various effects in the game. The `value` parameter is added to the current temporary stat value, and the result is ensured to be non-negative using
	 * `std::max(0, ...)`.
	 * After updating the temporary value, it calls the `calculateBasicBonus` method to ensure that the basic bonus is recalculated based on the new temporary stat value.
	 *
	 * @param value
	 */
	void updateTemporary(int value);

	/**
	 * @brief Make a stat gain roll
	 *
	 * This happens aspart of the levelling up process and/or the completion of a training package.
	 * The gain is determined by the `getStatGain` function, which takes into account the difference between the potential and temporary stat values.
	 *
	 * The resulting temporary stat can never be lowered below 1 through this process, even if the gain is negative.
	 */
	void performStatGainRoll();

	/**
	 * @brief Gets the total bonus provided by the stat, which is the sum of the basic bonus and the racial bonus.
	 *
	 * The total bonus is calculated by adding the basic bonus, which is determined by the temporary stat value, and the racial bonus, which is typically set at character creation based on the character race.
	 */
	int bonus() const {
		return basic_bonus_ + racial_bonus_;
	}

private:
	int temporary_{0};    // Temporary stat value that can be modified by various effects in the game, such as spells or temporary buffs/debuffs. This is the value that is used to calculate the basic bonus and potential stat value.
	int potential_{0};    // Potential stat value that is calculated at character creation and is very rarely modified in the game.
	int racial_bonus_{0}; // This is typically only set at character creation when the race is applied so we cache it as it is used frequently and is set and then never changes.
	int basic_bonus_{0};  // We cache thius valkue for performance reasons as it is used frequently and only changes when the temporary stat value changes, which is relatively infrequent compared to how often the bonus is calculated.
};

} // namespace rm::game::character
