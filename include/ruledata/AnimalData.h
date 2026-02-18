#pragma once

#include <set>
#include <string_view>

#include <CreatureConstitutionVarianceType.h>
#include <CreatureLevelVarianceType.h>
#include <CreatureBonusXpType.h>
#include <GameRuleData.h>
#include <ManoeuvreDifficultyType.h>
#include <table/CreatureBonusXpTable.h>

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
	void setBonusXpCode(CreatureBonusXpType::Type bonus_xp_code) { bonus_xp_code_ = bonus_xp_code; }

  /**
	 * @brief Get the bonus XP code for the animal
	 * @return Bonus XP code for the animal, used to determine how much bonus XP is awarded for killing the animal.
	 */
	CreatureBonusXpType::Type bonusXpCode() const { return bonus_xp_code_; }

	/**
	 * @brief Set the bonus constitution code for the animal
	 * @param constitution_variance_type Bonus constitution code for the animal, used to determine how much constitution variance a creature has.
	 */
	void setConstitutionVarianceType(CreatureConstitutionVarianceType::Type constitution_variance_type) { constitution_variance_type_ = constitution_variance_type; }

	/**
	 * @brief Get the bonus constitution code for the animal
	 * @return Bonus constitution code for the animal, used to determine how much constitution variance a creature has.
	 */
	CreatureConstitutionVarianceType::Type constitutionVarianceType() const { return constitution_variance_type_; }

	/**
	 * @brief Set the bonus level code for the animal
	 * @param level_variance_type Bonus level code for the animal, used to determine how much level variance a creature has.
	 */
	void setLevelVarianceType(CreatureLevelVarianceType::Type level_variance_type) { level_variance_type_ = level_variance_type; }

	/**
	 * @brief Get the bonus level code for the animal
	 * @return Bonus level code for the animal, used to determine how much level variance a creature has.
	 */
	CreatureLevelVarianceType::Type levelVarianceType() const { return level_variance_type_; }

	/**
	 * @brief Set the average level of the animal
	 * @param average_level Average level of the animal, used to determine how much damage it can take before it dies.
	 */
	void setAverageLevel(int average_level) { average_level_ = average_level; }

	/**
	 * @brief Get the average level of the animal
	 * @return Average level of the animal, used to determine how much damage it can take before it dies.
	 */
	int averageLevel() const { return average_level_; }

	/**
	 * @brief Get the number of hits for the animal at a given level
	 *
	 * This is used to determine how much damage an animal can take before it dies at a given level. The number of hits is calculated based on the base hits, the constitution code and a random roll.
	 *
	 * @param level Level of the animal, used to determine how much damage it can take before it dies.
	 * @return Number of hits for the animal at the given level, used to determine how much damage it can take before it dies.
	 * 
	 * TODO: Take into account the average level of animal.
	 */
	int hits(int level) const;

private:
	std::string name_{}; /**< In game name of the animal */
	std::string description_{}; /**< Description of the animal for flavour purposes */
	int base_hits_{}; /**< Base hits for the animal, used to determine how much damage it can take before it dies. This is not used for animals that are not meant to be attacked such as fish. */
	int base_movement_{}; /**< Base movement for the animal, used to determine how far it can move in a turn. */
	int defensive_bonus_{}; /**< Defensive bonus for the animal, used to determine how much damage it takes when attacked. */
	int frequency_factor_{}; /**< Frequency factor for the animal, used to determine how common it is to find in the appropriate environ. */
	int moving_manoeuvre_bonus_{}; /**< Bonus to Manoeuvre rolls when the animal is moving, used to determine how difficult it is to hit the animal when it is moving. */
	CreatureBonusXpType::Type bonus_xp_code_{}; /**< Bonus XP code for the animal, used to determine how much bonus XP is awarded for killing the animal. */
	CreatureConstitutionVarianceType::Type constitution_variance_type_{}; /**< Bonus constitution code for the animal, used to determine how much constitution variance a creature has. */
	CreatureLevelVarianceType::Type level_variance_type_{}; /**< Bonus level code for the animal, used to determine how much level variance a creature has. */
	int average_level_{}; /**< Average level of the animal, used to determine how much damage it can take before it dies. */

	/**
	 * @brief Gets the number of hits per level difference based on the constitution code.
	 * 
	 * This value is used to determine how much the hit points of an animal may vary for any given level based on its constitution code. Higher constitution codes represent greater variance in hit points.
	 * @return The number of hits per level difference.
	 */
	int hitsPerLevelDifference() const;

	/**
	 * @brief Gets the bonus exhaustion points.
	 * 
	 * This value is used when calculating the exhaustion points for an animal based on its constitution code. Animals with a bonus constitution code will have bonus exhaustion points added to their exhaustion points.
	 * @return The bonus exhaustion points value.
	 */
	int bonusExhaustionPoints() const;

	/**
	 * @brief Gets the stamina bonus.
	 * 
	 * This value is used when calculating the number of hits for an animal based on its constitution code and a random roll.
	 * @return The stamina bonus value.
	 */
	int staminaBonus() const;

};
