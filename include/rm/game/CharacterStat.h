#pragma once

#include <memory>
#include <vector>
#include <Dice.h>
#include <GameObject.h>

namespace Character {

	/**
	 * @brief Get the basic bonus for the stat given the temporary value.
	 * @param temp_value temporary value of the stat to calculate the basic bonus for
	 * @return basic bonus for the stat based on the temporary value
	 */
	int getBasicBonus(int temp_value);

	/**
	 * @brief Get the potential value for the stat given the temporary value.
	 * @param temp_value temporary value of the stat to calculate the potential value for
	 * @return potential value for the stat based on the temporary value
	 */
	int getInitialPotentialStatValue(int temp_value);

	/**
	 * @brief Get the stat gain for a temporary stat given te difference between the temporary and potential values
	 * @param difference The difference between the temporary and potential values of the stat to calculate the stat gain for
	 * @return The temporary stat gain based on the difference in the stat values
	 */
	int getStatGain(int difference);

	/**
	 * @brief A class representing a modifgier to a value, such as a stat. This is used to represent temporary modifiers from spells or potential modifiers from skills or attribute increases that can affect the stat bonus.
	 * 
	 * The Modifier class is designed to be a base class for different types of modifiers that can affect a stat. It contains a virtual method `modification()` that can be overridden by derived classes to provide the specific logic for calculating the modification value based on the type of modifier.
	 * 
	 */
	class Modifier {
	public:
		virtual ~Modifier() = default; /** Virtual destructor to make the class polymorphic */
		virtual int modification(); /** Virtual method to calculate the modification value based on the type of modifier. This can be overridden by derived classes to provide specific logic for different types of modifiers. */
	};

	/**
	 * @class Stat
	 * @brief Represents a character stat in the game, such as strength or dexterity and contains the logic to determine any bonus the stat applies to rolls based on the temporary stat value and any modifiers that may be affecting the stat.
	 * 
	 * Each character will have 10 stats, each of which is represented by this class.
	 * 
	 */
	class Stat : public GameObject {
		friend class GameObjectFactory; // Allow the factory to access the private members of the class to set the unique identifier when creating objects
	public:

		/**
		 * @brief Sets the racial bonus value.
		 * 
		 * This is typically only set at character creation when the race is set.
		 * 
		 * @param bonus The racial bonus value to set.
		 */
		void setRacialBonus(int bonus) { racial_bonus_ = bonus; }

		/**
		 * @brief Add a bonus modifier to the stat.
		 * @param modifier Modifier to add to the stat bonus. This could be a temporary modifier from a spell or a potential modifier from a skill or attribute increase.
		 */
		void addBonusModifier(std::shared_ptr<Modifier> modifier) {
			modifiers_.push_back(modifier);
		}

		/**
		 * @brief Clear all bonus modifiers from the stat.
		 *
		 * This method removes all modifiers that have been added to the stat, effectively resetting any temporary or potential modifications that may have been applied.
		 * This can be useful in situations where the character's state changes significantly, such as when a spell wears off or when a new potential modifier is applied.
		 */
		void clearBonusModifiers() {
			modifiers_.clear();
		}

		/**
		 * @brief Remove a specific bonus modifier from the stat.
		 *
		 * This method removes a specific modifier from the list of modifiers affecting the stat. It uses the `std::remove` algorithm to find and remove the specified modifier from the vector of modifiers.
		 * This can be useful when a specific effect that was modifying the stat is no longer applicable, such as when a spell is dispelled or when a temporary buff expires.
		 *
		 * @param modifier The modifier to remove from the stat.
		 */
		void removeBonusModifier(std::shared_ptr<Modifier> modifier) {
			modifiers_.erase(std::remove(modifiers_.begin(), modifiers_.end(), modifier), modifiers_.end());
		}

		/**
		 * @brief Calculates the total stat bonus by summing up the temporary, potential, basic bonus, racial bonus, and special bonus.
		 * @return The total stat bonus.
		 */
		int bonus() const { return basic_bonus_ + racial_bonus_ + modifierTotal(); }

		/**
		 * @brief Gets the potential stat value.
		 * @return The potential stat value.
		 */
		int potential() const { return potential_; }

		/**
		 * @brief Gets the temporary stat value.
		 * @return The temporary stat value.
		 */
		int temporary() const { return temporary_; }

		/**
		 * @brief Sets the temporary stat value and calculates the initial potential stat value based on it.
		 *
		 * This method is used to set the temporary stat value, which may be modified by various effects in the game.
		 * After setting the temporary value, it calls the `getInitialPotentialStatValue` function to calculate the initial potential stat value based on the new temporary stat value.
		 *
		 * @param value The temporary stat value to set.
		 */
		void setTemporary(int value) { 
			temporary_ = value; 
			potential_ = getInitialPotentialStatValue(temporary_);
			basic_bonus_ = getBasicBonus(temporary_);
		}

