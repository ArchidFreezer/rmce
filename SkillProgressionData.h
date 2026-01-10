#pragma once

#include "GameRuleData.h"
#include "SkillProgressionType.h"

/**
 * @class SkillProgressionData
 * 
 * @brief Class representing a skill progression for a skill or skill category
 * 
 * Each skill or skill category that is developed by a character has a progression associated with it which determines
 * the bonus applied given the number of skill ranks developed. Each progression gives a different bonus. The development
 * type is determined by the skill development type.
 * 
 * @see #SkillProgressionType
 */
class SkillProgressionData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param type #SkillProgressionType that the progression uses
	 */
	SkillProgressionData(SkillProgressionType type);

	/**
	 * @brief Get the skill bonus provided by the progression type for the number of ranks defined in the parameter
	 * @param ranks Number of ranks to get the bonus for
	 * @return int containing the skill bonus
	 */
	int getBonus(int ranks);

	/**
	 * @brief Get the unique identifier for the book
	 * @return Unique identifier as a string reference
	 */
	inline const std::string& id() { return id_; }

private:
	/**
	 * @brief Get the bonus given the number of ranks and the bonus per rank provided
	 * @param ranks Number of ranks to get the bonus for
	 * @param zero Bonus for 0 ranks
	 * @param ten Bonus for the first 10 ranks
	 * @param twenty Bonus for ranks 11 to 20
	 * @param thirty Bonus for ranks 21 to 30
	 * @param remaining Bonus for any ranks over 30
	 * @return int containing the skill bonus
	 */
	int getBonus(int ranks, float zero, float ten, float twenty, float thirty, float remaining);

	SkillProgressionType type_;
	std::string id_{};
};
