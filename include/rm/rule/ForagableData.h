#pragma once

#include <string>

#include <GameRuleData.h>
#include <Location.h>
#include <ForagableEffectType.h>
#include <ForagablePreparationType.h>
#include <SkillDifficultyType.h>

using namespace rm::rule::enums;

namespace rm::rule {

/**
 * @class ForagableData
 * @brief Details regarding herbs and other items that can be found in the game via foraging
 */
class ForagableData : public GameRuleData {
public:
	/**
	 * @brief Default constructor is deleted to ensure the base class is initialised
	 */
	ForagableData() = delete;

	/**
	 * @brief Constructor to initialise base class
	 * @param id Unique identifier for the foragable resource
	 */
	ForagableData(std::string_view id) : GameRuleData(id) {};

	inline static std::string prefix_{"FORAGABLE"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return ForagableData::prefix_;
	}

	/**
	 * @brief Set the name of the foragable resource
	 * @param name Name of the foragable resource
	 */
	void setName(std::string_view name) {
		name_ = name;
	}

	/**
	 * @brief Get the name of the foragable resource
	 * @return Name of the foragable resource as a string reference
	 */
	const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Set the effect type of the foragable resource, used to determine the type of effect that the prepared resource has when used
	 * @param effect_type The effect type of the foragable resource, used to determine the type of effect that the prepared resource has when used
	 */
	void setEffectType(ForagableEffectType::Type effect_type) {
		effect_type_ = effect_type;
	}

	/**
	 * @brief Get the effect type of the foragable resource, used to determine the type of effect that the prepared resource has when used
	 * @return The effect type of the foragable resource, used to determine the type of effect that the prepared resource has when used
	 */
	ForagableEffectType::Type effectType() const {
		return effect_type_;
	}

	/**
	 * @brief Set the location definition for the foragable resource
	 * @param location Location definition for the foragable resource, used to determine where the resource can be found in the game world. This is used to match against specific locations to determine if the resource can be found there.
	 */
	void setLocation(rm::game::Location location) {
		location_ = std::make_unique<rm::game::Location>(std::move(location));
	}

	/**
	 * @brief Get the location for the foragable resource
	 * @return Location definition for the foragable resource, used to determine where the resource can be found in the game world. This is used to match against specific locations to determine if the resource can be found there.
	 */
	const rm::game::Location& location() const {
		return *location_;
	}

	/**
	 * @brief Set the difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource
	 * @param difficulty Difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource
	 */
	void setDifficulty(SkillDifficultyType::Type difficulty) {
		difficulty_ = difficulty;
	}

	/**
	 * @brief Get the difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource
	 * @return Difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource
	 */
	SkillDifficultyType::Type difficulty() const {
		return difficulty_;
	}

	/**
	 * @brief Get the difficulty modifier for the resource, used to modify the skill check roll when foraging for the resource based on its difficulty.
	 * @return Difficulty modifier for the resource, used to determine the skill check difficulty when foraging for the resource
	 */
	int difficultyModifier() const {
		switch (difficulty()) {
		case SkillDifficultyType::kRoutine:
			return 30;
		case SkillDifficultyType::kEasy:
			return 20;
		case SkillDifficultyType::kLight:
			return 10;
		case SkillDifficultyType::kMedium:
			return 0;
		case SkillDifficultyType::kHard:
			return -10;
		case SkillDifficultyType::kVeryHard:
			return -20;
		case SkillDifficultyType::kExtremelyHard:
			return -30;
		case SkillDifficultyType::kSheerFolly:
			return -50;
		case SkillDifficultyType::kAbsurd:
			return -70;
		default:
			return 0;
		}
	}

	/**
	 * @brief Get the difficulty rating for the resource, used to determine the number of doses found when looking for specific resources. This is a relative rating compared to other resources and is not used for skill checks.
	 * @return Difficulty rating for the resource, used to determine the number of doses found when looking for specific resources
	 */
	int difficultyRating() const {
		switch (difficulty()) {
		case SkillDifficultyType::kRoutine:
			return 1;
		case SkillDifficultyType::kEasy:
			return 2;
		case SkillDifficultyType::kLight:
			return 3;
		case SkillDifficultyType::kMedium:
			return 4;
		case SkillDifficultyType::kHard:
			return 5;
		case SkillDifficultyType::kVeryHard:
			return 6;
		case SkillDifficultyType::kExtremelyHard:
			return 7;
		case SkillDifficultyType::kSheerFolly:
			return 8;
		case SkillDifficultyType::kAbsurd:
			return 9;
		default:
			return 0;
		}
	}