		/**
		 * @brief Change the temporary stat value and recalculate the basic bonus accordingly.
 		 *
		 * This method is used to update the temporary stat value, which may be modified by various effects in the game.
		 * After updating the temporary value, it calls the `calculateBasicBonus` method to ensure that the basic bonus is recalculated based on the new temporary stat value.
		 *
		 * @param value 
		 */
		void updateTemporary(int value) {
			temporary_ += value;
			basic_bonus_ = getBasicBonus(temporary_);
		}

		/**
		 * @brief Make a stat gain roll
		 * 
		 * This happens aspart of the levelling up process and/or the completion of a training package.
		 * The gain is determined by the `getStatGain` function, which takes into account the difference between the potential and temporary stat values.
		 * 
		 * The resulting temporary stat can never be lowered below 1 through this process, even if the gain is negative.
		 */
		void makeStatgainRoll() {
			int gain = getStatGain(potential_ - temporary_);

			if (gain < 0) {
				// If the gain is negative then we need to ensure that we don't reduce the temporary stat value below 1.
				gain = std::max(gain, 1 - temporary_);
			}
			updateTemporary(gain);
		}


	private:
		int temporary_{ 0 }; // Temporary stat value that can be modified by various effects in the game, such as spells or temporary buffs/debuffs. This is the value that is used to calculate the basic bonus and potential stat value.
		int potential_{ 0 }; // Potential stat value that is calculated at character creation and is very rarely modified in the game.
		int racial_bonus_{ 0 }; // This is typically only set at character creation when the race is applied so we cache it as it is used frequently and is set and then never changes.
		int basic_bonus_{ 0 }; // We cache thius valkue for performance reasons as it is used frequently and only changes when the temporary stat value changes, which is relatively infrequent compared to how often the bonus is calculated.
		std::vector<std::shared_ptr<Modifier>> modifiers_; // List of modifiers that affect the stat, such as temporary modifiers from spells or potential modifiers from skills or attribute increases

		/**
		 * @brief Get the sum of the modifications from all the modifiers affecting the stat.
		 * @return Sum of the modifications from all the modifiers affecting the stat.
		 */
		int modifierTotal() const {
			int total{ 0 };
			for (const auto& modifier : modifiers_) {
				total += modifier->modification();
			}
			return total;
		}
	};

	int getBasicBonus(int temp_value) {
		if (temp_value <= 10) return (temp_value - 21) / 2;
		if (temp_value <= 30) return (temp_value - 35) / 5;
		if (temp_value <= 69) return 0;
		if (temp_value <= 89) return (temp_value - 65) / 5;
		if (temp_value <= 100) return (temp_value - 80) / 2;
		return (temp_value - 95) * 2;
	};

	int getInitialPotentialStatValue(int temp_value) {
		if (temp_value <= 24) return 20 + archid::Dice(10).roll(8).result();
		if (temp_value <= 34) return 30 + archid::Dice(10).roll(7).result();
		if (temp_value <= 44) return 40 + archid::Dice(10).roll(6).result();
		if (temp_value <= 54) return 50 + archid::Dice(10).roll(5).result();
		if (temp_value <= 64) return 60 + archid::Dice(10).roll(4).result();
		if (temp_value <= 74) return std::max(temp_value, 70 + archid::Dice(10).roll(3).result());
		if (temp_value <= 84) return std::max(temp_value, 80 + archid::Dice(10).roll(2).result());
		if (temp_value <= 91) return 90 + archid::Dice(10).roll().result();
		if (temp_value == 92) return 91 + archid::Dice(9).roll().result();
		if (temp_value == 93) return 92 + archid::Dice(8).roll().result();
		if (temp_value == 94) return 93 + archid::Dice(7).roll().result();
		if (temp_value == 95) return 94 + archid::Dice(6).roll().result();
		if (temp_value == 96) return 95 + archid::Dice(5).roll().result();
		if (temp_value == 97) return 96 + archid::Dice(4).roll().result();
		if (temp_value == 98) return 97 + archid::Dice(3).roll().result();
		if (temp_value == 99) return 98 + archid::Dice(2).roll().result();
		return 99 + archid::Dice(2).roll().result();
	};

	int getStatGain(int difference) {
		// This uses two dice rolls and the difference between the stats determines how they are used.
		int roll1 = archid::Dice(10).roll().result();
		int roll2 = archid::Dice(10).roll().result();

		// Special case if the dice rolls are the same.
		if (roll1 == roll2) {
			if (roll1 <= 5) return -roll1;
			return roll1 * 2;
		}

		// If we get this far then the rolls are different and we need to determine which is the high and low roll.
		int high_roll = std::max(roll1, roll2);
		int low_roll = std::min(roll1, roll2);

		if (difference <= 10) return low_roll;
		if (difference <= 20) return high_roll;
		return high_roll + low_roll;
	};

}

