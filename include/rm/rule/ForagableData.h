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
	void setFindDifficulty(SkillDifficultyType::Type difficulty) {
		find_difficulty_ = difficulty;
	}

	/**
	 * @brief Get the difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource
	 * @return Difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource
	 */
	SkillDifficultyType::Type findDifficulty() const {
		return find_difficulty_;
	}

	/**
	 * @brief Get the difficulty modifier for the resource, used to modify the skill check roll when foraging for the resource based on its difficulty.
	 * @return Difficulty modifier for the resource, used to determine the skill check difficulty when foraging for the resource
	 */
	int findDifficultyModifier() const {
		return SkillDifficultyType::difficultyModifier(find_difficulty_);
	}

	/**
	 * @brief Get the difficulty rating for the resource, used to determine the number of doses found when looking for specific resources. This is a relative rating compared to other resources and is not used for skill checks.
	 * @return Difficulty rating for the resource, used to determine the number of doses found when looking for specific resources
	 */
	int findDifficultyRating() const {
		return SkillDifficultyType::difficultyRating(find_difficulty_);
	}

	/**
	 * @brief Set the other uses of the resource, such as detrimental or cultural
	 * @param other_uses The other uses of the resource, such as detrimental or cultural
	 */	
	void setOtherUses(std::string_view other_uses) {
		other_uses_ = other_uses;
	}

	/**
	 * @brief Get the other uses of the resource, such as detrimental or cultural
	 * @return The other uses of the resource, such as detrimental or cultural, as a string reference
	 */
	const std::string& otherUses() const {
		return other_uses_;
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
	 * @brief Set the medicinal uses of the resource, describing the effect the prepared form of the resource has when used
	 * @param medicinal_uses The medicinal uses of the foragable resource
	 */
	void setMedicinalUses(std::string_view medicinal_uses) {
		medicinal_uses_ = medicinal_uses;
	}

	/**
	 * @brief Get the medicinal uses of the prepared form of the foragable resource
	 * @return The medicinal uses of the prepared foragable resource as a string reference
	 */
	const std::string& medicinalUses() const {
		return medicinal_uses_;
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

	/**
	 * @brief Get the characteristics for the foragable resource, describing its visual appearance, smell, taste, and other notable features. This is used to provide additional information about the resource.
	 * @return The characteristics for the foragable resource as a string reference
	 */
	const std::string& characteristics() const {
		return characteristics_;
	}

	/**
	 * @brief Set the characteristics for the foragable resource, describing its visual appearance, smell, taste, and other notable features. This is used to provide additional information about the resource.
	 * @param characteristics The characteristics for the foragable resource.
	 */
	void setCharacteristics(std::string_view characteristics) {
		characteristics_ = characteristics;
	}

	/**
	 * @brief Get any warnings for the foragable resource, describing any potential dangers or precautions. This is used to provide additional information about the resource.
	 * @return Any warnings for the foragable resource as a string reference
	 */
	const std::string& warning() const {
		return warning_;
	}

	/**
	 * @brief Set any warnings for the foragable resource, describing any potential dangers or precautions. This is used to provide additional information about the resource.
	 * @param warning Any warnings for the foragable resource.
	 */
	void setWarning(std::string_view warning) {
		warning_ = warning;
	}

private:
	std::string name_{};                                /**< The name of the foragable resource. */
	ForagableEffectType::Type effect_type_{}; /**< The effect type of the foragable resource, used to determine the type of effect that the prepared resource has when used. */
	const SubcategoriedSkillData* lore_skill_{nullptr}; /**< The skill used to identify the herb. */
	std::unique_ptr<rm::game::Location> location_{};    /**< The location of the foragable resource. */
	ForagablePreparationType::Type preparation_type_{}; /**< The preparation type of the resource, used to determine how the resource can be prepared and used after it is foraged. */
	SkillDifficultyType::Type find_difficulty_{};       /**< The difficulty of foraging the resource, used to determine the skill check difficulty when foraging for the resource. */
	int addiction_factor_{};                            /**< The addiction factor of the resource, optionally used to determine how addictive the resource is when consumed. */
	std::string cost_{};                                /**< The cost of the foragable resource, used to determine how much it is worth when bought or sold. This is more a relative guide compared to other resources. */
	std::string characteristics_{};                     /**< The characteristics for the foragable resource, used to provide additional information about the resource. */
	std::string medicinal_uses_{};                              /**< The medicinal uses of the foragable resource, describing the effect the prepared form of the resource has when used. */
	std::string other_uses_{};                                /**< The other uses of the resource, such as detrimental or cultural. */
	std::string warning_{};                                  /**< Any warnings for the resource, describing any potential dangers or precautions. */
};

} // namespace rm::rule