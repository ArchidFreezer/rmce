#pragma once

#include <set>
#include <string_view>

#include <GameRuleData.h>
#include <ManoeuvreDifficultyType.h>
#include <table/BoundIntRowLookupTable.h>

class CreatureBonusXpTable;

/**
 * @class AnimalData
 * @brief Representation of an animal that may be found in the game.
 * 
 * There are both specific animals and generic animal types that are defined with the specific animals being fully defined specialisations of generic types.
 * The specific animals will be either commonly found animals that have a significant role in the game or unique animals that are only found in one location.
 * The generic animal types will be used to define the animals that are not significanrt enough to warrant a dedicated entry. The generic types may have a
 * list of specific animal names for flavour purposes, but will use the generic data. For example there may be a generic animal type of "Fish, Medium" with
 * a list of specific animal names such as "Trout", "Salmon" and "Pike".
 */
class AnimalData : public GameRuleData {
public:

	/**
	 * @brief Enumeration class representing the bonus XP code for an animal
	 *
	 * This is used to determine how much bonus XP is awarded for killing the animal. The actual bonus XP is determined by a lookup table that maps the bonus XP code to a bonus XP value.
	 */
	enum class BonusXpCode { kNone, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL	};

	/**
 * @brief Get the enumeration value based on a string and set it to the provided reference
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @param type reference to set the BonusXpCode represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid BonusXpCode
 *
 * @see toString()
 */
	void fromString(std::string_view sv, BonusXpCode& type);

	/**
	 * @brief Default constructor is deleted to ensure the base class is initialised
	 */
	AnimalData() = delete;

	/**
	 * @brief Constructor to initialise base class
	 * @param id Unique identifier for the animal
	 */
	AnimalData(std::string_view id) : GameRuleData(id) {};

	/**
	 * @brief Set the name of the animal
	 * @param name Animal name
	 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the animal
	 * @return Animal name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of the animal
	 * @param description Description of the animal for flavour purposes
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Get the description of the animal
	 * @return Description of the animal for flavour purposes as a string reference
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set the base hits of the animal
	 * @param base_hits Base hits for the animal, used to determine how much damage it can take before it dies. This is not used for animals that are not meant to be attacked such as fish.
	 */
	void setBaseHits(int base_hits) { base_hits_ = base_hits; }

	/**
	 * @brief Get the base hits of the animal
	 * @return Base hits for the animal, used to determine how much damage it can take before it dies. This is not used for animals that are not meant to be attacked such as fish.
	 */
	int baseHits() const { return base_hits_; }

	/**
	 * @brief Set the base movement of the animal
	 * @param base_movement Base movement for the animal, used to determine how far it can move in a turn.
	 */
	void setBaseMovement(int base_movement) { base_movement_ = base_movement; }

	/**
	 * @brief Get the base movement of the animal
	 * @return Base movement for the animal, used to determine how far it can move in a turn.
	 */
	int baseMovement() const { return base_movement_; }

	/**
	 * @brief Set the defensive bonus of the animal
	 * @param defensive_bonus Defensive bonus for the animal, used to determine how much damage it takes when attacked. This is not used for animals that are not meant to be attacked such as fish.
	 */
	void setDefensiveBonus(int defensive_bonus) { defensive_bonus_ = defensive_bonus; }

	/**
	 * @brief Get the defensive bonus of the animal
	 * @return Defensive bonus for the animal, used to determine how much damage it takes when attacked. This is not used for animals that are not meant to be attacked such as fish.
	 */
	int defensiveBonus() const { return defensive_bonus_; }

	/**
	 * @brief Set the frequency factor of the animal
	 * 
	 * The minimum value is 1 and the maximum value is 9 with 1 being the most common and 9 being the least common. Any value outside of this range will be set to the closest valid value.
	 * @param frequency_factor Frequency factor for the animal, used to determine how common it is to find in the appropriate environ.
	 */
	void setFrequencyFactor(int frequency_factor);

	/**
	 * @brief Get the frequency factor of the animal
	 * 
	 * The minimum value is 1 and the maximum value is 9 with 1 being the most common and 9 being the least common.
	 * @return Frequency factor for the animal, used to determine how common it is to find in the appropriate environ.
	 * 
	 * @see huntingModifier() for how the frequency factor is used to determine the difficulty of hunting the animal
	 * @see existencePercentageChance() for how the frequency factor is used to determine the chance of an animal existing in an appropriate environment
	 */
	int frequencyFactor() const { return frequency_factor_; }

	/**
	 * @brief Get the modifier to a Hunting skill roll when attempting to hunt this particular animal
	 * @return Hunting modifier for the animal, used to determine how difficult it is to hunt the animal. This is derived from the frequency factor with more common animals being easier to hunt and less common animals being harder to hunt.
	 */
	int huntingModifier() const;

