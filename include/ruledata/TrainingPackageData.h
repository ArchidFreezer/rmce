#pragma once

#include <map>
#include <set>
#include <stdexcept>
#include <string_view>
#include <Dice.h>
#include <GameRuleData.h>
#include <RaceData.h>

/**
 * @class TrainingPackageData
 * @brief A training package is a group of benefits (and sometimes disadvantages) that can be developed using development
 * points
 * 
 * The skills and abilities gained through a package represent long periods of exposure or training (sometimes years). If
 * these packages are developed after apprenticeship development, the character should be required to spend time in
 * training (the time will vary from package to package).
 * 
 * There are two different types of training packages: vocational and lifestyle. Most of the time, a character may have
 * only one lifestyle training package (GM discretion). However, he can have as many vocational training packages as he can
 * afford to purchase.
 * 
 * <em>Lifestyle</em> - These training packages represent groups of skills and abilities that would be gained by living a
 * particular lifestyle. Lifestyle packages should be hard to acquire, and take long periods of time when they are allowed.
 * 
 * <em>Vocational</em> - These training packages represent specific occupations in which a character may become proficient. He
 * will gain groups of skills and abilities that would necessarily be developed if the character were in the occupation.
 */
class TrainingPackageData : public GameRuleData {
public:
	/**
	 * @brief Thrown when there is an attempt to set an invalid skill rank
	 */
	class InvalidSkillRank : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidSkillRank(const std::string& error) : std::runtime_error{ error } {}
	};

	/**
	 * @brief Delete the default constructor to ensure we initialise the base class
	 */
	TrainingPackageData() = delete;

	/**
	 * @brief Basic constructor to initialise the base class
	 * @param id Unique identifier of the training package
	 */
	TrainingPackageData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the training package
	 * @param name Training package name
	 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the training package
	 * @return Training package name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of what defines the training package
	 * @param description std::string_view training package description
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Get the description of what the defines training package
	 * @return std::string reference of the description
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set additional notes about the training package that may be relevant for a GM when running a character with the training package
	 * @param notes std::string_view additional notes about the training package
	 */
	void setNotes(std::string_view notes) { notes_ = notes; }

	/**
	 * @brief Get additional notes about the training package that may be relevant for a GM when running a character with the training package
	 * @return std::string reference of the additional notes about the training package
	 */
	const std::string& notes() const { return notes_; }

	/**
	 * @brief Set the book that the training package is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_ = &book; }

	/**
	 * @brief Get the book that the training package is defined in
	 *
	 * @return BookData containing the training package definition
	 */
	const BookData& book() const { return *book_; }

	/**
	 * @brief  Add a race to those who can take the package if it is restricted
	 * 
	 * Some packages are restricted to a set of one or more races. The use of this method indicates that the package is one
	 * and that only races who have been added may take the package.
	 * 
	 * @param race RaceData to add
	 */
	void addRace(const RaceData& race) { races_.insert(&race); }

	/**
	 * @brief Set of races that may take the package
	 * 
	 * The set will be empty for packages that have no restrictions
	 * 
	 * @return Set of races that may take the package
	 */
	const std::set<const RaceData*> races() const { return  races_; }

	/**
	 * @brief Check whether a race can take the training package
	 * 
	 * @param race RaceData to check
	 * @return `true` if the package is available to the race
	 * @return `false` if the package is not available to the race
	 */
	bool available(const RaceData& race) {
		if (!races_.empty()) {
			for (const auto& key : races_) {
				if (key->id() == race.id()) return true;
			}
			return false;
		}
		return true;
	}

	/**
	 * @brief Add a qualifier that, if met, reduces the cost of the training package
	 * 
	 * All conditions of a qualifier must be met in order to receive the reduced cost
	 * 
	 * @param qualifier Description of the qualifier
	 * @param reduction Reduction in package cost
	 */
	void addQualifier(std::string_view qualifier, int reduction) { qualifiers_.emplace(qualifier, reduction); }

	/**
	 * @brief Get a collection of qualifiers that, if met, reduce the cost of the training package
	 * 
	 * Only a single discout can apply so if a character meets the requireemnts of more than one qualifier the one with the
	 * biggest discount should be applied.
	 * 
	 * @return Map of qualifier text and associated cost reduction
	 */
	const std::map<std::string, int> qualifiers() const { return qualifiers_; }

	/**
	 * @brief Set whether non spell users may take the package
	 * 
	 * Some training packages are only for semi, hybrid and pure spell users
	 * @param caster_only `true` if the package is only for spell users; `false` otherwise
	 */
	void setSpellUserOnly(bool caster_only) { caster_only_ = caster_only; }

	/**
	 * @brief Get whether non spell users may take the package
	 * 
	 * @return `true` if only semi, hybrid and pure spell users may take the package
	 * @return `false` if the package is available to any profession
	 */
	bool spellUserOnly() const { return caster_only_; }

	/**
	 * @brief Set whether the package is lifestyle based
	 * 
	 * @param lifestyle `true` if the package is a lfestyle package; `false` if the package is vocational
	 */
	void setLifestyle(bool lifestyle) { lifestyle_ = lifestyle; }

	/**
	 * @brief Get whether the package is lifestyle based
	 * @return `true` if the package is lifestyle based
	 * @return `false` if the package is not lifestyle based
	 */
	bool lifestyle() const { return lifestyle_; }

	/**
	 * @brief Set the number of months to acquire the package
	 * 
	 * This is the amount of time the character needs to train before gaining the benefits of the package. This time should
	 * be modified by a percentage equal to triple the character’s SD stat bonus (expressed as a percentage).
	 * 
	 * Example: If a character has a +5 SD stat bonus, he would gain the benefits of a training package 15% earlier than
	 * someone with a +0 SD bonus (i.e., 85% of the normal time). However, if the character has a -5 SD stat bonus, he
	 * would take 15% longer to gain the benefit (i.e., 115% of the normal time). 
	 * 
	 * @param months Number of months to acquire the package
	 */
	void setTimeToAcquire(int months) { time_to_acquire_ = months; }

	/**
	 * @brief Set the number of months to acquire the package
	 *
	 * This is the amount of time the character needs to train before gaining the benefits of the package. This time should
	 * be modified by a percentage equal to triple the character’s SD stat bonus (expressed as a percentage).
	 *
	 * Example: If a character has a +5 SD stat bonus, he would gain the benefits of a training package 15% earlier than
	 * someone with a +0 SD bonus (i.e., 85% of the normal time). However, if the character has a -5 SD stat bonus, he
	 * would take 15% longer to gain the benefit (i.e., 115% of the normal time).
	 *
	 * @return Number of months to acquire the package
	 */
	int timeToAcquire() const { return time_to_acquire_; }

	/**
	 * @brief The change in starting money for a character.
	 * 
	 * If the character takes this package as part of their apprenticeship it may modify the ammount of starting money they
	 * receive. The actual ammount is based on a random roll with this value setting the maximum or minimum change. The value
	 * returned is based on an open-ended high dice roll with open ended rolls being generated if the roll result is the
	 * maximum/minimum value.
	 * 
	 * Examples:
	 * + A value of 10 will return a number between 1 and 10, open-ended on 10 which may increase the number
	 * + A value of -8 will return a number between -1 and -8, open-ended on 8, which may reduce the number
	 *
	 * @param max Maximum difference in starting money
	 */
	void setStaringMoneyChangeMax(int max) { starting_money_max_ = max; }

	/**
	 * @brief Get the change in starting money if the package is taken during apprenticeship
	 * @return value of change in money
	 */
	int startingMoneyChange() const { 
		int abs_value = Dice(std::abs(starting_money_max_), 1, 0).roll().result();
		return (starting_money_max_ > 0 ? abs_value : abs_value * -1);
	}

	/**
	 * @brief Add a stat that receives a stat gain roll as part of the package
	 * @param stat StatType::Type stat to gain roll
	 */
	void addStatGain(StatType::Type stat) { stat_gains_.emplace(stat); }

	/**
	 * @brief Get set of stats that receive a stat gain roll
	 * @return Set of StatType::Type stats that recieve a stat gain roll
	 */
	const std::set< StatType::Type> statGains() const { return stat_gains_; }

	/**
	 * @brief Set the number of ranks for a skill the package provides
	 * @param skill SubcategoriedSkillData to add the ranks to
	 * @param ranks int number of ranks
	 */
	void addSkillRank(SubcategoriedSkillData skill, int ranks) {
		if (isRankSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillRank("There is already a rank set for skill " + skill.id());
		skill_ranks_.emplace(std::move(skill), ranks);
	}

	/**
	 * @brief Get the number of ranks for a skill the package provides
	 * @param skill SubcategoriedSkillData to get the ranks for
	 * @return number of ranks
	 */
	int skillRank(const SubcategoriedSkillData& skill) const {
		for (auto& key : skill_ranks_) {
			if (key.first.id() == skill.id()) return key.second;
		}
		return 0;
	}

	/**
	 * @brief Get the number of ranks for a skill the package provides
	 * @param skill SkillData to get the ranks for
	 * @param subcategory optional subcategory of @a skill
	 * @return number of ranks
	 */
	int skillRank(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		return skillRank(SubcategoriedSkillData(skill, subcategory));
	}

	/**
	 * @brief Gets a container with the skills that the package provides
	 * @return std::set of SkillData with ranks
	 */
	const std::set<SubcategoriedSkillData> skillsWithRanks() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& key : skill_ranks_) {
			const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if the package provides skill ranks for the skill
	 * @param skill SkillData to check
	 * @param subcategory optional subcategory of @a skill
	 * @return `true` if the package provides ranks
	 * @return `false` if the package does not provide ranks
	 */
	bool isRankSkill(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& key : std::views::keys(skill_ranks_)) {
			if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
		}
		return false;
	}

private:
	std::string name_{}; /**< Name of the training package */
	std::string description_{}; /**< General description of the training package */
	std::string notes_{}; /**< Additional notes about the training package that may be relevant for a GM when running a character with the training package */
	const BookData* book_{}; /**< Book that the training package is described in */
	std::set<const RaceData*> races_{}; /**< Races allowed to take the package if it is restricted, empty for all races */
	std::map<std::string, int> qualifiers_{}; /**<  Requirements, if met, that reduce the cost of the package */
	bool caster_only_{}; /**< Whether non spell user training packages may take the package */
	bool lifestyle_{}; /**< Whether the package is a lifestyle package */
	int time_to_acquire_{}; /**< Time, in months, to acquire the package */
	int starting_money_max_{}; /**< Change in starting money a character gains during apprenticeship */
	std::set<StatType::Type> stat_gains_{}; /**< Stats that receive a stat gain roll */
	std::map<SubcategoriedSkillData, int> skill_ranks_{}; /** Number of skill ranks gained */

};
