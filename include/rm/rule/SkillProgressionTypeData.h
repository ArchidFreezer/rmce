#pragma once

#include <string>
#include <set>

#include <GameRuleData.h>
#include <SkillProgressionData.h>

namespace rm::rule {

/**
 * @class SkillProgressionTypeData
 *
 * @brief Class representing the rules for the bonus that skill ranks provide
 *
 * When a character developes either a skillcategory or a skill they spend a development point and gain a certain bonus to the use of applicable skills. The size of the bonus gained depends on two things: the number of development points
 * spent and also the progression type.
 *
 * The progression types are applied at both the skill category and skill level.
 */
class SkillProgressionTypeData : public GameRuleData, SkillProgressionData {
public:
	SkillProgressionTypeData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

	/**
	 * @brief Constructor
	 * @param id Unique identifier for the progression type
	 */
	SkillProgressionTypeData(std::string_view id) : GameRuleData(id) {
	}

	inline static std::string prefix_{"SKILLPROGRESSIONTYPE"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return SkillProgressionTypeData::prefix_;
	}

	/**
	 * @brief Set the name of the progression type
	 * @param name Progression type name
	 */
	void setName(std::string_view name) {
		name_ = name;
	}

	/**
	 * @brief Get the name of the progression type
	 * @return Progression type name as a string reference
	 */
	const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Set the bonus provided with 0 skill ranks
	 * @param zero Bonus for 0 skill ranks
	 */
	void setZero(float zero) {
		zero_ = zero;
	}
	/**
	 * @brief Get the bonus provided with 0 skill ranks
	 * @return Bonus for 0 skill ranks
	 */
	float zero() const {
		return zero_;
	}

	/**
	 * @brief Set the bonus provided per rank for between 1 and 10 ranks
	 * @param ten Bonus provided per rank for between 1 and 10 ranks
	 */
	void setTen(float ten) {
		ten_ = ten;
	}
	/**
	 * @brief Get the bonus provided per rank for between 1 and 10 ranks
	 * @return Bonus provided per rank for between 1 and 10 ranks
	 */
	float ten() const {
		return ten_;
	}

	/**
	 * @brief Set the bonus provided per rank for between 11 and 20 ranks
	 * @param twenty Bonus provided per rank for between 11 and 20 ranks
	 */
	void setTwenty(float twenty) {
		twenty_ = twenty;
	}
	/**
	 * @brief Get the bonus provided per rank for between 11 and 20 ranks
	 * @return Bonus provided per rank for between 11 and 20 ranks
	 */
	float twenty() const {
		return twenty_;
	}

	/**
	 * @brief Set the bonus provided per rank for between 21 and 30 ranks
	 * @param thirty Bonus provided per rank for between 21 and 30 ranks
	 */
	void setThirty(float thirty) {
		thirty_ = thirty;
	}
	/**
	 * @brief Get the bonus provided per rank for between 21 and 30 ranks
	 * @return Bonus provided per rank for between 21 and 30 ranks
	 */
	float thirty() const {
		return thirty_;
	}

	/**
	 * @brief Set the bonus provided per rank above 30
	 * @param remaining Bonus provided per rank above 30
	 */
	void setRemaining(float remaining) {
		remaining_ = remaining;
	}
	/**
	 * @brief Get the bonus provided per rank above 30
	 * @return Bonus provided per rank for above 30
	 */
	float remaining() const {
		return remaining_;
	}

	/**
	 * @brief Get the skill bonus provided by the progression type for the number of ranks defined in the parameter
	 * @param ranks Number of ranks to get the bonus for
	 * @return int containing the skill bonus
	 */
	int bonus(int ranks) const override;

private:
	std::string name_{}; /**< Name of the progression type */
	float zero_{};       /**< Bonus provided for zero skill ranks */
	float ten_{};        /**< Bonus provided for between 1 and 10 skill ranks */
	float twenty_{};     /**< Bonus provided for between 11 and 20 skill ranks */
	float thirty_{};     /**< Bonus provided for between 21 and 30 skill ranks */
	float remaining_{};  /**< Bonus provided for over 30 skill ranks */

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
	int bonus(int ranks, float zero, float ten, float twenty, float thirty, float remaining) const;
};

/* ------------------------------------------------------------------ */
/* Free functions                                                     */
/* ------------------------------------------------------------------ */

/**
 * @brief Compute the combined progression from a set of progression types
 *
 * When a character has multiple magical realms, such as a hybrid caster, we need to combine the progression to determine the final power point progression. This function takes a set of progression types and computes the combined
 * progression by averaging the bonuses provided by each progression type for the given number of ranks.
 *
 * @param progression_types A set of pointers to SkillProgressionTypeData objects representing the progression types to combine.
 * @param combined_progression A reference to a SkillProgressionTypeData object where the combined progression will be stored.
 */
void computeCombinedProgression(const std::set<const SkillProgressionTypeData*>& progression_types, SkillProgressionTypeData& combined_progression);

} // namespace rm::rule