	/**
	 * @brief Get the percentage chance of an animal existing in an appropriate environment
	 * 
	 * This is derived from the frequency factor with more common animals being more likely to be found and less common animals being less likely to be found.
	 * 
	 * @return Percentage chance of an animal existing in an appropriate environment, used to determine if the animal is present when searching for animals in the environment.
	 */
	int existencePercentageChance() const;

	/**
	 * @brief Set the moving manoeuvre bonus of the animal
	 * 
	 * The MM Bonus is used if the Gamemaster requires a manoeuver roll to move at paces greater than a walk.
	 * 
	 * @param moving_manoeuvre_bonus Bonus to Manoeuvre rolls when the animal is moving, used to determine how difficult it is to hit the animal when it is moving.
	 */
	void setMovingManoeuvreBonus(int moving_manoeuvre_bonus) { moving_manoeuvre_bonus_ = moving_manoeuvre_bonus; }

	/**
	 * @brief Get the moving manoeuvre bonus of the animal
	 *
	 * The MM Bonus is used if the Gamemaster requires a manoeuver roll to move at paces greater than a walk.
	 *
	 * @return Bonus to Manoeuvre rolls when the animal is moving, used to determine how difficult it is to hit the animal when it is moving.
	 */
	int movingManoeuvreBonus() const { return moving_manoeuvre_bonus_; }

	/**
	 * @brief Set the bonus XP code for the animal
	 * @param bonus_xp_code Bonus XP code for the animal, used to determine how much bonus XP is awarded for killing the animal.
	 */
	void setBonusXpCode(BonusXpCode bonus_xp_code) { bonus_xp_code_ = bonus_xp_code; }

	/**
	 * @brief Get the string representation of a BonusXpCode enumeration value
	 *
	 * The output is the same as the input accepted by fromString() for the same enumeration value.
	 *
	 * @return String representation of the BonusXpCode enumeration value
	 *
	 * @see fromString()
	 */
	std::string bonusXpCodeToString() const;

	/**
	 * @brief Set the creature bonus XP table to use for determining the bonus XP awarded for killing the animal based on its bonus XP code
	 * @param creature_bonus_xp_table reference to the creature bonus XP table to use for determining the bonus XP awarded for killing the animal based on its bonus XP code
	 */
	void setCreatureBonusXpTable(const CreatureBonusXpTable& creature_bonus_xp_table) { creature_bonus_xp_table_ = &creature_bonus_xp_table; }


private:
	std::string name_{}; /**< In game name of the animal */
	std::string description_{}; /**< Description of the animal for flavour purposes */
	int base_hits_{}; /**< Base hits for the animal, used to determine how much damage it can take before it dies. This is not used for animals that are not meant to be attacked such as fish. */
	int base_movement_{}; /**< Base movement for the animal, used to determine how far it can move in a turn. */
	int defensive_bonus_{}; /**< Defensive bonus for the animal, used to determine how much damage it takes when attacked. */
	int frequency_factor_{}; /**< Frequency factor for the animal, used to determine how common it is to find in the appropriate environ. */
	int moving_manoeuvre_bonus_{}; /**< Bonus to Manoeuvre rolls when the animal is moving, used to determine how difficult it is to hit the animal when it is moving. */
	BonusXpCode bonus_xp_code_{}; /**< Bonus XP code for the animal, used to determine how much bonus XP is awarded for killing the animal. */
	const CreatureBonusXpTable* creature_bonus_xp_table_{}; /**< Pointer to the creature bonus XP table, used to determine how much bonus XP is awarded for killing the animal based on its bonus XP code. */

};

/**
 * @class TableColumnBonusXpCodeMatcher
 * @brief Class to match the bonus XP code to a table column index
 *
 * This is used as the column matcher for the CreatureBonusXpTable class.It takes a BonusXpCode and returns the corresponding column index for that code.
 */
class TableColumnBonusXpCodeMatcher {
public:
	/**
	 * @brief Get the index of the table column
	 * @return int table column index
	 */
	int column(AnimalData::BonusXpCode match) const;
};

/**
 * @class CreatureBonusXpTable
 * @brief Class to contain the creature bonus XP table
 *
 * This is a simple lookup table that takes a bonus XP code and a character level, returning the bonus XP awarded for killing or subduing an animal.
 * The bonus XP code is defined in the AnimalData class
 */
class CreatureBonusXpTable : public BoundIntRowLookupTable<TableColumnBonusXpCodeMatcher, AnimalData::BonusXpCode, int> {
public:

	/**
	 * @brief Constructor defining the name of the table
	 *
	 */
	CreatureBonusXpTable(std::string_view id) : BoundIntRowLookupTable(id, 1, 250) {}

	/**
	 * @brief In-game name of the table as used by players and NPCs
	 * @return Name of the table
	 */
	const std::string& name() const { return "Creature Bonus XP Table"; }

};
