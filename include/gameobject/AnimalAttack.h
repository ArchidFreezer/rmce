#pragma once

#include<optional>

#include <AttackSizeType.h>
#include <CriticalTableType.h>
#include <CriticalType.h>
#include <Dice.h>
#include <DiseaseData.h>
#include <PoisonData.h>
#include <NumberRange.h>
#include <WeaponTypeData.h>
#include <table/AttackTable.h>
#include <table/SpecialAttackTable.h>

/**
 * @brief Represents the characteristics of an animal attack.
 * 
 * Many of the fields in this class are optional and it is expected that only one or two of the fields will be set for given attack. For example, a bear's claw attack would have an offensive bonus and a non-weapon attack table,
 * but no weapon type or special attack, while a giant spider's bite would have an offensive bonus, a non-weapon attack table and a poison effect, but no weapon type or special attack table.
 * 
 * Animals will typically have multiple attacks available to them, for example a bear would have a claw attack and a bot attack, and the creature data will specify the chance that each attack is used in a round.
 * The fields in this class are used to determine the results of an attack when it is used.
 */
class AnimalAttack {
public:

	/**
	 * @brief Set the percentage chance that this attack will be used
	 * 
	 * This is not the chance that the attack will be used in a round, it is the chance the results of the attack should be applied to the target. For example, starfish only have a 10% chance of being poisonous
	 * so even though that attack is always used, the results of the attack are only applied 10% of the time. This is typically used on seconday attacks that are applied if a primary aattack gains a critical, though not necessarily.
	 * 
	 * What this is NOT is the chance that the attack will be used in a round, which is defined in the animal data.
	 * 
	 * @param chance Percentage chance that this attack will be used
	 */
	void setChance(const NumberRange<int>* chance) { chance_ = chance; }

	/**
	 * @brief Get the percentage chance that this attack will be used
	 *
	 * This is not the chance that the attack will be used in a round, it is the chance the results of the attack should be applied to the target. For example, starfish only have a 10% chance of being poisonous
	 * so even though that attack is always used, the results of the attack are only applied 10% of the time. This is typically used on seconday attacks that are applied if a primary aattack gains a critical, though not necessarily.
	 *
	 * What this is NOT is the chance that the attack will be used in a round, which is defined in the animal data.
	 *
	 * @return NumberRange<int> Percentage chance that this attack will be used
	 */
	std::optional<const NumberRange<int>*> chance() const { return chance_; }

	/**
	 * @brief Set the reference integer for the conditional attack that this attack may trigger
	 * 
	 * A conditional attack is an attack that is only used if a certain condition is met. The condition is determined by the reference integer which can be used to look up the condition in a table or to determine the condition in some other way. For example, the reference integer could be used to determine if the attack is only used during a certain phase of combat or if it is only used if the animal has taken a certain amount of damage.
	 * 
	 * @param conditional_attack_ref Reference integer for the conditional attack that this attack may trigger, used to determine the condition under which the conditional attack is used and as the key for looking up the conditional attack in the conditional_attacks map in AnimalData.
	 */
	void setConditionalAttackRef(int conditional_attack_ref) { conditional_attack_ref_ = conditional_attack_ref; }

	/**
	 * @brief Get the reference integer for the conditional attack that this attack may trigger
	 * 
	 * A conditional attack is an attack that is only used if a certain condition is met. The condition is determined by the reference integer which can be used to look up the condition in a table or to determine the condition in some other way. For example, the reference integer could be used to determine if the attack is only used during a certain phase of combat or if it is only used if the animal has taken a certain amount of damage.
	 * 
	 * @return int Reference integer for the conditional attack that this attack may trigger, used to determine the condition under which the conditional attack is used and as the key for looking up the conditional attack in the conditional_attacks map in AnimalData.
	 */
	std::optional<int> conditionalAttackRef() const { return conditional_attack_ref_; }

	/**
	 * @brief Check if the attack should be used based on a d100 roll and the percentage chance of the attack being used
	 * 
	 * If there is not a percentage chance defined for the attack, it is assumed that the attack should always be used and this method will return `true`.
	 * 
	 * @param d100_roll The result of a d100 roll, used to determine if the attack should be used based on the percentage chance of the attack being used
	 * @return `true` if the attack should be used, `false` otherwise
	 */
	bool shouldUseAttack(int d100_roll) const { return (chance_.has_value() ? chance_.value()->matches(d100_roll) : true); }

