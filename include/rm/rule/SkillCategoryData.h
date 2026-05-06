#pragma once

#include <stdexcept>
#include <string_view>
#include <vector>

#include <CharacterTraits.h>
#include <GameRuleData.h>
#include <SkillGroupData.h>
#include <StatType.h>
#include <SkillProgressionTypeData.h>

using namespace rm::rule::enums;

namespace rm::rule {

/**
 * @brief Thrown when there is an attempt to set either the skill progression or category progression in an invalad combination
 *
 * The category progression can only be kStandard or kNone and only kStandard if the skill progression is also kStandard
 */
class InvalidSkillProgression : public std::runtime_error {
public:
	/**
	 * @brief Exception constructor
	 *
	 * std::runtime_error will handle the string
	 *
	 * @param error String to display for the error
	 */
	InvalidSkillProgression(const std::string& error) : std::runtime_error{error} {
	}
};

/**
 * @class  SkillCategoryData
 * @brief Class to represent a skill category
 *
 * Each skill is grouped with other similar skills in a specific skill category. Each category contains skills that utilize similar physical and mental capabilities (i.e., stats) and similar basic, general abilities. In addition to
 * developing individual skills, a character may develop and improve all of the skills in a given skill category by increasing its skill category bonus
 */
class SkillCategoryData : public GameRuleData {
public:
	SkillCategoryData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

	/**
	 * @brief Constructor
	 * @param id Unique identifier of the skill category
	 */
	SkillCategoryData(std::string_view id) : GameRuleData(id) {
	}

	inline static std::string prefix_{"SKILLCATEGORY"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return SkillCategoryData::prefix_;
	}

	/**
	 * @brief Set the name of the skill category
	 * @param name skill category name
	 */
	void setName(std::string_view name) {
		name_ = name;
	}

	/**
	 * @brief Get the name of the category as seen in-game
	 * @return Name as a string reference
	 */
	const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Gets the full name of the skill category which includes the skill group
	 * @return Name including the skill group
	 */
	const std::string fullName() const {
		return group_->name() + " - " + name_;
	}

	/**
	 * @brief Set the skill group of the category
	 * @param group SkillGroupType::Type category group
	 */
	void setGroup(const SkillGroupData& group) {
		group_ = &group;
	}

	/**
	 * @brief Get the group the category belongs to
	 * @return SkillGroupType::Type categories skill group
	 */
	const SkillGroupData& group() const {
		return *group_;
	}

	/**
	 * @brief Add stat that applies stat bonus to skills in the category
	 *
	 * There are 3 stats associated with each skill category and duplicates are allowed
	 *
	 * @param stat whose bonus should be applied to skills category
	 * @returns Number of stats associated withthe category after the operation
	 * @throws TooManyStatsException if attempting to add a stat when 3 already have been applied
	 * @throws UsingCharacterRealmStatsException if attempting to add a stat when using character realm stats
	 * @see setUseRealmStats()
	 */
	int addStat(StatType::Type stat);

	/**
	 * @brief Set the stats that apply stat bonus to skills in the category
	 * @param stats vector of stats whose bonus should be applied to skills category
	 */
	void setStats(std::vector<StatType::Type> stats) {
		stats_ = std::move(stats);
	}

	/**
	 * @brief Gets the number of stats currently associated with the category
	 *
	 * Up to 3 stats bonuses may be applied to skills in a skill category. This function returns the current number applied
	 * @return Number of stats applied
	 */
	const int numberOfStats() const {
		return stats_.size();
	}

	/**
	 * @brief Remove a stats from those currently associated with the category
	 *
	 * Remove a stat from the list of those whose bonus is currently applied to skill in the category.
	 * If the stat is not in the currently applied then the function does nothing
	 *
	 * @param stat Stat to remove
	 * @return `true` if the stat was removed
	 * @return `false` if the stat was not found or not removed
	 */
	bool removeStat(StatType::Type stat);

	/**
	 * @brief Empties the list of stats associated with the category
	 */
	void clearStats() {
		stats_.clear();
	}

	/**
	 * @brief Gets the stats that that provide a bonus to the category and skills within it
	 *
	 * A stat may appear more than once in the list
	 *
	 * @return std::vector of stats
	 */
	const std::vector<StatType::Type>& stats() const {
		return stats_;
	}

	/**
	 * @brief Set whether to use the characters realm stats for determining the bonus
	 *
	 * Some skill categories such as those from the Spell skill group use the characters realm stats for determining the
	 * bonus provided. If this flag is set then the stats() method should not be used.
	 *
	 * If this flag is set then any stats previously set on the category using addStat() will be removed.
	 *
	 * @param use_realm_stats bool Whether to use character realm stats
	 */
	void setUseRealmStats(bool use_realm_stats);

