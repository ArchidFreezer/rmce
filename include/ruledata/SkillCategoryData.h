#pragma once

#include <string_view>
#include <vector>

#include "GameRuleData.h"
#include "SkillGroupType.h"
#include "StatType.h"
#include <SkillProgressionTypeData.h>

/**
 * @brief Thrown when more skill category stats than are allowed are attempted to be added
 * 
 * Skill categories allow the bonus for 3 stats to be applied to the total skill bonus. If more than this number are
 * attempted to be added this exception should be thrown.
 */
class TooManyStatsException : public std::runtime_error {
public:
	/**
	 * @brief Exception constructor
	 *
	 * std::runtime_error will handle the string
	 *
	 * @param error String to display for the error
	 */
	TooManyStatsException(const std::string& error) : std::runtime_error{ error }	{}
};

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
	InvalidSkillProgression(const std::string& error)	: std::runtime_error{ error } {}
};

/**
 * @class  SkillCategoryData
 * @brief Class to represent a skill category
 * 
 * Each skill is grouped with other similar skills in a specific skill category. Each category contains skills that utilize
 * similar physical and mental capabilities (i.e., stats) and similar basic, general abilities. In addition to developing
 * individual skills, a character may develop and improve all of the skills in a given skill category by increasing its
 * skill category bonus
 */
class SkillCategoryData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param id Unique identifier of the skill category
	 */
	SkillCategoryData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Constructor
	 * @param group Skill group that the category belongs to
	 * @param id Unique identifier of the skill category
	 * @param name Name of the category as seen in-game
	 * @param skillProgression Default progression type for skills in the category
	 * @param categoryProgression Progression type for the skill category
	 */
	SkillCategoryData(std::string_view id, std::string_view name, SkillGroupType::Type group, SkillProgressionTypeData& skillProgression, SkillProgressionTypeData& categoryProgression) :
		GameRuleData(id),
		group_{ group },
		name_{ name },
		skillProgression_{ &skillProgression },
		skillCategoryProgression_{ &categoryProgression } {}


	/**
	 * @brief Get the name of the category as seen in-game
	 * @return Name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Gets the full name of the skill category which includes the skill group
	 * @return Name including the skill group
	 */
	const std::string getFullName() const { return toString(group_) + " - " + name_; }

	/**
	 * @brief Get the group the category belongs to
	 * @return SkillGroupType::Type categories skill group
	 */
	const SkillGroupType::Type getGroup() const { return group_; }

	/**
	 * @brief Add stat that applies stat boinus to skills in the category
	 * 
	 * There are 3 stats associated with each skill category and duplicates are allowed
	 * 
	 * @param stat whose bonus should be applied to skills category
	 * @returns Number of stats associated withthe category after the operation
	 * @throws TooManyStatsException if attempting to add a stat when 3 already have been applied
	 */
	int addStat(StatType::Type stat);

	/**
	 * @brief Gets the number of stats currently associated with the category
	 * 
	 * Up to 3 stats bonuses may be applied to skills in a skill category. This function returns the current number applied
	 * @return Number of stats applied
	 */
	const int getNumberOfStats() const {	return stats_.size();	}

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
	void clearStats() { stats_.clear(); }

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
	 * @param skillProgression SkillProgressionTypeData&  progression to set for skills in the category
	 * @param categoryProgression SkillProgressionTypeData&  progression to set for the category
	 * 
	 * @throws InvalidSkillProgression if the combination of progressions is invalid
	 */
	void setSkillProgressions(const SkillProgressionTypeData& skillProgression, const SkillProgressionTypeData& categoryProgression);

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
	const SkillProgressionTypeData& getDefaultSkillProgression() const { return *skillProgression_; }

	/**
	 * @brief Gets the skill progression for the skill category
	 *
	 * Each skill category has a skill progression that defines the bonus each skill rank in the category provides. The bonus
	 * typically provides diminishing returns for the number of ranks. This function gets the progression that will be
	 * applied to this skill category.
	 *
	 * @return SkillProgressionType::Type default skill progressiomn type
	 */
	const SkillProgressionTypeData& getSkillCategoryProgression() const { return *skillCategoryProgression_; }

private:
	SkillGroupType::Type group_{}; /**< Name of the skill group; the category belongs to */
	const SkillProgressionTypeData* skillProgression_{}; /**< How many bonus points each skill rank provides by default in skills in the category */
	const SkillProgressionTypeData* skillCategoryProgression_{}; /**< How many bonus points each skill rank provides to the category */
	std::string name_{}; /**< Name of the category as seen in-game */
	std::vector<StatType::Type> stats_{}; /**< Stats providing a bonus to the category */

};
