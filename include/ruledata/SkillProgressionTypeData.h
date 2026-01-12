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
	/**
	 * @brief Constructor
	 * @param id Unique identifier for the progression type
	 * @param name In game name of the progression type
	 * @param zero Bonus provided with 0 skill ranks
	 * @param ten Bonus provided per rank for between 1 and 10 ranks
	 * @param twenty Bonus provided per rank for between 11 and 20 ranks
	 * @param thirty Bonus provided per rank for between 21 and 30 ranks
	 * @param remaining Bonus provided per rank for over 30 ranks
	 */
	SkillProgressionTypeData(std::string_view id, std::string_view name, float zero, float ten, float twenty, float thirty, float remaining);

	/**
	 * @brief Constructor
	 * @param id Unique identifier for the progression type
	 */
	SkillProgressionTypeData(std::string_view id);

	/**
	 * @brief Set the name of the progression type
	 * @param name Progression type name
	 */
	inline void setName(std::string_view name) { name_ = name; }
	
	/**
	 * @brief Get the name of the progression type
	 * @return Progression type name as a string reference
	 */
	inline const std::string& name() { return name_; }

	/**
	 * @brief Set the bonus provided with 0 skill ranks
	 * @param zero Bonus for 0 skill ranks
	 */
	inline void setZero(float zero) { zero_ = zero; }
	/**
	 * @brief Get the bonus provided with 0 skill ranks
	 * @return Bonus for 0 skill ranks
	 */
	inline float zero() { return zero_; }

	/**
	 * @brief Set the bonus provided per rank for between 1 and 10 ranks
	 * @param ten Bonus provided per rank for between 1 and 10 ranks
	 */
	inline void setTen(float ten) { ten_ = ten; }
	/**
	 * @brief Get the bonus provided per rank for between 1 and 10 ranks
	 * @return Bonus provided per rank for between 1 and 10 ranks
	 */
	inline float ten() { return ten_; }

	/**
	 * @brief Set the bonus provided per rank for between 11 and 20 ranks
	 * @param ten Bonus provided per rank for between 11 and 20 ranks
	 */
	inline void setTwenty(float twenty) { twenty_ = twenty; }
	/**
	 * @brief Get the bonus provided per rank for between 11 and 20 ranks
	 * @return Bonus provided per rank for between 11 and 20 ranks
	 */
	inline float twenty() { return twenty_; }

	/**
	 * @brief Set the bonus provided per rank for between 21 and 30 ranks
	 * @param ten Bonus provided per rank for between 21 and 30 ranks
	 */
	inline void setThirty(float thirty) { thirty_ = thirty; }
	/**
	 * @brief Get the bonus provided per rank for between 21 and 30 ranks
	 * @return Bonus provided per rank for between 21 and 30 ranks
	 */
	inline float thirty() { return thirty_; }

	/**
	 * @brief Set the bonus provided per rank above 30
	 * @param ten Bonus provided per rank above 30
	 */
	inline void setRemaining(float remaining) { remaining_ = remaining; }
	/**
	 * @brief Get the bonus provided per rank above 30
	 * @return Bonus provided per rank for above 30
	 */
	inline float remaining() { return remaining_; }

private:
	std::string name_{};  /**< Name of the progression type */
	float zero_{}; /**< Bonus provided for zero skill ranks */
	float ten_{}; /**< Bonus provided for between 1 and 10 skill ranks */
	float twenty_{}; /**< Bonus provided for between 11 and 20 skill ranks */
	float thirty_{}; /**< Bonus provided for between 21 and 30 skill ranks */
	float remaining_{}; /**< Bonus provided for over 30 skill ranks */
};

inline SkillProgressionTypeData::SkillProgressionTypeData(std::string_view id) : GameRuleData(id) {}

inline SkillProgressionTypeData::SkillProgressionTypeData(std::string_view id, std::string_view name, float zero, float ten, float twenty, float thirty, float remaining) :
	GameRuleData(id),
	name_{ name },
	zero_{ zero },
	ten_{ ten },
	twenty_{ twenty },
	thirty_{ thirty },
	remaining_{ remaining } {}