#pragma once

#include <GameRuleData.h>

namespace rm::rule {

/**
 * @class SkillGroupData
 * @brief Skill group
 *
 * A skil group is the top level of the skill hierarchy and is a container for skill categories. It does not have a significant impact in game, but is used in some other game data objects to define choices based on the skills in the group.
 *
 */
class SkillGroupData : public GameRuleData {
public:
	SkillGroupData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

	/**
	 * @brief Constructor
	 * @param id Unique identifier of the skill group
	 */
	SkillGroupData(std::string_view id) : GameRuleData(id) {
	}

	inline static std::string prefix_{"SKILLGROUP"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return SkillGroupData::prefix_;
	}

	/**
	 * @brief Set the name of the skill group
	 * @param name skill group name
	 */
	void setName(std::string_view name) {
		name_ = name;
	}

	/**
	 * @brief Get the name of the group as seen in-game
	 * @return Name as a string reference
	 */
	const std::string& name() const {
		return name_;
	}

private:
	std::string name_{}; /**< Name of the game as seen in-game */
};

} // namespace rm::rule