	/**
	 * @brief Gets whether to use the characters realm stats for determining the bonus
	 *
	 * Some skill categories such as those from the Spell skill group use the characters realm stats for determining the
	 * bonus provided. If this flag is set then the stats() method should not be used.
	 *
	 * @return `true` if the character realm stats should be used
	 * @return `false` if the stats from stats() should be used
	 */
	bool useRealmStats() const {
		return use_realm_stats_;
	}

	/**
	 * @brief Sets the skill progressions for both skills in the category and the catehory itself
	 *
	 * Each skill in the category has a skill progression that defines the bonus each skill rank provides. This function sets
	 * the progression that is applied by default. There are several mechanisms that may change the progression for indivdual
	 * characters, such as training packages, or professions, so this value should only be used to initialise skills.
	 *
	 * The category progression is defines the bonus that each rank in the category provides.
	 *
	 * There are only two valid progressions for the category:
	 *  - kStandard: If the skill progression is also kStandard
	 *  - kNone: All other cases
	 * Any attempt to set another combination will throw an exception
	 *
	 * @param skill_progression SkillProgressionTypeData&  progression to set for skills in the category
	 * @param category_progression SkillProgressionTypeData&  progression to set for the category
	 *
	 * @throws InvalidSkillProgression if the combination of progressions is invalid
	 */
	void setSkillProgressions(const SkillProgressionTypeData& skill_progression, const SkillProgressionTypeData& category_progression);

	/**
	 * @brief Gets the default skill progression for skills in the category
	 *
	 * Each skill in the category has a skill progression that defines the bonus each skill rank provides. This function gets
	 * the progression that is applied by default. There are several mechanisms that may change the progression for indivdual
	 * characters, such as training packages, or professions, so this may not be the progression used after the skill has been
	 * initialised.
	 *
	 * @return SkillProgressionTypeData&  default skill progressiomn type
	 */
	const SkillProgressionTypeData& defaultSkillProgression() const {
		return *skill_progression_;
	}

	/**
	 * @brief Gets the skill progression for the skill category
	 *
	 * Each skill category has a skill progression that defines the bonus each skill rank in the category provides. The bonus
	 * typically provides diminishing returns for the number of ranks. This function gets the progression that will be
	 * applied to this skill category.
	 *
	 * @return SkillProgressionType::Type default skill progressiomn type
	 */
	const SkillProgressionTypeData& skillCategoryProgression() const {
		return *skill_category_progression_;
	}

	/**
	 * @brief Thrown when attempting to add a skill category stats when the category is set to use character realm stats
	 *
	 * It is invalid to have both skill category stats and also to use the characters realm stats as these are mutually
	 * exclusive options.
	 */
	class UsingCharacterRealmStatsException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		UsingCharacterRealmStatsException(const std::string& error) : std::runtime_error{error} {
		}
	};

	/**
	 * @brief Set the character traits relevant to the skill category
	 *
	 * These traits may be used by the character AI when making decisions about which skill categories to develop and how to use them.
	 *
	 * @param traits CharacterTraits struct containing the traits relevant to the skill category
	 */
	void setTraits(rm::game::character::CharacterTraits traits) {
		traits_ = traits;
	}

	/**
	 * @brief Get the character traits relevant to the skill category
	 *
	 * These traits may be used by the character AI when making decisions about which skill categories to develop and how to use them.
	 *
	 * @return CharacterTraits struct containing the traits relevant to the skill category
	 */
	const rm::game::character::CharacterTraits traits() const {
		return traits_;
	}

private:
	const SkillGroupData* group_{};                                /**< Skill group the category belongs to */
	const SkillProgressionTypeData* skill_progression_{};          /**< How many bonus points each skill rank provides by default in skills in the category */
	const SkillProgressionTypeData* skill_category_progression_{}; /**< How many bonus points each skill rank provides to the category */
	std::string name_{};                                           /**< Name of the category as seen in-game */
	std::vector<StatType::Type> stats_{};                          /**< Stats providing a bonus to the category */
	bool use_realm_stats_{};                                       /**< Whether the realm stat of the character should determine the applicable stats */
	rm::game::character::CharacterTraits traits_{};                                     /**< Character traits relevant to the category that may be used by the character AI when making decisions about which skills to develop and how to use them */
};

} // namespace rm::rule