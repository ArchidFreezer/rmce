#pragma once
#include <map>
#include <ranges>

#include <table/AttackTable.h>
#include <BookData.h>
#include <CriticalType.h>
#include <GameRuleData.h>
#include <SkillData.h>

/**
 * @class WeaponTypeData 
 * @brief Class containing the data about different types of weapons that may be found in game
 * 
 * The data includes the dimentions of a typical weapon, the attack table it uses and other data such as criticals and
 * ranges if applicable.
 */
class WeaponTypeData : public GameRuleData {
public:

	WeaponTypeData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

	/**
	 * @brief Constructor
	 * @param id Unique identifier for the book
	 */
	WeaponTypeData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the weapon type
	 * @param name Weapon type name
	 */
	void setName(std::string_view name) { name_ = name; }
	/**
	 * @brief Get the name of the weapon type
	 * @return Weapon type name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the notes of the weapon type
	 * @param notes Weapon type notes
	 */
	void setNotes(std::string_view notes) { notes_ = notes; }
	/**
	 * @brief Get the notes of the weapon type
	 * @return Weapon type notes as a string reference
	 */
	const std::string& notes() const { return notes_; }

	/**
	 * @brief Set the skill the waepon uses
	 * @param skill SkillData reference to a skill
	 */
	void setSkill(const SkillData& skill) { skill_ = &skill; }

	/**
	 * @brief Get the skill the weapon uses
	 * @return SkillData reference to the skill the weapon uses
	 */
	const SkillData& skill() const { return *skill_; }

	/**
	 * @brief Set the book that the weapon type is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_ = &book; }

	/**
	 * @brief Get the book that the weapon type is defined in
	 * @return Reference to the BookData containing the weapon type definition
	 */
	const BookData& book() const { return *book_; }

	/**
	 * @brief Set the attack table the weapon type uses
	 * @param table AttackTable reference used
	 */
	void setAttackTable(const AttackTable& table) { table_ = &table; }

	/**
	 * @brief Set the attack table the weapon type uses
	 * @return AttackTable reference used
	 */
	const AttackTable& attackTable() const { return *table_; }

	/**
	 * @brief Set the fumble range for the weapon
	 * 
	 * If an unmodified attack roll falls within the range shown, the attacker fumbles his weapon and must then roll on the
	 * appropriate Fumble Table.
	 * 
	 * @param fumble max unmodified roll considered a fumble
	 */
	void setFumble(int fumble) { fumble_ = fumble; }

	/**
	 * @brief Set the fumble range for the weapon
	 *
	 * If an unmodified attack roll falls within the range shown, the attacker fumbles his weapon and must then roll on the
	 * appropriate Fumble Table.
	 *
	 * @return max unmodified roll considered a fumble
	 */
	int fumble() const { return fumble_; }

	/**
	 * @brief Set the numbers, when rolled doubled, trigger a breakage check
	 * 
	 * The number range is from 1 to the number set, i.e. if \a breakage is 3 then double 1, double 2 and double 3 trigger a
	 * breakage check
	 * 
	 * @param breakage int breakage number
	 */
	void setBreakage(int breakage) { breakage_ = breakage; }

	/**
	 * @brief Get the numbers, when rolled doubled, trigger a breakage check
	 * 
	 * The number range is from 1 to the number set, i.e. if \a breakage is 3 then double 1, double 2 and double 3 trigger a
	 * breakage check
	 * 
	 * @return int breakage top number
	 */
	int breakage() const { return breakage_; }

	/**
	 * @brief Set the standard range of strengths for a normal weapon of this type. 
	 * 
	 * If a breakage roll is required this value is used to determine if the weapon breaks, with higher numbers being better.
	 * The number here is for standard weapons so unique weapons may have a different value
	 * 
	 * @param max_strength int maximum strength of a regular weapon of this type
	 */
	void setMaxStrength(int max_strength) { max_strength_ = max_strength; }

	/**
	 * @brief Get the standard range of strengths for a normal weapon of this type.
	 *
	 * If a breakage roll is required this value is used to determine if the weapon breaks, with higher numbers being better.
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @return int maximum strength of a regular weapon of this type
	 */
	int maxStrength() const { return max_strength_; }

	/**
	 * @brief Set the standard range of strengths for a normal weapon of this type.
	 *
	 * If a breakage roll is required this value is used to determine if the weapon breaks, with higher numbers being better.
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @param min_strength int minimum strength of a regular weapon of this type
	 */
	void setMinStrength(int min_strength) { min_strength_ = min_strength; }

	/**
	 * @brief Get the standard range of strengths for a normal weapon of this type.
	 *
	 * If a breakage roll is required this value is used to determine if the weapon breaks, with higher numbers being better.
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @return int minimum strength of a regular weapon of this type
	 */
	int minStrength() const { return min_strength_; }

	/**
	 * @brief Set the standard maximum length of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @param max_length int maximum length of a regular weapon of this type
	 */
	void setMaxLength(int max_length) { max_length_ = max_length; }

	/**
	 * @brief Get the standard maximum length of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @return int maximum length of a regular weapon of this type
	 */
	int maxLength() const { return max_length_; }

	/**
	 * @brief Set the standard minimum length of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @param min_length int minimum length of a regular weapon of this type
	 */
	void setMinLength(int min_length) { min_length_ = min_length; }

	/**
	 * @brief Get the standard minimum length of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @return int minimum length of a regular weapon of this type
	 */
	int minLength() const { return min_length_; }

