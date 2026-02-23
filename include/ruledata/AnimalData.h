#pragma once

#include <set>
#include <string_view>
#include <vector>

#include <AnimalOutlookType.h>
#include <AnimalAttack.h>
#include <ArmourType.h>
#include <CreatureConstitutionVarianceType.h>
#include <LevelVarianceType.h>
#include <CreatureBonusXpType.h>
#include <CreatureMovementSpeedType.h>
#include <CreaturePaceData.h>
#include <CreatureSizeType.h>
#include <CriticalModifierType.h>
#include <CriticalSizeTableType.h>
#include <GameRuleData.h>
#include <Location.h>
#include <ManoeuvreDifficultyType.h>
#include <NumberRange.h>
#include <TreasureCodeData.h>
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
	 * @brief Set the carry capacity of the animal
	 * @param carry_capacity Carry capacity of the animal, used to determine how much weight the animal can carry when used as a mount or pack animal.
	 */
	void setCarryCapacity(int carry_capacity) { carry_capacity_ = carry_capacity; }

	/**
	 * @brief Get the carry capacity of the animal
	 * @return Carry capacity of the animal, used to determine how much weight the animal can carry when used as a mount or pack animal.
	 */
	int carryCapacity() const { return carry_capacity_; }

	/**
	 * @brief Set the riding bonus of the animal
	 * @param riding_bonus Riding bonus of the animal, used to determine how difficult it is to ride the animal when used as a mount.
	 */
	void setRidingBonus(int riding_bonus) { riding_bonus_ = riding_bonus; }

	/**
	 * @brief Get the riding bonus of the animal
	 * @return Riding bonus of the animal, used to determine how difficult it is to ride the animal when used as a mount.
	 */
	int ridingBonus() const { return riding_bonus_; }

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
	 * 
	 * This is used behind the scenes to generate the hits for an animal for a given level.
	 * 
	 * @param constitution_variance_type Bonus constitution code for the animal, used to determine how much constitution variance a creature has.
	 */
	void setConstitutionVarianceType(CreatureConstitutionVarianceType::Type constitution_variance_type) { constitution_variance_type_ = constitution_variance_type; }

	/**
	 * @brief Get the bonus constitution code for the animal
	 *
	 * This is used behind the scenes to generate the hits for an animal for a given level.
	 *
	 * @return Bonus constitution code for the animal, used to determine how much constitution variance a creature has.
	 */
	CreatureConstitutionVarianceType::Type constitutionVarianceType() const { return constitution_variance_type_; }

	/**
	 * @brief Set the  level variance code for the animal
	 * 
	 * In order to use this it should be applied to the LevelVarianceTable with an open-ended d100 roll
	 * 
	 * @code{.cpp}
	 * GameRuleDataCache cache{};
	 * GameRuleDataFactory factory{ cache };
	 * 
	 * std::string table_id = "CREATURE_LEVEL_VARIANCE_TABLE";
	 * LevelVarianceTable& table = factory.get<LevelVarianceTable>(table_id);
	 * 
	 * int variance = table.cell(animal.levelVarianceType(), Dice(100, 5).roll().result());
	 * int animal_level = animal.averageLevel() + variance;
	 * @endcode
	 * 
	 * @param level_variance_type Level variance code for the animal, used to determine how much level variance a creature has.
	 * @see LevelVarianceTable for how the level variance code is used to determine the level variance of a creature.
	 */
	void setLevelVarianceType(LevelVarianceType::Type level_variance_type) { level_variance_type_ = level_variance_type; }

	/**
	 * @brief Get the level varaince code for the animal
	 *
	 * In order to use this it should be applied to the LevelVarianceTable with an open-ended d100 roll
	 *
	 * @code{.cpp}
	 * GameRuleDataCache cache{};
	 * GameRuleDataFactory factory{ cache };
	 *
	 * std::string table_id = "CREATURE_LEVEL_VARIANCE_TABLE";
	 * LevelVarianceTable& table = factory.get<LevelVarianceTable>(table_id);
	 *
	 * int variance = table.cell(animal.levelVarianceType(), Dice(100, 5).roll().result());
	 * int animal_level = animal.averageLevel() + variance;
	 * @endcode
	 *
	 * @return Level variance code for the animal, used to determine how much level variance a creature has.
	 * @see LevelVarianceTable for how the level variance code is used to determine the level variance of a creature.
	 */
	LevelVarianceType::Type levelVarianceType() const { return level_variance_type_; }

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
	 */
	int hits(int level) const;

	/**
	 * @brief Get the exhaustion points for the animal
	 *
	 * The exhaustion points are calculated based on the constitution code and a random roll.
	 *
	 * @return Exhaustion points for the animal, used to determine how much exhaustion an animal gains when it is attacked.
	 */
	int exhaustionPoints() const;

	/**
	 * @brief Sets the treasure code data for this object.
	 * @param treasure_code A pointer to the treasure code data to associate with this object. May be null to clear the current treasure code.
	 */
	void setTreasureCode(const TreasureCodeData& treasure_code) { treasure_code_ = &treasure_code; }

	/**
	 * @brief Gets the treasure code data associated with this object, if any.
	 *
	 * The treasure code is stored as a pointer and may not have been initialised so it is considered optional. A check should be made
	 * before using the value to determine if it has been set yet:
	 * @code{.cpp}
	 * if (animal.treasureCode()) {                        // Check if the treasure code has been set
	 *   const TreasureCodeData* treasure_code = animal.treasureCode().value();   // Get the treasure code pointer
	 *   // Use the code
	 * }
	 * @endcode
	 * or
	 * @code
	 * std::cout << (animal.treasureCode() ? animal.treasureCode().value()->id() : "Treasure code not set") << std::endl;
	 * @endcode
	 * @return An optional containing a pointer to the treasure code data associated with this object, or an empty optional if no treasure code is associated.
	 */
	const std::optional<const TreasureCodeData*> treasureCode() const { return treasure_code_; }

	/**
	 * @brief Set the size of the animal
	 * @param size Size of the animal, used as a relative guide with Medium being approximately man sized
	 */
	void setSize(CreatureSizeType::Type size) { size_ = size; }

	/**
	 * @brief Get the size of the animal
	 * @return Size of the animal, used as a relative guide with Medium being approximately man sized
	 */
	CreatureSizeType::Type size() const { return size_; }

	/**
	 * @brief Set the armour type of the animal
	 * @param armour_type Armour type of the animal, used to determine how much damage it takes when attacked.
	 */
	void setArmourType(ArmourType::Type armour_type) { armour_type_ = armour_type; }

	/**
	 * @brief Get the armour type of the animal
	 * @return Armour type of the animal, used to determine how much damage it takes when attacked.
	 */
	ArmourType::Type armourType() const { return armour_type_; }

	/**
	 * @brief Set the movement speed of the animal
	 * @param movement_speed Movement speed of the animal, used to determine how far it can move in a turn.
	 */
	void setMovementSpeed(CreatureMovementSpeedType::Type movement_speed) { movement_speed_ = movement_speed; }

	/**
	 * @brief Get the movement speed of the animal
	 * @return Movement speed of the animal, used to determine how far it can move in a turn.
	 */
	CreatureMovementSpeedType::Type movementSpeed() const { return movement_speed_; }

	/**
	 * @brief Set the attack quickness of the animal
	 * @param attack_quickness The speed that the creature attacks at, impacting initiative and DB
	 */
	void setAttackQuickness(CreatureMovementSpeedType::Type attack_quickness) { attack_quickness_ = attack_quickness; }

	/**
	 * @brief Get the attack quickness of the animal
	 * @return The speed that the creature attacks at, impacting initiative and DB
	 */
	CreatureMovementSpeedType::Type attackQuickness() const { return attack_quickness_; }

	/**
	 * @brief Set the maximum pace of the animal
	 * @param max_pace Pointer to the maximum pace of the animal, used to determine how far it can move in a turn based on its movement speed. This is stored as a pointer to avoid having to copy the pace data for each animal and instead just reference the same data for all animals with the same movement speed.
	 */
	void setMaxPace(const CreaturePaceData& max_pace) { max_pace_ = &max_pace; }

	/**
	 * @brief Get the pace data for the animal
	 * @return Pointer to the pace data for the animal, used to determine how far it can move in a turn based on its movement speed. This is stored as a pointer to avoid having to copy the pace data for each animal and instead just reference the same data for all animals with the same movement speed.
	 */
	const CreaturePaceData* maxPace() const { return max_pace_; }

	/**
	 * @brief Set the outlook of the animal
	 * @param outlook Outlook of the animal, used to determine how it behaves in combat and how it reacts to the world around it.
	 */
	void setOutlook(AnimalOutlookType::Type outlook) { outlook_ = outlook; }

	/**
	 * @brief Get the outlook of the animal
	 * @return Outlook of the animal, used to determine how it behaves in combat and how it reacts to the world around it.
	 */
	AnimalOutlookType::Type outlook() const { return outlook_; }

	/**
	 * @brief Set the critical table type for the animal
	 * @param critical_table_type Critical table type for the animal, used to determine which critical table to use when the animal is hit with a critical hit.
	 */
	void setCriticalTableType(CriticalSizeTableType::Type critical_table_type) { critical_table_type_ = critical_table_type; }

	/**
	 * @brief Get the critical table type for the animal
	 * @return Critical table type for the animal, used to determine which critical table to use when the animal is hit with a critical hit.
	 */
	CriticalSizeTableType::Type criticalTableType() const { return critical_table_type_; }

	/**
	 * @brief Add a critical modifier to the animal
	 * @param critical_modifier Critical modifier for the animal, used to determine which critical modifiers apply when the animal is hit with a critical hit.
	 */
	void addCriticalModifier(CriticalModifierType::Type critical_modifier) { critical_modifiers_.emplace(critical_modifier); }

	/**
	 * @brief Set the critical modifiers for the animal
	 * @param critical_modifiers Set of critical modifiers for the animal, used to determine which critical modifiers apply when the animal is hit with a critical hit.
	 */
	void setCriticalModifiers(std::set<CriticalModifierType::Type> critical_modifiers) { critical_modifiers_ = std::move(critical_modifiers); }

	/**
	 * @brief Get the set of critical modifiers for the animal
	 * @return Set of critical modifiers for the animal, used to determine which critical modifiers apply when the animal is hit with a critical hit.
	 */
	const std::set<CriticalModifierType::Type>& criticalModifiers() const { return critical_modifiers_; }

	/**
	 * @brief Set the encounter range for the animal
	 * @param encounter_range A pair containing the minimum and maximum number of animals typically enountered in a single encounter, used to determine how many animals are encountered when an encounter with the animal is generated.
	 */
	void setEncounterRange(std::pair<int, int> encounter_range) { encounter_range_ = encounter_range; }

	/**
	 * @brief Get the encounter range for the animal
	 * @return A pair containing the minimum and maximum number of animals typically enountered in a single encounter, used to determine how many animals are encountered when an encounter with the animal is generated.
	 */
	const std::pair<int, int> encounterRange() const { return encounter_range_; }

	/**
	 * @brief Set the number of young range for the animal
	 * @param number_young_range A pair containing the minimum and maximum number of young typically born in a single birth, used to determine how many young are born when a birth event is generated for the animal.
	 */
	void setNumberYoungRange(std::pair<int, int> number_young_range) { number_young_range_ = number_young_range; }

	/**
	 * @brief Get the number of young range for the animal
	 * @return A pair containing the minimum and maximum number of young typically born in a single birth, used to determine how many young are born when a birth event is generated for the animal.
	 */
	const std::pair<int, int> numberYoungRange() const { return number_young_range_; }

	/**
	 * @brief Set the location definition for the animal
	 * @param location Location definition for the animal, used to determine where the animal can be found in the game world. This is used to match against specific locations to determine if the animal can be found there.
	 */
	void setLocation(Location location) { location_ = std::make_unique<Location>(std::move(location)); }

	/**
	 * @brief Get the location for the animal
	 * @return Location definition for the animal, used to determine where the animal can be found in the game world. This is used to match against specific locations to determine if the animal can be found there.
	 */
	const Location& location() const { return *location_; }

	/**
	 * @brief Add an attack to the animal
	 * @param chance_range Pointer to a NumberRange<int> that represents the chance of the attack being used in a round, used as the key for the attack in the attacks map.
	 * @param attack AnimalAttack object that represents the attack, used as the value for the attack in the attacks map.
	 */
	void addAttack(const NumberRange<int>* chance_range, AnimalAttack attack) { attacks_.emplace(chance_range, std::move(attack)); }

	/**
	 * @brief Get the map of attacks for the animal
	 * @return Map of attack data for the animal, keyed by a pointer to a NumberRange<int> that represents the chance of the attack being used in a round. This is stored as a pointer to avoid having to copy the attack data for each animal and instead just reference the same data for all animals with the same attack chances.
	 */
	const std::map<const NumberRange<int>*, AnimalAttack>& attacks() const { return attacks_; }

	/**
	 * @brief Get the attack for a given d100 roll
	 * @param d100_roll The result of a d100 roll, used to determine which attack is used in a round based on the chance ranges defined for each attack.
	 * @return The attack for the given d100 roll, determined by finding the first attack in the attacks map where the d100 roll falls within the chance range key. If no such attack is found, a default constructed AnimalAttack is returned.
	 */
	AnimalAttack getAttack(int d100_roll) const { return getAttack(1, d100_roll); };

	/**
	 * @brief Get the attack for a given d100 roll
	 * @param num_attackers The number of attackers in the round, used to determine which attack is used in a round based on the chance ranges defined for each attack. This is used to determine if the attack should be used based on the number of attackers in the round and the chance range for the attack. For example, an attack may only be used if there are 2 or more attackers in the round, so if there is only 1 attacker then that attack would not be used regardless of the d100 roll.
	 * @param d100_roll The result of a d100 roll, used to determine which attack is used in a round based on the chance ranges defined for each attack.
	 * @return The attack for the given d100 roll, determined by finding the first attack in the attacks map where the d100 roll falls within the chance range key. If no such attack is found, a default constructed AnimalAttack is returned.
	 */
	AnimalAttack getAttack(int num_attackers, int d100_roll) const;

	/**
	 * @brief Add an attack to be used based on the number of attackers in the round
	 * 
	 * This is typically used for small creatures where a single one is not very dangerous but a group of them can be. For example, Piranhas on there ownb are not that dangerous, but a shoal can be deadly.
	 * If there are multiple of these defined the one with the largest group size that is less than or equal to the number of attackers in the round will be used. For example, if there are attacks defined for 2 or more attackers and 4 or more attackers and there are 3 attackers in the round then the attack for 2 or more attackers will be used as it is the largest group size that is less than or equal to the number of attackers in the round.
	 * 
	 * @param num_attackers The number of attackers in the round, used to determine if the attack should be used based on the number of attackers in the round and the chance range for the attack. For example, an attack may only be used if there are 2 or more attackers in the round, so if there is only 1 attacker then that attack would not be used regardless of the d100 roll.
	 * @param attack AnimalAttack object that represents the attack, used as the value for the attack in the attacks_by_num_attackers map with the key being the number of attackers required for the attack to be used.
	 */
	void addGroupAttack(int num_attackers, AnimalAttack attack) { group_attacks_.emplace(num_attackers, std::move(attack)); }

	/**
	 * @brief Get the map of attacks for the animal based on the number of attackers in the round
	 *
	 * This is typically used for small creatures where a single one is not very dangerous but a group of them can be. For example, Piranhas on there ownb are not that dangerous, but a shoal can be deadly.
	 * If there are multiple of these defined the one with the largest group size that is less than or equal to the number of attackers in the round will be used. For example, if there are attacks defined for 2 or more attackers and 4 or more attackers and there are 3 attackers in the round then the attack for 2 or more attackers will be used as it is the largest group size that is less than or equal to the number of attackers in the round.
	 *
	 * @return Map of attack data for the animal, keyed by the number of attackers in the round. This is used to determine if the attack should be used based on the number of attackers in the round and the chance range for the attack. For example, an attack may only be used if there are 2 or more attackers in the round, so if there is only 1 attacker then that attack would not be used regardless of the d100 roll.
	 */
	const std::map<int, AnimalAttack>& groupAttacks() const { return group_attacks_; }

	/**
	 * @brief Get the attack for a given number of attackers in the round
	 *
	 * This is typically used for small creatures where a single one is not very dangerous but a group of them can be. For example, Piranhas on there ownb are not that dangerous, but a shoal can be deadly.
	 * If there are multiple of these defined the one with the largest group size that is less than or equal to the number of attackers in the round will be used. For example, if there are attacks defined for 2 or more attackers and 4 or more attackers and there are 3 attackers in the round then the attack for 2 or more attackers will be used as it is the largest group size that is less than or equal to the number of attackers in the round.
	 * 
	 * The reurn value needs to be checked to see if an attack was found for the given number of attackers in the round as it is possible that there are no attacks defined for the number of attackers in the round. For example, if there are attacks defined for 4 or more attackers and 6 or more attackers and there are only 3 attackers in the round then no attack would be found and an empty optional would be returned.
	 *
	 * @param num_attackers The number of attackers in the round, used to determine if the attack should be used based on the number of attackers in the round and the chance range for the attack. For example, an attack may only be used if there are 2 or more attackers in the round, so if there is only 1 attacker then that attack would not be used regardless of the d100 roll.
	 * @return The attack for the given number of attackers in the round, determined by finding the attack in the attacks_by_num_attackers map with the largest key that is less than or equal to the number of attackers in the round. If no such attack is found, a default constructed AnimalAttack is returned.
	 */
	std::optional<AnimalAttack> getAttackByNumAttackers(int num_attackers) const;

	/**
	 * @brief Add a ranged attack to the animal
	 * @param attack AnimalAttack object that represents the ranged attack, used to determine which attacks are ranged attacks. This is used to determine if the animal can make ranged attacks and which attacks are ranged attacks.
	 */
	void addRangedAttack(AnimalAttack attack) { ranged_attacks_.emplace_back(std::move(attack)); }

	/**
	 * @brief Get the vector of ranged attacks for the animal
	 * @return Vector of ranged attack data for the animal, used to determine which attacks are ranged attacks. This is used to determine if the animal can make ranged attacks and which attacks are ranged attacks.
	 */
	const std::vector<AnimalAttack>& rangedAttacks() const { return ranged_attacks_; }

	/**
	 * @brief Add a conditional attack to the animal
	 *
	 * A conditional attack is an attack that is only used if a certain condition is met. The condition is determined by the reference integer which can be used to look up the condition in a table or to determine the condition in some other way. For example, the reference integer could be used to determine if the attack is only used during a certain phase of combat or if it is only used if the animal has taken a certain amount of damage.
	 *
	 * @param ref The reference integer for the conditional attack, used to determine the condition under which the attack is used.
	 * @param attack AnimalAttack object that represents the conditional attack, used to determine the details of the attack and as the value for the conditional attack in the conditional_attacks map with the key being the reference integer for the condition under which the attack is used.
	 */
	void addConditionalAttack(int ref, AnimalAttack attack) { conditional_attacks_.emplace(ref, std::move(attack)); }

	/**
	 * @brief Get the conditional attack for a given reference integer
	 *
	 * A conditional attack is an attack that is only used if a certain condition is met. The condition is determined by the reference integer which can be used to look up the condition in a table or to determine the condition in some other way. For example, the reference integer could be used to determine if the attack is only used during a certain phase of combat or if it is only used if the animal has taken a certain amount of damage.
	 *
	 * The return value needs to be checked to see if an attack was found for the given reference integer as it is possible that there are no attacks defined for the reference integer. For example, if there are attacks defined for reference integers 1 and 2 and the reference integer 3 is passed in then no attack would be found and an empty optional would be returned.
	 *
	 * @param ref The reference integer for the conditional attack, used to determine the condition under which the attack is used.
	 * @return The conditional attack for the given reference integer, determined by looking up the reference integer in the conditional_attacks map. If no such attack is found, a default constructed AnimalAttack is returned.
	 */
	std::optional<AnimalAttack> getConditionalAttack(int ref) const;

	/**
	 * @brief Get the map of conditional attacks for the animal
	 *
	 * A conditional attack is an attack that is only used if a certain condition is met. The condition is determined by the reference integer which can be used to look up the condition in a table or to determine the condition in some other way. For example, the reference integer could be used to determine if the attack is only used during a certain phase of combat or if it is only used if the animal has taken a certain amount of damage.
	 *
	 * @return Map of conditional attack data for the animal, keyed by the reference integer for the condition under which the attack is used. This is used to determine which attacks are conditional attacks and to look up the details of the conditional attacks based on the reference integer for the condition under which they are used.
	 */
	const std::map<int, AnimalAttack>& conditionalAttacks() const { return conditional_attacks_; }