	/**
	 * @brief Set the form of the resource, used to determine how the resource is found in its raw state
	 * @param form The form of the resource, used to determine how the resource is found in its raw state
	 */	
	void setForm(std::string_view form) {
		form_ = form;
	}

	/**
	 * @brief Get the form of the resource, used to determine how the resource is found in its raw state
	 * @return The form of the resource, used to determine how the resource is found in its raw state as a string reference
	 */
	const std::string& form() const {
		return form_;
	}

	/**
	 * @brief Set the preparation type of the resource, used to determine how the resource can be prepared and used after it is foraged
	 * @param preparation_type The preparation type of the resource, used to determine how the resource can be prepared and used after it is foraged
	 */
	void setPreparationType(ForagablePreparationType::Type preparation_type) {
		preparation_type_ = preparation_type;
	}

	/**
	 * @brief Get the preparation type of the resource, used to determine how the resource can be prepared and used after it is foraged
	 * @return The preparation type of the resource, used to determine how the resource can be prepared and used after it is foraged
	 */
	ForagablePreparationType::Type preparationType() const {
		return preparation_type_;
	}

	/**
	 * @brief Set the addiction factor of the resource, optionally used to determine how addictive the resource is when consumed
	 * @param addiction_factor The addiction factor of the resource, optionally used to determine how addictive the resource is when consumed
	 */
	void setAddictionFactor(int addiction_factor) {
		addiction_factor_ = addiction_factor;
	}

	/**
	 * @brief Get the addiction factor of the resource, optionally used to determine how addictive the resource is when consumed
	 * @return The addiction factor of the resource, optionally used to determine how addictive the resource is when consumed
	 */
	int addictionFactor() const {
		return addiction_factor_;
	}

	/**
	 * @brief Set the cost of the foragable resource, used to determine how much it is worth when bought or sold. This is more a relative guide compared to other resources.
	 * @param cost The cost of the foragable resource, used to determine how much it is worth when bought or sold. This is more a relative guide compared to other resources.
	 */
	void setCost(std::string_view cost) {
		cost_ = cost;
	}

	/**
	 * @brief Get the cost of the foragable resource, used to determine how much it is worth when bought or sold. This is more a relative guide compared to other resources.
	 * @return The cost of the foragable resource, used to determine how much it is worth when bought or sold. This is more a relative guide compared to other resources as a string reference.
	 */
	const std::string& cost() const {
		return cost_;
	}

	/**
	 * @brief Set the effect the prepared form of the resource has when used
	 * @param effect The effect of the foragable resource
	 */
	void setEffect(std::string_view effect) {
		effect_ = effect;
	}

	/**
	 * @brief Get the effect of the prepared form of the foragable resource
	 * @return The effect of the prepared foragable resource as a string reference
	 */
	const std::string& effect() const {
		return effect_;
	}

	/**
	 * @brief Get the skill used to identify the herb
	 * @return The skill used to identify the herb
	 */
	const SubcategoriedSkillData* loreSkill() const {
		return lore_skill_;
	}

	/**
	 * @brief Set the skill used to identify the herb
	 * @param lore_skill The skill used to identify the herb
	 */
	void setLoreSkill(const SubcategoriedSkillData* lore_skill) {
		lore_skill_ = lore_skill;
	}

private:
	std::string name_{};                                /**< The name of the foragable resource. */
	ForagableEffectType::Type effect_type_{};           /**< The effect type of the foragable resource, used to determine the type of effect that the prepared resource has when used. */
	std::unique_ptr<rm::game::Location> location_{};    /**< The location of the foragable resource. */
	SkillDifficultyType::Type difficulty_{};            /**< The difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource. */
	std::string form_{};                                /**< The form of the resource, used to determine how the resource is found in its raw state. */
	ForagablePreparationType::Type preparation_type_{}; /**< The preparation type of the resource, used to determine how the resource can be prepared and used after it is foraged. */
	int addiction_factor_{};                            /**< The addiction factor of the resource, optionally used to determine how addictive the resource is when consumed. */
	std::string cost_{};                                /**< The cost of the foragable resource, used to determine how much it is worth when bought or sold. This is more a relative guide compared to other resources. */
	std::string effect_{};                              /**< The effect that the prepared resource has when used. */
	const SubcategoriedSkillData* lore_skill_{nullptr}; /**< The skill used to identify the herb. */
};

} // namespace rm::rule