#pragma once

#include <string>

#include "GameRuleData.h"

/**
 * @class SkillProgressionTypeData
 * 
 * @brief Class representing the rules for the bonus that skill ranks provide
 *
 * When a character developes either a skillcategory or a skill they spend a development point and gain a certain bonus to
 * the use of applicable skills. The size of the bonus gained depends on two things: the number of development points
 * spent and also the progression type.
 *
 * The progression types are applied at both the skill category and skill level.
 */
class SkillProgressionTypeData : public GameRuleData {
public:

private:
	std::string name_{};  /**< Name of the progression type */
	float zero_{}; /**< Bonus provided for zero skill ranks */
	float ten_{}; /**< Bonus provided for between 1 and 10 skill ranks */
	float twenty_{}; /**< Bonus provided for between 11 and 20 skill ranks */
	float thirty_{}; /**< Bonus provided for between 21 and 30 skill ranks */
	float remainder_{}; /**< Bonus provided for over 30 skill ranks */
};