private:
	std::string name_{}; /**< In game name of the animal */
	std::string description_{}; /**< Description of the animal for flavour purposes */
	int base_hits_{}; /**< Base hits for the animal, used to determine how much damage it can take before it dies. This is not used for animals that are not meant to be attacked such as fish. */
	int base_movement_{}; /**< Base movement for the animal, used to determine how far it can move in a turn. */
	int defensive_bonus_{}; /**< Defensive bonus for the animal, used to determine how much damage it takes when attacked. */
	int frequency_factor_{}; /**< Frequency factor for the animal, used to determine how common it is to find in the appropriate environ. */
	int moving_manoeuvre_bonus_{}; /**< Bonus to Manoeuvre rolls when the animal is moving, used to determine how difficult it is to hit the animal when it is moving. */
	int carry_capacity_{}; /**< Carrying capacity of the animal, used to determine how much weight it can carry when used as a mount or pack animal. This is not used for animals that are not meant to be used as mounts or pack animals. */
	int riding_bonus_{}; /**< Riding bonus for the animal, used to determine the bonus to riding skill rolls when using the animal as a mount. This is not used for animals that are not meant to be used as mounts. */
	CreatureBonusXpType::Type bonus_xp_code_{}; /**< Bonus XP code for the animal, used to determine how much bonus XP is awarded for killing the animal. */
	CreatureConstitutionVarianceType::Type constitution_variance_type_{}; /**< Bonus constitution code for the animal, used to determine how much constitution variance a creature has. */
	LevelVarianceType::Type level_variance_type_{}; /**< Bonus level code for the animal, used to determine how much level variance a creature has. */
	int average_level_{}; /**< Average level of the animal, used to determine how much damage it can take before it dies. */
	std::optional<const TreasureCodeData*> treasure_code_{}; /**< Optional pointer to a TreasureCodeData object that represents the treasure that can be found on the animal or in it's lair when it is killed. This is optional as not all animals will have treasure. */
	CreatureSizeType::Type size_{}; /**< Size of the animal, used as a relative guide with Medium being approximately man-sized */
	ArmourType::Type armour_type_{}; /**< Armour type of the animal, used to determine how much damage it takes when attacked. */
	CreatureMovementSpeedType::Type movement_speed_{}; /**< Movement speed of the animal, used to determine how far it can move in a turn. */
	CreatureMovementSpeedType::Type attack_quickness_{}; /**< The speed that the creature attacks at, impacting initiative and DB */
	const CreaturePaceData* max_pace_{}; /**< Pointer to the maximum pace of the animal, used to determine how far it can move in a turn based on its movement speed. This is stored as a pointer to avoid having to copy the pace data for each animal and instead just reference the same data for all animals with the same movement speed. */	
	AnimalOutlookType::Type outlook_{}; /**< Outlook of the animal, used to determine how it behaves in combat and how it reacts to the world around it. */
	CriticalSizeTableType::Type critical_table_type_{}; /**< Critical table type for the animal, used to determine which critical table to use when the animal is hit with a critical hit. */
	std::set<CriticalModifierType::Type> critical_modifiers_{}; /**< Set of critical modifiers for the animal, used to determine which critical modifiers apply when the animal is hit with a critical hit. */
	std::pair<int, int> encounter_range_{}; /**< A pair containing the minimum and maximum number of animals typically enountered in a single encounter, used to determine how many animals are encountered when an encounter with the animal is generated. */
	std::pair<int, int> number_young_range_{}; /**< A pair containing the minimum and maximum number of young typically born in a single birth, used to determine how many young are born when a birth event is generated for the animal. */
	std::unique_ptr<Location> location_{}; /**< Location definition for the animal, used to determine where the animal can be found in the game world. This is used to match against specific locations to determine if the animal can be found there. */
	std::map<const NumberRange<int>*, AnimalAttack> attacks_{}; /**< Map of attack data for the animal, keyed by a pointer to a NumberRange<int> that represents the chance of the attack being used in a round. This is stored as a pointer to avoid having to copy the attack data for each animal and instead just reference the same data for all animals with the same attack chances. */
	std::map<int, AnimalAttack> group_attacks_{}; /**< Map of attack data for the animal, keyed by the number of attackers in the round. This is used to determine if the attack should be used based on the number of attackers in the round. For example, an attack may only be used if there are 2 or more attackers in the round. */
	std::vector<AnimalAttack> ranged_attacks_{}; /**< Vector of ranged attack data for the animal, used to determine which attacks are ranged attacks. This is used to determine if the animal can make ranged attacks and which attacks are ranged attacks. */
	std::map<int, AnimalAttack> conditional_attacks_{}; /**< Map of attack data for the animal where the attack is conditional. These may be called by other attacks the produce a non-tiny ciritcal result when they are resolved. */

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
