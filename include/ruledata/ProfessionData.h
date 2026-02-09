#pragma once

#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <vector>

#include <BookData.h>
#include <GameRuleData.h>
#include <GameRuleDataChoice.h>
#include <RealmType.h>
#include <SkillCategoryData.h>
#include <SkillData.h>
#include <SkillDevelopmentCost.h>
#include <SkillDevelopmentType.h>
#include <SpellListData.h>
#include <StatType.h>
#include <SubcategoriedSkillData.h>
#include <SpellUserType.h>

/**
 * @class ProfessionData 
 * @brief Class representing the character modifiers that a profession defines
 */
class ProfessionData : public GameRuleData {
public:

	/**
	 * @brief Thrown when there is an attempt to retrieve a skill category development cost for a categopry that does not have one defined
	 */
	class InvalidCategoryDevelopmentCost : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidCategoryDevelopmentCost(const std::string& error) : std::runtime_error{ error } {}
	};

	/**
	 * @brief Thrown when there is an attempt to set an invalid skill bonus
	 */
	class InvalidSkillBonus : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidSkillBonus(const std::string& error) : std::runtime_error{ error } {}
	};

	/**
	 * @brief Thrown when there is an attempt to set a skill as everymen when it is already set as resticted or vise-versa
	 *
	 * A skill cannot be both everymen and restricted for the same race
	 */
	class InvalidSkillDevelopment : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidSkillDevelopment(const std::string& error) : std::runtime_error{ error } {}
	};

	/**
	 * @brief Delete the default constructor to ensure we initialise the base class
	 */
	ProfessionData() = delete;

	/**
	 * @brief Basic constructor to initialise the base class
	 * @param id Unique identifier of the profession
	 */
	ProfessionData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the profession
	 * @param name Profession name
	 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the profession
	 * @return Profession name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of what defines the profession
	 * @param description std::string_view profession description
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Get the description of what the defines profession
	 * @return std::string reference of the description
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set the book that the profession is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_.emplace(&book); }

	/**
	 * @brief Get the book that the profession is defined in
	 *
	 * The book is stored as a pointer and may not have been initialised so it is considered optional. A check should be made
	 * before using the value to determine if the book has been set yet:
	 * @code
	 * if (profession.book()) {                              // Check if the book has been set
	 *   const BookData* book = profession.book().value();   // Get the book pointer
	 *   // Use the book
	 * }
	 * @endcode
	 * or
	 * @code
	 * std::cout << (profession.book() ? profession.book().value()->name() : "Book not set") << std::endl;
	 * @endcode
	 * @return Pointer to the BookData containing the profession definition
	 */
	const std::optional<const BookData*> book() const { return book_; }

	/**
	 * @brief Set the type of spell user the profession is
	 * @param spell_user_type SpellUserType::Type spell user type
	 */
	void setSpellUserType(SpellUserType::Type spell_user_type) { spell_user_type_ = spell_user_type; }

	/**
	 * @brief Get the type of spell user the profession is
	 * @return SpellUserType::Type spell user type
	 */
	SpellUserType::Type spellUserType() const { return spell_user_type_; }

	/**
	 * @brief Add a realm to those the profession draws power from
	 * @param realm RealmType::Type profession power realm
	 */
	void addRealm(RealmType::Type realm) {
		realms_.emplace(realm);
	}

	/**
	 * @brief Get whether the profession draws power from a power realm
	 * @param realm RealmType::Type power realm
	 * @return `true` if the profession draws power from the realm
	 * @return `false` if the profession does not draw power from the realm
	 */
	bool isRealm(RealmType::Type realm) const { return (realms_.find(realm) != realms_.end()); }

	/**
	 * @brief Get the realms that the profession draws power from
	 * @return td::set<RealmType::Type> power realms
	 */
	const std::set<RealmType::Type> realms() { return realms_; }

	/**
	 * @brief Add stat that applies stat bonus to the profession
	 *
	 * There are 3 stats associated with each profession and duplicates are allowed
	 *
	 * @param stat whose bonus should be applied to the profession
	 * @returns Number of stats associated with the profession after the operation
	 * @throws TooManyStatsException if attempting to add a stat when 3 already have been applied
	 * @see setUseRealmStats()
	 */
	int addStat(StatType::Type stat);

	/**
	 * @brief Gets the number of stats currently associated with the profession
	 *
	 * Up to 3 stats bonuses may be applied to profession. This function returns the current number applied
	 * @return Number of stats applied
	 */
	const int numberOfStats() const { return stats_.size(); }

	/**
	 * @brief Remove a stats from those currently associated with the profession
	 *
	 * Remove a stat from the list of those whose bonus is currently applied to the profession.
	 * If the stat is not currently applied then the function does nothing
	 *
	 * @param stat Stat to remove
	 * @return `true` if the stat was removed
	 * @return `false` if the stat was not found or not removed
	 */
	bool removeStat(StatType::Type stat);

	/**
	 * @brief Empties the list of stats associated with the profession
	 */
	void clearStats() { stats_.clear(); }

	/**
	 * @brief Gets the stats that that provide a bonus to the profession
	 *
	 * A stat may appear more than once in the list
	 *
	 * @return std::vector of stats
	 */
	const std::vector<StatType::Type>& stats() const { return stats_; }

	/**
 * @brief Add a choice that defines base spell lists that the character has to choose from
 * @param choice GameRuleDataChoice choice definition
 */
	void addBaseSpellListChoice(GameRuleDataChoice<SpellListData> choice) { base_spell_list_choices_.push_back(std::move(choice)); }

	/**
	 * @brief Get the number of choices a character has to make regarding base spell lists
	 *
	 * During character development it is sometimes necessary to choose one or more base spell lists.This returns how many
	 * choices need to be made. This is not the number of options to be selected in a single choice.
	 * @return Number of choices to be make
	 */
	int numBaseSpellListChoices() const { return base_spell_list_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more base spell lists
	 * @return vector of GameRuleDataChoice objects with the choices to be made
	 */
	const std::vector<GameRuleDataChoice<SpellListData>>& baseSpellListChoices() const { return base_spell_list_choices_; }

	/**
	 * @brief Add a choice that defines the skill that a character may select one or more from to become everyman
	 * @param choice GameRuleDataChoice choice definition
	 * @param type SkillDevelopmentType::Type type choices will have
	 */
	void addSkillDevelopmentTypeChoice(GameRuleDataChoice<SubcategoriedSkillData> choice, SkillDevelopmentType::Type type) { skill_development_type_choices_.emplace(std::move(choice), type); }

	/**
	 * @brief Get the number of choices a character has to make regarding everyman skills
	 *
	 * During character development the player may have the option change the development type of one of more skills.
	 * This returns how many choices need to be made. This is not the number of options to be selected
	 * in a single choice.
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numSkillDevelopmentTypeChoices() const { return skill_development_type_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skills that may have their development type changed
	 * @return map of GameRuleDataChoice objects with the choices to be made
	 */
	const std::map<GameRuleDataChoice<SubcategoriedSkillData>, SkillDevelopmentType::Type>& skillDevelopmentTypeChoices() const { return skill_development_type_choices_; }

	/**
	 * @brief Add a choice that defines the skill that a character may select one or more subcategories from to become everyman
	 * @param choice GameRuleDataChoice choice definition
	 * @param type SkillDevelopmentType::Type type choices will have
	 */
	void addSkillSubcategoryDevelopmentTypeChoice(GameRuleDataChoice<SkillData> choice, SkillDevelopmentType::Type type) { skill_subcategory_skill_development_type_choices_.emplace(std::move(choice), type); }

	/**
	 * @brief Get the number of choices a character has to make regarding skill subcategoryeveryman skills
	 *
	 * During character development the player may have the option change the development type of one of more skills.
	 * This returns how many choices need to be made. This is not the number of options to be selected
	 * in a single choice.
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numSkillSubcategoryDevelopmentTypeChoices() const { return skill_subcategory_skill_development_type_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skill subcategories that may have their development type changed
	 * @return map of GameRuleDataChoice objects with the choices to be made
	 */
	const std::map<GameRuleDataChoice<SkillData>, SkillDevelopmentType::Type>& skillSubcategoryDevelopmentTypeChoices() const { return skill_subcategory_skill_development_type_choices_; }

	/**
	 * @brief Add a choice that defines the skill categories that a character may select one or more skills from to change their development type
	 * @param choice GameRuleDataChoice choice definition
	 * @param type SkillDevelopmentType::Type type choices will have
	 */
	void addSkillCategorySkillDevelopmentTypeChoice(GameRuleDataChoice<SkillCategoryData> choice, SkillDevelopmentType::Type type) { skill_category_skill_development_type_choices_.emplace(std::move(choice), type); }

	/**
	 * @brief Get the number of choices a character has to make regarding development types skills in skill categories
	 *
	 * During character development the player may have the option change the development type of one of more skills.
	 * This returns how many choices need to be made from skill categories. This is not the number of options to be selected
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numSkillCategorySkillDevelopmentTypeChoices() const { return skill_category_skill_development_type_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skills within a skill
	 * category that may have their development type changed
	 * @return vector of GameRuleDataChoice objects with the choices to be made
	 * @see SkillDevelopmentType
	 */
	const std::map<GameRuleDataChoice<SkillCategoryData>, SkillDevelopmentType::Type>& skillCategorySkillDevelopmentTypeChoices() const { return skill_category_skill_development_type_choices_; }

	/**
	 * @brief Add a choice that defines the skill groups that a character may select one or more skills from to change their development type
	 * @param choice GameRuleDataChoice choice definition
	 * @param type SkillDevelopmentType::Type type choices will have
	 */
	void addSkillGroupSkillDevelopmentTypeChoice(GameRuleDataChoice<SkillGroupData> choice, SkillDevelopmentType::Type type) { skill_group_skill_development_type_choices_.emplace(std::move(choice), type); }

	/**
	 * @brief Get the number of choices a character has to make regarding development types skills in skill groups
	 *
	 * During character development the player may have the option change the development type of one of more skills.
	 * This returns how many choices need to be made from skill groups. This is not the number of options to be selected
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numSkillGroupSkillDevelopmentTypeChoices() const { return skill_group_skill_development_type_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skills within a skill
	 * group that may have their development type changed
	 * @return vector of GameRuleDataChoice objects with the choices to be made
	 * @see SkillDevelopmentType
	 */
	const std::map<GameRuleDataChoice<SkillGroupData>, SkillDevelopmentType::Type>& skillGroupSkillDevelopmentTypeChoices() const { return skill_group_skill_development_type_choices_; }

	/**
	 * @brief Add a special bonus to all skills in a group
	 * @param group Name of the group
	 * @param bonus bonus value
	 */
	void addSkillGroupSpecialBonus(const SkillGroupData& group, int bonus) { skill_group_special_bonuses_.emplace(&group, bonus); }

	/**
	 * @brief Get a container with groups that add a special bonus to skills
	 * @return std::set of group names
	 */
	const std::set<const SkillGroupData*> skillGroupsWithSpecialBonus() const {
		auto keys = std::views::keys(skill_group_special_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Check if a group adds a special bonus to skills
	 * @param group SkillGroupData group to check
	 * @return `true` if the group has a bonus
	 * @return `false` if the group does not has a bonus
	 */
	bool isSpecialBonusSkillGroup(const SkillGroupData& group) const {
		for (auto& key : std::views::keys(skill_group_special_bonuses_)) {
			if (key->id() == group.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the special bonus for a group
	 * @param group SkillGroupData group to get the bonus for
	 * @return bonus for the group
	 */
	int skillGroupSpecialBonus(const SkillGroupData& group) const {
		return (isSpecialBonusSkillGroup(group) ? skill_group_special_bonuses_.at(&group) : 0);
	}

	/**
	 * @brief Add a bonus to all skills in a group
	 * @param group Name of the group
	 * @param bonus bonus value
	 */
	void addSkillGroupProfessionBonus(const SkillGroupData& group, int bonus) { skill_group_profession_bonuses_.emplace(&group, bonus); }

	/**
	 * @brief Get a container with the names of groups that have bonuses
	 * @return std::set of group names
	 */
	const std::set<const SkillGroupData*> skillGroupsWithProfessionBonus() const {
		auto keys = std::views::keys(skill_group_profession_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Check if a group has a bonus
	 * @param group SkillGroupData group to check
	 * @return `true` if the group has a bonus
	 * @return `false` if the group does not has a bonus
	 */
	bool isProfessionBonusSkillGroup(const SkillGroupData& group) const { 
		for (auto& key : std::views::keys(skill_group_profession_bonuses_)) { 
			if (key->id() == group.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the bonus for a group
	 * @param group SkillGroupData group to get the bonus for
	 * @return bonus for the group
	 */
	int skillGroupProfessionBonus(const SkillGroupData& group) const {
		return (isProfessionBonusSkillGroup(group) ? skill_group_profession_bonuses_.at(&group) : 0);
	}

	/**
	 * @brief Set the development type for a skill
	 * @param skill SubcategoriedSkillData to set the development type for
	 * @param type SkillDevelopmentType::Type value to set
	 */
	void setSkillDevelopmentType(SubcategoriedSkillData skill, SkillDevelopmentType::Type type) {
		if (isSkillDevelopmentTypeSet(skill.skillData(), skill.subcategory())) {
			throw InvalidSkillDevelopment("There is already a development set for skill " + skill.id());
		}
		skill_development_types_.emplace(std::move(skill), type); 
	}

	/**
	 * @brief Get the development type for a skill
	 * @param skill SubcategoriedSkillData to get the development type for
	 * @return SkillDevelopmentType::Type value
	 */
	SkillDevelopmentType::Type skillDevelopmentType(const SubcategoriedSkillData& skill) const { 
		for (auto& key : skill_development_types_) {
			if (key.first.id() == skill.id()) return key.second;
		}
		return SkillDevelopmentType::kStandard;
	}

	/**
	 * @brief Get the development type for a skill
	 * @param skill SubcategoriedSkillData to get the development type for
	 * @param subcategory optional subcategory of @a skill
	 * @return SkillDevelopmentType::Type value
	 */
	SkillDevelopmentType::Type skillDevelopmentType(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		return skillDevelopmentType(SubcategoriedSkillData(skill, subcategory));
	}

	/**
	 * @brief Gets a container with the skill that the profession has a development type set for
	 * @return std::set of SubcategoriedSkillData with the development type set
	 */
	const std::set<SubcategoriedSkillData> skillsWithSkillDevelopmentType() const {
		std::set<SubcategoriedSkillData> ret{};
		for (auto& key : skill_development_types_) {
			const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if there is a development type set for a skill
	 * @param skill SkillData to check
	 * @param subcategory optional subcategory of @a skill
	 * @return `true` if the skill has a development type set
	 * @return `false` if the skill does not have a development type set
	 */
	bool isSkillDevelopmentTypeSet(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& key : std::views::keys(skill_development_types_)) {
			if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add skill development type for all skills in a category
	 * @param category SkillCategoryData to add a skill development type for
	 * @param type SkillDevelopmentType::Type value to set
	 */
	void addSkillCategorySkillDevelopmentType(const SkillCategoryData& category, SkillDevelopmentType::Type type) { skill_category_skill_development_types_.emplace(&category, type); }

	/**
	 * @brief Get a container of all the skill categories with a SkillDevelopmentType
	 * @return std::set of categories with a SkillDevelopmentType
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithSkillDevelopmentType() const {
		auto keys = std::views::keys(skill_category_skill_development_types_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Get whether a skill category has a SkillDevelopmentType
	 * @param category SkillCategoryData to check
	 * @return `true` if the category has a SkillDevelopmentType
	 * @return `false` if the category does not have a SkillDevelopmentType
	 */
	bool isSkillDevelopmentTypeSkillCategory(const SkillCategoryData& category) const {
		for (auto& cat : std::views::keys(skill_category_skill_development_types_)) {
			if (cat->id() == category.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the SkillDevelopmentType a category has
	 * @param category SkillCategoryData to check
	 * @return SkillDevelopmentType type @a category has
	 */
	SkillDevelopmentType::Type skillCategorySkillDevelopmentType(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_skill_development_types_)) {
			if (cat->id() == category.id()) return skill_category_skill_development_types_.at(cat);
		}
		return SkillDevelopmentType::kStandard;
	}

	/**
	 * @brief Add a development type to all skills in a group
	 * @param group SkillGroupData group to add
	 * @param type SkillDevelopmentType::Type to set
	 */
	void addSkillGroupSkillDevelopmentType(const SkillGroupData& group, SkillDevelopmentType::Type type) { skill_group_skill_development_types_.emplace(&group, type); }

	/**
	 * @brief Get a container with the names of groups that have skill development type changes
	 * @return std::set of groups
	 */
	const std::set<const SkillGroupData*> skillGroupsWithSkillDevelopmentType() const {
		auto keys = std::views::keys(skill_group_skill_development_types_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Check if a skills in a group have a development type set
	 * @param group SkillGroupData to check
	 * @return `true` if the skills in the group are modified
	 * @return `false` if the skills in the group are not modified
	 */
	bool isSkillDevelopmentTypeSkillGroup(const SkillGroupData& group) const {
		for (auto& key : std::views::keys(skill_group_skill_development_types_)) {
			if (key->id() == group.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the development type to set for all skills in a group
	 * @param group SkillGroupData group to get the skill development type for
	 * @return SkillDevelopmentType::Type for skill in the group
	 */
	SkillDevelopmentType::Type skillGroupSkillDevelopmentType(const SkillGroupData& group) const {
		for (auto& key : std::views::keys(skill_group_skill_development_types_)) {
			if (key->id() == group.id()) return skill_group_skill_development_types_.at(key);
		}
		return SkillDevelopmentType::kStandard;
	}

	/**
	 * @brief Add special bonus for all skills in a category
	 * @param category SkillCategoryData to add a bonus for
	 * @param bonus int bonus value
	 */
	void addSkillCategorySpecialBonus(const SkillCategoryData& category, int bonus) { skill_category_special_bonuses_.emplace(&category, bonus); }

	/**
	 * @brief Get a container of all the skill categories with a special bonus
	 * @return std::set of categories with a bonus
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithSpecialBonus() const {
		auto keys = std::views::keys(skill_category_special_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Get whether a skill category has a special bonus
	 * @param category SkillCategoryData to check
	 * @return `true` if the category has a bonus
	 * @return `false` if the category does not have a bonus
	 */
	bool isSpecialBonusSkillCategory(const SkillCategoryData& category) const {
		for (auto& cat : std::views::keys(skill_category_special_bonuses_)) {
			if (cat->id() == category.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the special bonus a category has
	 * @param category SkillCategoryData to check
	 * @return int bonus @a category has
	 */
	int skillCategorySpecialBonus(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_special_bonuses_)) {
			if (cat->id() == category.id()) return skill_category_special_bonuses_.at(cat);
		}
		return 0;
	}

	/**
	 * @brief Add profession bonus for all skills in a category
	 * @param category SkillCategoryData to add a bonus for
	 * @param bonus int bonus value
	 */
	void addSkillCategoryProfessionBonus(const SkillCategoryData& category, int bonus) { skill_category_profession_bonuses_.emplace(&category, bonus); }

	/**
	 * @brief Get a container of all the skill categories with a profession bonus
	 * @return std::set of categories with a bonus
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithProfessionBonus() const {
		auto keys = std::views::keys(skill_category_profession_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Get whether a skill category has a profession bonus
	 * @param category SkillCategoryData to check
	 * @return `true` if the category has a bonus
	 * @return `false` if the category does not have a bonus
	 */
	bool isProfessionBonusSkillCategory(const SkillCategoryData& category) const {
		for (auto& cat : std::views::keys(skill_category_profession_bonuses_)) {
			if (cat->id() == category.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the profession bonus a category has
	 * @param category SkillCategoryData to check
	 * @return int bonus @a category has
	 */
	int skillCategoryProfessionBonus(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_profession_bonuses_)) {
			if (cat->id() == category.id()) return skill_category_profession_bonuses_.at(cat);
		}
		return 0;
	}

	/**
	 * @brief Set the bonus the profession has with a skill
	 * @param skill SubcategoriedSkillData to set the bonus for
	 * @param bonus int bonus value
	 */
	void setSkillBonus(SubcategoriedSkillData skill, int bonus) {
		if (isBonusSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillBonus("There is already a bonus set for skill " + skill.id());
		skill_bonuses_.emplace(std::move(skill), bonus);
	}

	/**
	 * @brief Sets the object's skill bonuses from the provided map, replacing any existing bonuses.
	 * @param bonuses A map from SubcategoriedSkillData to int containing skill bonuses. The map is assigned to the object's internal skill_bonuses_ member, replacing its previous contents.
	 */
	void setSkillBonuses(const std::map<SubcategoriedSkillData, int>& bonuses) {
		skill_bonuses_ = bonuses;
	}

	/**
	 * @brief Get the bonus that the profession provides to a skill
	 * @param skill SubcategoriedSkillData to get the bonus for
	 * @return bonus value
	 */
	int skillBonus(const SubcategoriedSkillData& skill) const {
		for (auto& key : skill_bonuses_) {
			if (key.first.id() == skill.id()) return key.second;
		}
		return 0;
	}

	/**
	 * @brief Get the bonus that the profession provides to a skill
	 * @param skill SkillData to get the bonus for
	 * @param subcategory optional subcategory of @a skill
	 * @return bonus value
	 */
	int skillBonus(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		return skillBonus(SubcategoriedSkillData(skill, subcategory));
	}

	/**
	 * @brief Gets a container with the skill that the profession has a bonus for
	 * @return std::set of SkillData with bonuses
	 */
	const std::set<SubcategoriedSkillData> skillsWithBonus() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& key : skill_bonuses_) {
			const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Get a container with the skills that the profession has a bonus for
	 * @return std::map of SubcategoriedSkillData and bonus value
	 */
	const std::map<SubcategoriedSkillData, int>& skillBonuses() const { return skill_bonuses_; }

	/**
	 * @brief Check if there is a professional bonus for a skill
	 * @param skill SkillData to check
	 * @param subcategory optional subcategory of @a skill
	 * @return `true` if the skill has a bonus
	 * @return `false` if the skill does not have a bonus
	 */
	bool isBonusSkill(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& key : std::views::keys(skill_bonuses_)) {
			if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add the cost in development points to develop one or more ranks in a skill category
	 * 
	 * @attention @a cost is moved during this operation so its content is undefined afterwards.
	 * 
	 * @param category SkillCategoryData category to add the costs for
	 * @param cost SkillDevelopmentCost cost to develop ranks
	 */
	void addSkillCategoryDevelopmentCost(const SkillCategoryData& category, SkillDevelopmentCost cost) { skill_category_development_costs_.emplace(&category, std::move(cost)); }

	/**
	 * @brief Get a container of all the skill categories with a development cost
	 * @return std::set of categories with a development cost
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithCost() const {
		auto keys = std::views::keys(skill_category_development_costs_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Get the cost to develop ranks in a skill category
	 * @param category SkillCategoryData to get the costs for
	 * @return SkillDevelopmentCost costs to develop ranks
	 * @throws InvalidCategoryDevelopmentCost if there are no costs for the category
	 */
	const SkillDevelopmentCost& skillCategoryDevelopmentCost(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_development_costs_)) {
			if (cat->id() == category.id()) return skill_category_development_costs_.at(cat);
		}
		throw InvalidCategoryDevelopmentCost("There is no skill development cost for the category: " + category.id());
	}

private:
	std::string name_{}; /**< Name of the profession */
	std::string description_{}; /**< General description of the profession */
	std::optional<const BookData*> book_{ std::nullopt }; /**< Book that the profession is described in */
	SpellUserType::Type spell_user_type_{ SpellUserType::kNone }; /**< Spell user type */
	std::set< RealmType::Type> realms_{};/**< Realm(s) that the profession draws power from */
	std::vector<StatType::Type> stats_{}; /**< Stats providing a bonus to the profession */
	std::vector<GameRuleDataChoice<SpellListData>> base_spell_list_choices_{}; /**< Set of spell lists that the profession base lists should be chosen from */

	// Skill bonuses
	std::map<SubcategoriedSkillData, int> skill_bonuses_{}; /** bonus to individual skills */
	std::map<const SkillCategoryData*, int> skill_category_profession_bonuses_{}; /** profession bonus to skill categories */
	std::map<const SkillCategoryData*, int> skill_category_special_bonuses_{}; /** special bonus to skills in a category */
	std::map<const SkillGroupData*, int> skill_group_profession_bonuses_{}; /** profession bonus to skills in a group */
	std::map<const SkillGroupData*, int> skill_group_special_bonuses_{}; /** special bonus to skills in a group */

	// Skill development types
	std::map<SubcategoriedSkillData, SkillDevelopmentType::Type> skill_development_types_{}; /** Skill with their development type changed */
	std::map<const SkillCategoryData*, SkillDevelopmentType::Type> skill_category_skill_development_types_{}; /** Skill categories that all skills within have their development type changed */
	std::map<const SkillGroupData*, SkillDevelopmentType::Type> skill_group_skill_development_types_{}; /** Skill groups that all skills within have their development type changed */

	// Skill development type choices
	std::map<GameRuleDataChoice<SkillData>, SkillDevelopmentType::Type> skill_subcategory_skill_development_type_choices_{}; /** Set of skills the character may select one or more subategories from to change their development type */
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, SkillDevelopmentType::Type> skill_development_type_choices_{}; /** Set of skills that the character may select one or more from to change their development type */
	std::map<GameRuleDataChoice<SkillCategoryData>, SkillDevelopmentType::Type> skill_category_skill_development_type_choices_{}; /** Set of skill categories the character may select one or more skills from to change their development type */
	std::map<GameRuleDataChoice<SkillGroupData>, SkillDevelopmentType::Type> skill_group_skill_development_type_choices_{}; /** Set of skill groups the character may select one or more skills from to change their development type */

	std::map<const SkillCategoryData*, SkillDevelopmentCost> skill_category_development_costs_{}; /** Cost to purchase ranks for a skill category */
};