	/**
	 * @brief Set the standard maximum weight of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @param max_weight int maximum weight of a regular weapon of this type
	 */
	void setMaxWeight(int max_weight) { max_weight_ = max_weight; }

	/**
	 * @brief Get the standard maximum weight of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @return int maximum weight of a regular weapon of this type
	 */
	int maxWeight() const { return max_weight_; }

	/**
	 * @brief Set the standard minimum weight of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @param min_weight int minimum weight of a regular weapon of this type
	 */
	void setMinWeight(int min_weight) { min_weight_ = min_weight; }

	/**
	 * @brief Get the standard minimum weight of a normal weapon of this type.
	 *
	 * The number here is for standard weapons so unique weapons may have a different value
	 *
	 * @return int minimum weight of a regular weapon of this type
	 */
	int minWeight() const { return min_weight_; }

	/**
	 * @brief Set if the weapon normally has a wooden haft
	 * 
	 * During breakage checks a weapon with a wooden haft that hits an 'edged' weapon has its strength reduced by 15
	 * 
	 * @param wooden_haft `true` if the weapon typically has a wooden haft; `false` otherwise
	 */
	void setWoodenHaft(bool wooden_haft) { wooden_haft_ = wooden_haft; }

	/**
	 * @brief Get if the weapon typically has a wooden haft
	 * During breakage checks a weapon with a wooden haft that hits an 'edged' weapon has its strength reduced by 15
	 *
	 * @return `true` if the weapon typically has a wooden haft
	 * @return `false` if the weapon does not typically have a wooden haft
	 */
	bool woodenHaft() const { return wooden_haft_; }

	/**
	 * @brief Add a critical type that these weapons may inflict along with any associated bonus
	 * 
	 * Weapons not only have a standard critical type that they inflict, but may also have additional types depending on how
	 * the weapon is used. These alternative criticals typically have a negative modifier to the crit roll to indicate that
	 * the weapon is being used in an unusual way. An example would be a rapier which is intended to be used to puncture an
	 * opponent using the tip, however it would be possibel to try and use the weapon for slashing, but that would be less
	 * effective.
	 * 
	 * @param type CriticalType::Type critical being applied
	 * @param modifier int modifier to the critical table roll
	 */
	void addCriticalModifier(CriticalType::Type type, int modifier) { criticals_.insert(std::make_pair(type, modifier));	}

	/**
	 * @brief Get a container with the critical types that the weapon can inflict
	 * 
	 * Weapons may inflict multiple different types of criticals depending on how they are used and this method returns those
	 * available to the weapon. Each weapon has a standard critical that is uses that should be applied unless the player
	 * actively states they are using the weapon in a specific way, available via the critical() method.
	 * 
	 * @return std::vector<CriticalType::Type> containing the available criticals
	 * @see critical()
	 */
	const std::vector<CriticalType::Type> criticals() const {
		auto keys = std::views::keys(criticals_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Get the primary critical type weapons of this type inflict
	 * 
	 * Weapons have a primary type of critical that they inflict, they may be capable of inflicting other types but those
	 * will cause less damage on average, reflected by a negative modifier applied to the critical roll.
	 * 
	 * @return CriticalType::Type primary critical type
	 */
	const CriticalType::Type primaryCritical() const {
		CriticalType::Type primary{};
		int max{-500};
		for (auto crit : std::views::keys(criticals_)) {
			if (criticals_.at(crit) > max) primary = crit;
		}
		return primary;
	}

	/**
	 * @brief Get the modifier associated with the critical type
	 * 
	 * @param critical CriticalType::Type type of critical to get the modifier for
	 * @return int modifier
	 */
	int criticalModifier(CriticalType::Type critical) const { return criticals_.at(critical); }

	/**
	 * @brief Add the weap[on attack modifier for a range of distances
	 * 
	 * The distances are measured in feet
	 * 
	 * @param range NumberRange containing the minimum and maximum range for this modifier
	 * @param modifier int attack modifier
	 */
	void addRange(const NumberRange<int>& range, int modifier) { ranges_.insert(std::make_pair(&range, modifier)); }

	/**
	 * @brief Get the attack range modifier for a range in feet
	 * @param range int range to get the modifier for
	 * @return int attack modifier
	 * @throws InvalidWeaponRangeException if the weapon may not be used at the range
	 */
	int rangeModifier(int range) const {
		for (auto& matcher : std::views::keys(ranges_)) {
			if (matcher->matches(range)) return ranges_.at(matcher);
		}
		throw InvalidWeaponRangeException(name_ + " cannot be used at range " + std::to_string(range));
	}

	/**
 * @brief Get a container with the ranges that the weapon can attack from
 *
 * @return std::vector<NumberRange<int>*> containing the available ranges
 */
	const std::vector<const NumberRange<int>*> ranges() const {
		auto keys = std::views::keys(ranges_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Thrown when attempting to retrieve an attack modifier for a range that the weapon cannot be used at
	 */
	class InvalidWeaponRangeException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidWeaponRangeException(const std::string& error) : std::runtime_error{ error }{}
	};

private:
	std::string name_{};
	std::string notes_{};
	const SkillData* skill_{};
	const BookData* book_{};
	const AttackTable* table_{};
	int fumble_{};
	int breakage_{};
	float min_strength_{};
	float max_strength_{};
	float min_length_{};
	float max_length_{};
	float min_weight_{};
	float max_weight_{};
	bool wooden_haft_{};
	std::map<CriticalType::Type, int> criticals_{};
	std::map<const NumberRange<int>*, int> ranges_;
};