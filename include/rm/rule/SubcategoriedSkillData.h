#pragma once
#include <cctype>
#include <optional>
#include <string_view>

#include <GameRuleData.h>
#include <SkillData.h>

namespace rm::rule {

/**
 * @class SubcategoriedSkillData
 * @brief Wrapper class for a Skilldata object that allows for a specific skill subcategory to be defined.
 *
 * This is typically referenced in classes that are used for character development such as races or professions where they need to be able to set modifiers on specific subcategories of a skill. An example would be that the Dwarf race sets
 * the Survival skill as everyman, but only for the underground subcategory.
 *
 * The subcategory is optional to allow this wrapper to be used for skills that do not have subcategories or the modifier is for all subcategories of the skill.
 */
class SubcategoriedSkillData : public GameRuleData {
public:
	/**
	 * @brief Do not allow construction without the skill and subcategory being defined
	 */
	SubcategoriedSkillData() = delete;

	/**
	 * @brief Constructor
	 * @param skill_data SkillData the skill the subcategory is for
	 * @param subcategory Optional subcategory
	 */
	SubcategoriedSkillData(const SkillData& skill_data, std::optional<std::string_view> subcategory = std::nullopt)
	    : GameRuleData(skill_data.id() + (subcategory ? "_" + std::string(subcategory.value()) : "")), skill_data_{&skill_data}, subcategory_{subcategory} {
	}

	inline static std::string prefix_{"SUBCATEGORIEDSKILL"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return SubcategoriedSkillData::prefix_;
	}

	/**
	 * @brief Get the skill data that the subcategory is for
	 * @return SkillData reference
	 */
	const SkillData& skillData() const {
		return *skill_data_;
	}

	/**
	 * @brief Get the subcategory
	 * @return string name of the subcategory
	 */
	const std::optional<std::string>& subcategory() const {
		return subcategory_;
	}

private:
	const SkillData* skill_data_{};            /**< Skill that the subcategory is for */
	std::optional<std::string> subcategory_{}; /**< Optional subcategory name */
};

} // namespace rm::rule