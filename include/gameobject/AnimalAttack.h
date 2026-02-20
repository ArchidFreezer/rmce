#pragma once

#include<optional>

#include <AttackSizeType.h>
#include <CriticalTableType.h>
#include <CriticalType.h>
#include <Dice.h>
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
	 * @brief Check if the attack has a weapon attack
	 * @return `true` if the attack has a weapon attack, `false` otherwise
	 */
	bool hasWeaponAttack() const { return weapon_type_ != nullptr; }

	/**
	 * @brief Set the weapon type for weapon attacks, if there is one. This is used to determine which weapon table to use for the attack
	 * @param weapon_type Weapon type for any weapon attacks, used to determine which weapon table to use for the attack
	 */
	void setWeaponType(const WeaponTypeData& weapon_type) { weapon_type_ = &weapon_type; }

	/**
	 * @brief Get the weapon type for weapon attacks, used to determine which weapon table to use for the attack
	 * @return WeaponTypeData for weapon attacks, used to determine which weapon table to use for the attack
	 */
	const WeaponTypeData* weaponType() const { return weapon_type_; }

	/**
	 * @brief Check if the attack has a special attack
	 * @return `true` if the attack has a special attack, `false` otherwise
	 */
	bool hasSpecialAttack() const { return special_table_ != nullptr; }

	/**
	 * @brief Set the AttackTable for special attacks, used to determine the attack result for special attacks
	 * @param special_table AttackTable for special attacks, used to determine the attack result for special attacks
	 */
	void setSpecialTable(const AttackTable& special_table) { special_table_ = &special_table; }

	/**
	 * @brief Get the AttackTable for special attacks, used to determine the attack result for special attacks
	 * @return AttackTable for special attacks, used to determine the attack result for special attacks
	 */
	const AttackTable* specialTable() const { return special_table_; }

	/**
	 * @brief Add an attack that the creature may use in a round
	 * @param multi_attack Pointer to an attack that the creature may use in a round
	 */
	void addMultiAttack(const AnimalAttack& multi_attack) { multi_attacks_.insert(&multi_attack); }

	/**
	 * @brief Set the set of attacks that the creature may use in a round
	 * @param multi_attacks Set of pointers to attacks that the creature may use in a round
	 */
	void setMultiAttacks(const std::set<const AnimalAttack*>& multi_attacks) { multi_attacks_ = multi_attacks; }

	/**
	 * @brief Get the set of attacks that the creature may use in a round
	 * @return Set of pointers to attacks that the creature may use in a round
	 */
	const std::set<const AnimalAttack*>& multiAttacks() const { return multi_attacks_; }

	/**
	 * @brief Check if the creature has a set of attacks that it may use in a round
	 * @return `true` if the creature has any attacks that it may use in a round, `false` otherwise
	 */
	bool hasMultiAttacks() const { return !multi_attacks_.empty(); }

	/**
	 * @brief Set the minimum number of attackers that use this attack in a round, used to indicate that there should be multiple rolls to represent multiple animals such as ants
	 *
	 * If this is set to a value greater than 0 then it indicvates that this attack will be used in preference to any attacks with percentage chances.
	 *
	 * @param min_num_attackers Minimum number of attackers that use this attack in a round
	 */
	void setMinNumAttackers(int min_num_attackers) { min_num_attackers_ = min_num_attackers; }

	/**
	 * @brief Get the minimum number of attackers that use this attack in a round, used to indicate that there should be multiple rolls to represent multiple animals such as ants
	 *
	 * If this is set to a value greater than 0 then it indicvates that this attack will be used in preference to any attacks with percentage chances.
	 *
	 * @return int Minimum number of attackers that use this attack in a round
	 */
	int minNumAttackers() const { return min_num_attackers_; }

	/**
	 * @brief Check if the attack is used when multiple attackers are present
	 * 
	 * If the number of attackers is greter than minNumAttackers then this attack will be used each round until the number of attackers is reduced.
	 * 
	 * @return `true` if the attack is used when multiple attackers are present, `false` otherwise
	 */
	bool usesMultipleAttackers() const { return min_num_attackers_ > 0; }

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
	 * @brief Set the critical table to use for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 * @param critical_table Critical table to use for the attack, used to determine which critical table to use for the attack
	 */
	void setCriticalTable(CriticalTableType::Type critical_table) { critical_table_ = critical_table; }

	/**
	 * @brief Get the critical table to use for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 * @return CriticalTableType::Type Critical table to use for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 */
	std::optional<CriticalTableType::Type> criticalTable() const { return critical_table_; }

	/**
	 * @brief Set the critical type for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 * @param critical_type Critical type for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 */
	void setCriticalType(CriticalType::Type critical_type) { critical_type_ = critical_type; }

	/**
	 * @brief Get the critical type for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 * @return CriticalType::Type Critical type for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used.
	 */
	std::optional<CriticalType::Type> criticalType() const { return critical_type_; }

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
	 * @brief Set the number of hits per round the target takes following the attack
	 * @param hits_per_rounds Number of hits per round the target takes following the attack
	 */
	void setHitsPerRounds(int hits_per_rounds) { hits_per_rounds_ = hits_per_rounds; }

	/**
	 * @brief Get the number of hits per round the target takes following the attack
	 * @return int Number of hits per round the target takes following the attack
	 */
	int hitsPerRounds() const { return hits_per_rounds_; }

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


private:
	std::optional <const NumberRange<int>*> chance_{}; /**< Percentage chance that this attack will be used */
	int offensive_bonus_{}; /**< Offensive bonus for the attack, used to determine how difficult it is to hit with the attack. */
	AttackSizeType::Type non_weapon_size_{}; /**< Size of a non-weapon attack. */
	const SpecialAttackTable* non_weapon_table_{}; /**< SpecialAttackTable for non-weapon attacks. */
	const WeaponTypeData* weapon_type_{}; /**< Weapon type for weapon attacks, used to determine which weapon table to use for the attack. */
	const AttackTable* special_table_{}; /**< AttackTable for special attacks, used to determine the attack result for special attacks. */
	std::set<const AnimalAttack*> multi_attacks_{}; /**< Set of attacks that the creature may use in a round */
	int min_num_attackers_{}; /**< If this number of creatures attack as a group, this attack may be used. For example, (10) indicates that if 10 of these creatures attack as a group they may use the given attack. */
	int num_attacks_{ 1 }; /**< Number of attacks that the creature makes with this attack in a round, used to indicate that there should be multiple rolls to represent multiple attacks such as a bear's two claws and one bite. */
	int concussion_multiplier_{ 1 }; /**< Multiplier for concussion damage, used to determine how much concussion damage is dealt by the attack. */
	std::optional<CriticalTableType::Type> critical_table_{}; /**< Critical table to use for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used. */
	std::optional<CriticalType::Type> critical_type_{}; /**< Critical type for the attack, used to determine which critical table to use for the attack. If not set then the default critical table will be used. */
	std::optional<const PoisonData*> poison_{}; /**< Poison applied by the attack, if any. If not set then the attack does not apply any poison. */
	int hits_per_rounds_{}; /**< Number of hits per round the target takes following the attack. */
	int range_{}; /**< Range of the attack, used to determine how far away the target can be for the attack to be used. This is used for attacks like a Spitting Cobra that spits venom to blind it's targets. */
	std::optional<std::string> special_{}; /**< Special attacks that are not specifically defined above. */
};