	/**
	 * @brief Check if the attack should be used based on a randmly generated d100 roll and the percentage chance of the attack being used
	 * 
	 * If there is not a percentage chance defined for the attack, it is assumed that the attack should always be used and this method will return `true`.
	 * 
	 * @return `true` if the attack should be used, `false` otherwise
	 */
	bool shouldUseAttack() const { return shouldUseAttack(Dice(100).roll(false).result()); }

	/**
	 * @brief set the offensive bonus for the attack
	 * @param offensive_bonus Offensive bonus for the attack, used to determine how difficult it is to hit with the attack.
	 */
	void setOffensiveBonus(int offensive_bonus) { offensive_bonus_ = offensive_bonus; }
	/**
	 * @brief Get the offensive bonus for the attack
	 * @return int Offensive bonus for the attack, used to determine how difficult it is to hit with the attack.
	 */
	int offensiveBonus() const { return offensive_bonus_; }

	/**
	 * @brief Set the size of a non-weapon attack
	 * @param non_weapon_size Size of a non-weapon attack
	 */
	void setNonWeaponSize(AttackSizeType::Type non_weapon_size) { non_weapon_size_ = non_weapon_size; }

	/**
	 * @brief Get the size of a non-weapon attack
	 * @return AttackSizeType::Type Size of a non-weapon attack
	 */
	AttackSizeType::Type nonWeaponSize() const { return non_weapon_size_; }

	/**
	 * @brief Check if the attack has a non-weapon attack
	 * @return `true` if the attack has a non-weapon attack, `false` otherwise
	 */
	bool hasNonWeaponAttack() const { return non_weapon_table_ != nullptr; }

	/**
	 * @brief Set the SpecialAttackTable for non-weapon attacks
	 * @param non_weapon_table SpecialAttackTable for non-weapon attacks
	 */
	void setNonWeaponTable(const SpecialAttackTable& non_weapon_table) { non_weapon_table_ = &non_weapon_table; }

	/**
	 * @brief Get the SpecialAttackTable for non-weapon attacks
	 * @return SpecialAttackTable for non-weapon attacks
	 */
	const SpecialAttackTable* nonWeaponTable() const { return non_weapon_table_; }

	/**
	 * @brief Check if the attack has a weapon or elemental ball/bolt attack
	 * @return `true` if the attack has a weapon or elemental ball/bolt attack, `false` otherwise
	 */
	bool hasWeaponAttack() const { return weapon_table_ != nullptr; }

	/**
	 * @brief Set the AttackTable for weapon or elemental ball/bolt attacks, used to determine the attack result for weapon or elemental ball/bolt attacks
	 * @param weapon_table AttackTable for weapon or elemental ball/bolt attacks, used to determine the attack result for weapon or elemental ball/bolt attacks
	 */
	void setWeaponTable(const AttackTable& weapon_table) { weapon_table_ = &weapon_table; }

	/**
	 * @brief Get the AttackTable for weapon or elemental ball/bolt attacks, used to determine the attack result for weapon or elemental ball/bolt attacks
	 * @return AttackTable for weapon or elemental ball/bolt attacks, used to determine the attack result for weapon or elemental ball/bolt attacks
	 */
	const AttackTable* weaponTable() const { return weapon_table_; }

	/**
	 * @brief Set the minimum number of attackers that use this attack in a round, used to indicate that there should be multiple rolls to represent multiple animals such as ants
	 *
	 * If this is set to a value greater than 0 then it indicvates that this attack will be used in preference to any attacks with percentage chances.
	 *
	 * @param min_group_size Minimum number of attackers that use this attack in a round
	 */
	void setMinGroupSize(int min_group_size) { min_group_size_ = min_group_size; }

	/**
	 * @brief Get the minimum number of attackers that use this attack in a round, used to indicate that there should be multiple rolls to represent multiple animals such as ants
	 *
	 * If this is set to a value greater than 0 then it indicvates that this attack will be used in preference to any attacks with percentage chances.
	 *
	 * @return int Minimum number of attackers that use this attack in a round
	 */
	int minGroupSize() const { return min_group_size_; }

	/**
	 * @brief Check if the attack is used when multiple attackers are present
	 * 
	 * If the number of attackers is greter than minNumAttackers then this attack will be used each round until the number of attackers is reduced.
	 * 
	 * @return `true` if the attack is used when multiple attackers are present, `false` otherwise
	 */
	bool usesMultipleAttackers() const { return min_group_size_ > 1; }

	/**
	 * @brief Set the number of attacks that the creature makes with this attack in a round, used to indicate that there should be multiple rolls to represent multiple attacks such as a bear's two claws and one bite
	 * @param num_attacks Number of attacks that the creature makes with this attack in a round
	 */
	void setNumAttacks(int num_attacks) { num_attacks_ = num_attacks; }

