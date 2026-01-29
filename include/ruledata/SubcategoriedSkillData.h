#pragma once
#include <cctype>
#include <optional>
#include <string_view>

#include <SkillData.h>

/**
 * @class SubcategoriedSkillData
 * @brief Wrapper class for a Skilldata object that allows for a specific skill subcategory to be defined.
 * 
 * This is typically referenced in classes that are used for character development such as races or professions where they
 * need to be able to set modifiers on specific subcategories of a skill. An example would be that the Dwarf race sets the
 * Survival skill as everyman, but only for the underground subcategory.
 * 
 * The subcategory is optional to allow this wrapper to be used for skills that do not have subcategories or the modifier
 * is for all subcategories of the skill.
 */
class SubcategoriedSkillData {
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
	SubcategoriedSkillData(const SkillData& skill_data, std::optional<std::string_view> subcategory=std::nullopt) : 
		skill_data_{ &skill_data }, 
		subcategory_{ subcategory }, 
		id_{ skill_data_->id() + (subcategory_ ? "_" + subcategory_.value() : "") } {}

	/**
	 * @brief Get the skill data that the subcategory is for
	 * @return SkillData reference
	 */
	const SkillData& skillData() const { return *skill_data_; }

	/**
	 * @brief Get the subcategory
	 * @return string name of the subcategory
	 */
	const std::optional<std::string>& subcategory() const { return subcategory_; }

	/**
	 * @brief Get a unique identifier for the object
	 * @return 
	 */
	const std::string& id() const { return id_; }

	/**
	 * @brief Override the less than operator
	 *
	 * The creation of this overload allows all SubcategoriedSkillData objects to be used as keys in sorted containers
	 *
	 * @param other SubcategoriedSkillData object to compoare against
	 * @return `true` if this object is consdered to be < @a other
	 * @return `false` if this object is not consdered to be < @a other
	 */
	bool operator<(const SubcategoriedSkillData& other) const { return (id_ < other.id_); }

private:
	const SkillData* skill_data_{}; /**< Skill that the subcategory is for */
	std::optional<std::string> subcategory_{}; /**< Optional subcategory name */
	const std::string id_{}; /**< Unique identifier */
};