	/**
	 * @brief Get the number of attacks that the creature makes with this attack in a round, used to indicate that there should be multiple rolls to represent multiple attacks such as a bear's two claws and one bite
	 * @return int Number of attacks that the creature makes with this attack in a round
	 */
	int numAttacks() const { return num_attacks_; }

	/**
	 * @brief Set the multiplier for concussion damage, used to determine how much concussion damage is dealt by the attack
	 * @param concussion_multiplier Multiplier for concussion damage, used to determine how much concussion damage is dealt by the attack
	 */
	void setConcussionMultiplier(int concussion_multiplier) { concussion_multiplier_ = concussion_multiplier; }

	/**
	 * @brief Get the multiplier for concussion damage, used to determine how much concussion damage is dealt by the attack
	 * @return int Multiplier for concussion damage, used to determine how much concussion damage is dealt by the attack
	 */
	int concussionMultiplier() const { return concussion_multiplier_; }

	/**
	 * @brief Set the poison applied by the attack, if any. If not set then the attack does not apply any poison.
	 * @param poison Poison applied by the attack, if any. If not set then the attack does not apply any poison.
	 */
	void setPoison(const PoisonData& poison) { poison_ = &poison; }

	/**
	 * @brief Get the poison applied by the attack, if any. If not set then the attack does not apply any poison.
	 * @return PoisonData* Poison applied by the attack, if any. If not set then the attack does not apply any poison.
	 */
	std::optional<const PoisonData*> poison() const { return poison_; }

	/**
	 * @brief Set the disease applied by the attack, if any. If not set then the attack does not apply any disease.
	 * @param disease Disease applied by the attack, if any. If not set then the attack does not apply any disease.
	 */
	void setDisease(const DiseaseData& disease) { disease_ = &disease; }

	/**
	 * @brief Get the disease applied by the attack, if any. If not set then the attack does not apply any disease.
	 * @return DiseaseData* Disease applied by the attack, if any. If not set then the attack does not apply any disease.
	 */
	std::optional<const DiseaseData*> disease() const { return disease_; }

	/**
	 * @brief Set the range of the attack, used to determine how far away the target can be for the attack to be used. This is used for attacks like a Spitting Cobra that spits venom to blind it's targets.
	 * 
	 * The range is in feet
	 * 
	 * @param range Range of the attack, used to determine how far away the target can be for the attack to be used. This is used for attacks like a Spitting Cobra that spits venom to blind it's targets.
	 */
	void setRange(int range) { range_ = range; }

	/**
	 * @brief Get the range of the attack, used to determine how far away the target can be for the attack to be used. This is used for attacks like a Spitting Cobra that spits venom to blind it's targets.
	 * 
	 * The range is in feet
	 * 
	 * @return int Range of the attack, used to determine how far away the target can be for the attack to be used. This is used for attacks like a Spitting Cobra that spits venom to blind it's targets.
	 */
	int range() const { return range_; }

	/**
	 * @brief Set special attacks that are not specifically defined above.
	 * @param special Special attacks that are not specifically defined above.
	 */
	void setSpecial(std::string special) { special_ = std::move(special); }

	/**
	 * @brief Get special attacks that are not specifically defined above.
	 * @return std::optional<std::string> Special attacks that are not specifically defined above.
	 */
	std::optional<const std::string> special() const { return special_; }

	/**
	 * @brief Set if the creature will use all of the other available standard attacks in a round
	 * @param use_all_attacks If true, the creature will use all of the other available standard attacks in a round
	 */
	void setUseAllAttacks(bool use_all_attacks) { use_all_attacks_ = use_all_attacks; }

	/**
	 * @brief Get if the creature will use all of the other available standard attacks in a round
	 * @return `true` if the creature will use all of the other available standard attacks in a round, `false` otherwise
	 */
	bool useAllAttacks() const { return use_all_attacks_; }

	/**
	 * @brief Set if this is a conditional attack that is used if another attack results in a non-tiny crit, the ID of the attack to use if the condition is met
	 * @param same_round_attack_id If this is set and the current attack results in a non-tiny crit the conditional attack with the given ID will also be used this round
	 */
	void setSameRoundAttackId(int same_round_attack_id) { same_round_attack_id_ = same_round_attack_id; }

	/**
	 * @brief Get if this is a conditional attack that is used if another attack results in a non-tiny crit, the ID of the attack to use if the condition is met
	 * @return int If this is set and the current attack results in a non-tiny crit the conditional attack with the given ID will also be used this round
	 */
	int sameRoundAttackId() const { return same_round_attack_id_; }

	/**
	 * @brief Set if this is a conditional attack that is used if another attack results in a non-tiny crit, the ID of the attack to use next round if the condition is met
	 * @param next_round_attack_id If this is set and the current attack results in a non-tiny crit the conditional attack with the given ID will be used next round
	 */
	void setNextRoundAttackId(int next_round_attack_id) { next_round_attack_id_ = next_round_attack_id; }

	/**
	 * @brief Get if this is a conditional attack that is used if another attack results in a non-tiny crit, the ID of the attack to use next round if the condition is met
	 * @return int If this is set and the current attack results in a non-tiny crit the conditional attack with the given ID will be used next round
	 */
	int nextRoundAttackId() const { return next_round_attack_id_; }

	/**
	 * @brief Set the type of critical that this attack always results in
	 * @param auto_critical_type Type of critical that this attack always results in
	 */
	void setAutoCriticalType(CriticalType::Type auto_critical_type) { auto_critical_type_ = auto_critical_type; }

	/**
	 * @brief Get the type of critical that this attack always results in
	 * @return CriticalType::Type Type of critical that this attack always results in
	 */
	std::optional <CriticalType::Type> autoCriticalType() const { return auto_critical_type_; }

	/**
	 * @brief Set the size of the auto critical, used for attacks that always result in a non-tiny crit to determine the size of the crit
	 * @param auto_critical_size Size of the auto critical, used for attacks that always result in a non-tiny crit to determine the size of the crit
	 */
	void setAutoCriticalSize(std::string auto_critical_size) { auto_critical_size_ = std::move(auto_critical_size); }

	/**
	 * @brief Get the size of the auto critical, used for attacks that always result in a non-tiny crit to determine the size of the crit
	 * @return std::string Size of the auto critical, used for attacks that always result in a non-tiny crit to determine the size of the crit
	 */
	std::optional <std::string> autoCriticalSize() const { return auto_critical_size_; }

private:
	std::optional <const NumberRange<int>*> chance_{}; /**< Percentage chance that this attack will be used */
	std::optional<int> conditional_attack_ref_{}; /**< If this is set, the attack is a conditional attack that is used if another attack results in a non-tiny crit, the ID of the attack to use if the condition is met */
	int offensive_bonus_{}; /**< Offensive bonus for the attack, used to determine how difficult it is to hit with the attack. */
	AttackSizeType::Type non_weapon_size_{}; /**< Size of a non-weapon attack. */
	const SpecialAttackTable* non_weapon_table_{}; /**< SpecialAttackTable for non-weapon attacks. */
	const AttackTable* weapon_table_{}; /**< AttackTable for weapon or elemental ball/bolt attacks that use standard attack tables, used to determine the attack result for special attacks. */
	int num_attacks_{ 1 }; /**< Number of attacks that the creature makes with this attack in a round, used to indicate that there should be multiple rolls to represent multiple attacks such as a bear's two claws and one bite. */
	bool use_all_attacks_{ false }; /**< If true, the creature will use all of the other available standard attacks in a round. */
	int min_group_size_{}; /**< If this number of creatures attack as a group, this attack may be used. For example, (10) indicates that if 10 of these creatures attack as a group they may use the given attack. */
	int concussion_multiplier_{ 1 }; /**< Multiplier for concussion damage, used to determine how much concussion damage is dealt by the attack. */
	std::optional<const PoisonData*> poison_{}; /**< Poison applied by the attack, if any. If not set then the attack does not apply any poison. */
	std::optional<const DiseaseData*> disease_{}; /**< Disease applied by the attack, if any. If not set then the attack does not apply any disease. */
	int range_{}; /**< Range of the attack, used to determine how far away the target can be for the attack to be used. This is used for attacks like a Spitting Cobra that spits venom to blind it's targets. */
	std::optional<std::string> special_{}; /**< Special attacks that are not specifically defined above. */
	int same_round_attack_id_{}; /**< If this is set and the current attack results in a non-tiny crit the conditional attack with the given ID will also be used this round */
	int next_round_attack_id_{}; /**< If this is set and the current attack results in a non-tiny crit the conditional attack with the given ID will be used next round */
	std::optional <CriticalType::Type> auto_critical_type_{}; /**< Type of critical that this attack always results in. */
	std::optional <std::string> auto_critical_size_{}; /**< Size of the auto critical, used for attacks that always result in a non-tiny crit to determine the size of the crit. */
};