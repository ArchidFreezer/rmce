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

namespace rm {

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
		void setSpellUserType(rule::enums::SpellUserType::Type spell_user_type) { spell_user_type_ = spell_user_type; }

		/**
		 * @brief Get the type of spell user the profession is
		 * @return SpellUserType::Type spell user type
		 */
		rule::enums::SpellUserType::Type spellUserType() const { return spell_user_type_; }

		/**
		 * @brief Add a realm to those the profession draws power from
		 * @param realm RealmType::Type profession power realm
		 */
		void addRealm(rule::enums::RealmType::Type realm) {
			realms_.emplace(realm);
		}

		/**
		 * @brief Add realms to those the profession draws power from
		 * @param realms Set of RealmType::Type profession power realms
		 */
		void setRealms(std::set<rule::enums::RealmType::Type> realms) { realms_ = std::move(realms); }

		/**
		 * @brief Get whether the profession draws power from a power realm
		 * @param realm RealmType::Type power realm
		 * @return `true` if the profession draws power from the realm
		 * @return `false` if the profession does not draw power from the realm
		 */
		bool isRealm(rule::enums::RealmType::Type realm) const { return (realms_.find(realm) != realms_.end()); }

		/**
		 * @brief Get the realms that the profession draws power from
		 * @return td::set<RealmType::Type> power realms
		 */
		const std::set<rule::enums::RealmType::Type> realms() { return realms_; }

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
		int addStat(rule::enums::StatType::Type stat);

		/**
		 * @brief Set the stats that apply stat bonuses to the profession
		 *
		 * There are 3 stats associated with each profession and duplicates are allowed. This function will replace any existing stats with the new list.
		 *
		 * @param stats Vector of stats whose bonus should be applied to the profession
		 * @throws TooManyStatsException if attempting to set more than 3 stats
		 * @see setUseRealmStats()
		 */
		void setStats(std::vector<rule::enums::StatType::Type> stats) { stats_ = std::move(stats); }

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
		bool removeStat(rule::enums::StatType::Type stat);

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
		const std::vector<rule::enums::StatType::Type>& stats() const { return stats_; }

		/**
	 * @brief Add a choice that defines base spell lists that the character has to choose from
	 * @param choice GameRuleDataChoice choice definition
	 */
		void addBaseSpellListChoice(GameRuleDataChoice<SpellListData> choice) { base_spell_list_choices_.emplace(std::move(choice)); }

		/**
		 * @brief Set the choices that defines base spell lists that the character has to choose from
		 *
		 * This will replace any existing choices with the new list.
		 * @param choices set of GameRuleDataChoice choice definitions
		 */
		void setBaseSpellListChoices(std::set<GameRuleDataChoice<SpellListData>> choices) { base_spell_list_choices_ = std::move(choices); }

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
		 * @return set of GameRuleDataChoice objects with the choices to be made
		 */
		const std::set<GameRuleDataChoice<SpellListData>>& baseSpellListChoices() const { return base_spell_list_choices_; }

		/**
		 * @brief Add a choice that defines the skill that a character may select one or more from to become everyman
		 * @param choice GameRuleDataChoice choice definition
		 * @param type SkillDevelopmentType::Type type choices will have
		 */
		void addSkillDevelopmentTypeChoice(GameRuleDataChoice<SubcategoriedSkillData> choice, rule::enums::SkillDevelopmentType::Type type) { skill_development_type_choices_.emplace(std::move(choice), type); }

		/**
		 * @brief Sets the skill development type choices for skills.
		 * @param choices A map associating subcategoried skill data choices with their corresponding skill development types.
		 */
		void setSkillDevelopmentTypeChoices(std::map<GameRuleDataChoice<SubcategoriedSkillData>, rule::enums::SkillDevelopmentType::Type> choices) { skill_development_type_choices_ = std::move(choices); }

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
		const std::map<GameRuleDataChoice<SubcategoriedSkillData>, rule::enums::SkillDevelopmentType::Type>& skillDevelopmentTypeChoices() const { return skill_development_type_choices_; }

		/**
		 * @brief Add a choice that defines the skill that a character may select one or more subcategories from to become everyman
		 * @param choice GameRuleDataChoice choice definition
		 * @param type SkillDevelopmentType::Type type choices will have
		 */
		void addSkillSubcategoryDevelopmentTypeChoice(GameRuleDataChoice<SkillData> choice, rule::enums::SkillDevelopmentType::Type type) { skill_subcategory_skill_development_type_choices_.emplace(std::move(choice), type); }

		/**
		 * @brief Sets the skill development type choices for skill subcategories.
		 * @param choices A map associating skill data choices with their corresponding skill development types.
		 */
		void setSkillSubcategoryDevelopmentTypeChoices(std::map<GameRuleDataChoice<SkillData>, rule::enums::SkillDevelopmentType::Type> choices) { skill_subcategory_skill_development_type_choices_ = std::move(choices); }

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
		const std::map<GameRuleDataChoice<SkillData>, rule::enums::SkillDevelopmentType::Type>& skillSubcategoryDevelopmentTypeChoices() const { return skill_subcategory_skill_development_type_choices_; }

		/**
		 * @brief Add a choice that defines the skill categories that a character may select one or more skills from to change their development type
		 * @param choice GameRuleDataChoice choice definition
		 * @param type SkillDevelopmentType::Type type choices will have
		 */
		void addSkillCategorySkillDevelopmentTypeChoice(GameRuleDataChoice<SkillCategoryData> choice, rule::enums::SkillDevelopmentType::Type type) { skill_category_skill_development_type_choices_.emplace(std::move(choice), type); }

		/**
		 * @brief Sets the skill development type choices for skill categories.
		 * @param choices A map associating skill category data choices with their corresponding skill development types.
		 */
		void setSkillCategorySkillDevelopmentTypeChoices(std::map<GameRuleDataChoice<SkillCategoryData>, rule::enums::SkillDevelopmentType::Type> choices) { skill_category_skill_development_type_choices_ = std::move(choices); }

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
		const std::map<GameRuleDataChoice<SkillCategoryData>, rule::enums::SkillDevelopmentType::Type>& skillCategorySkillDevelopmentTypeChoices() const { return skill_category_skill_development_type_choices_; }

		/**
		 * @brief Add a choice that defines the skill groups that a character may select one or more skills from to change their development type
		 * @param choice GameRuleDataChoice choice definition
		 * @param type SkillDevelopmentType::Type type choices will have
		 */
		void addSkillGroupSkillDevelopmentTypeChoice(GameRuleDataChoice<SkillGroupData> choice, rule::enums::SkillDevelopmentType::Type type) { skill_group_skill_development_type_choices_.emplace(std::move(choice), type); }

		/**
		 * @brief Sets the skill development type choices for skill groups.
		 * @param choices A map associating skill group data choices with their corresponding skill development types.
		 */
		void setSkillGroupSkillDevelopmentTypeChoices(std::map<GameRuleDataChoice<SkillGroupData>, rule::enums::SkillDevelopmentType::Type> choices) { skill_group_skill_development_type_choices_ = std::move(choices); }

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
		const std::map<GameRuleDataChoice<SkillGroupData>, rule::enums::SkillDevelopmentType::Type>& skillGroupSkillDevelopmentTypeChoices() const { return skill_group_skill_development_type_choices_; }

		/**
		 * @brief Add a special bonus to all skills in a group
		 * @param group Name of the group
		 * @param bonus bonus value
		 */
		void addSkillGroupSpecialBonus(const SkillGroupData& group, int bonus) { skill_group_special_bonuses_.emplace(&group, bonus); }

		/**
		 * @brief Sets the object's skill-group special bonuses from the provided map.
		 *
		 * The map is stored in the object's internal skill_group_special_bonuses_ (the contents are transferred into internal storage; depending on overloads/qualifiers this may be moved or copied).
		 * @param bonuses A map whose keys are pointers to SkillGroupData and whose values are integer bonus amounts.
		 */
		void setSkillGroupSpecialBonuses(std::map<const SkillGroupData*, int> bonuses) { skill_group_special_bonuses_ = std::move(bonuses); }

		/**
		 * @brief Get a container with groups that the profession has a special bonus for
		 * @return std::map of const SkillGroupData* and bonus value
		 */
		const std::map<const SkillGroupData*, int>& skillGroupSpecialBonuses() const { return skill_group_special_bonuses_; }

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
		 * @brief Sets the object's skill-group profession bonuses from the provided map.
		 *
		 * The map is stored in the object's internal skill_group_profession_bonuses_ (the contents are transferred into internal storage; depending on overloads/qualifiers this may be moved or copied).
		 * @param bonuses A map whose keys are pointers to SkillGroupData and whose values are integer bonus amounts.
		 */
		void setSkillGroupProfessionBonuses(std::map<const SkillGroupData*, int> bonuses) { skill_group_profession_bonuses_ = std::move(bonuses); }

		/**
		 * @brief Get a container with groups that that the profession has a bonus for
		 * @return std::map of const SkillGroupData* and bonus value
		 */
		const std::map<const SkillGroupData*, int>& skillGroupProfessionBonuses() const { return skill_group_profession_bonuses_; }

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
		void setSkillDevelopmentType(SubcategoriedSkillData skill, rule::enums::SkillDevelopmentType::Type type) {
			if (isSkillDevelopmentTypeSet(skill.skillData(), skill.subcategory())) {
				throw InvalidSkillDevelopment("There is already a development set for skill " + skill.id());
			}
			skill_development_types_.emplace(std::move(skill), type);
		}

		/**
		 * @brief Sets the skill development types mapping.
		 * @param types A map associating subcategorized skill data with their corresponding skill development types.
		 */
		void setSkillDevelopmentTypes(std::map<SubcategoriedSkillData, rule::enums::SkillDevelopmentType::Type> types) { skill_development_types_ = std::move(types); }

		/**
		 * @brief Gets a reference to the map of skill development types indexed by subcategorized skill data.
		 * @return A reference to the map containing skill development types.
		 */
		std::map<SubcategoriedSkillData, rule::enums::SkillDevelopmentType::Type>& skillDevelopmentTypes() { return skill_development_types_; }

		/**
		 * @brief Get the development type for a skill
		 * @param skill SubcategoriedSkillData to get the development type for
		 * @return SkillDevelopmentType::Type value
		 */
		rule::enums::SkillDevelopmentType::Type skillDevelopmentType(const SubcategoriedSkillData& skill) const {
			for (auto& key : skill_development_types_) {
				if (key.first.id() == skill.id()) return key.second;
			}
			return rule::enums::SkillDevelopmentType::kStandard;
		}

		/**
		 * @brief Get the development type for a skill
		 * @param skill SubcategoriedSkillData to get the development type for
		 * @param subcategory optional subcategory of @a skill
		 * @return SkillDevelopmentType::Type value
		 */
		rule::enums::SkillDevelopmentType::Type skillDevelopmentType(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
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
		void addSkillCategorySkillDevelopmentType(const SkillCategoryData& category, rule::enums::SkillDevelopmentType::Type type) { skill_category_skill_development_types_.emplace(&category, type); }

		/**
		 * @brief Sets the internal mapping from skill categories to their corresponding skill development types by assigning the provided map.
		 *
		 * The function stores this mapping in the object's internal state (the map will be assigned from the provided argument).
		 * @param types A map that associates pointers to SkillCategoryData (keys) with SkillDevelopmentType::Type values.
		 */
		void setSkillCategorySkillDevelopmentTypes(std::map<const SkillCategoryData*, rule::enums::SkillDevelopmentType::Type> types) { skill_category_skill_development_types_ = std::move(types); }

		/**
		 * @brief Get a container with the skill categories that the profession has a skill development type for
		 * @return std::map of const SkillCategoryData* and SkillDevelopmentType::Type
		 */
		const std::map<const SkillCategoryData*, rule::enums::SkillDevelopmentType::Type>& skillCategorySkillDevelopmentTypes() const { return skill_category_skill_development_types_; }

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
		rule::enums::SkillDevelopmentType::Type skillCategorySkillDevelopmentType(const SkillCategoryData& category) {
			for (auto& cat : std::views::keys(skill_category_skill_development_types_)) {
				if (cat->id() == category.id()) return skill_category_skill_development_types_.at(cat);
			}
			return rule::enums::SkillDevelopmentType::kStandard;
		}

		/**
		 * @brief Add a development type to all skills in a group
		 * @param group SkillGroupData group to add
		 * @param type SkillDevelopmentType::Type to set
		 */
		void addSkillGroupSkillDevelopmentType(const SkillGroupData& group, rule::enums::SkillDevelopmentType::Type type) { skill_group_skill_development_types_.emplace(&group, type); }

		/**
		 * @brief Sets the internal mapping from skill groups to their corresponding skill development types by assigning the provided map.
		 * @param types A map that associates pointers to SkillGroupData (keys) with SkillDevelopmentType::Type values. The function stores this mapping in the object's internal state (the map will be assigned from the provided argument).
		 */
		void setSkillGroupSkillDevelopmentTypes(std::map<const SkillGroupData*, rule::enums::SkillDevelopmentType::Type> types) { skill_group_skill_development_types_ = std::move(types); }

		/**
		 * @brief Gets a reference to container with the skill group skill development types
		 * @return A map that associates pointers to SkillGroupData (keys) with SkillDevelopmentType::Type values.
		 */
		const std::map<const SkillGroupData*, rule::enums::SkillDevelopmentType::Type>& skillGroupSkillDevelopmentTypes() { return skill_group_skill_development_types_; }

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
		rule::enums::SkillDevelopmentType::Type skillGroupSkillDevelopmentType(const SkillGroupData& group) const {
			for (auto& key : std::views::keys(skill_group_skill_development_types_)) {
				if (key->id() == group.id()) return skill_group_skill_development_types_.at(key);
			}
			return rule::enums::SkillDevelopmentType::kStandard;
		}

		/**
		 * @brief Add special bonus for all skills in a category
		 * @param category SkillCategoryData to add a bonus for
		 * @param bonus int bonus value
		 */
		void addSkillCategorySpecialBonus(const SkillCategoryData& category, int bonus) { skill_category_special_bonuses_.emplace(&category, bonus); }

		/**
		 * @brief Sets the object's skill category special bonuses from the provided map, replacing any existing bonuses.
		 * @param bonuses A map from const SkillCategoryData* to int containing skill category special bonuses. The map is assigned to the object's internal skill_category_special_bonuses_ member, replacing its previous contents.
		 */
		void setSkillCategorySpecialBonuses(std::map<const SkillCategoryData*, int> bonuses) { skill_category_special_bonuses_ = std::move(bonuses); }

		/**
		 * @brief Get a container with the skill categories that the profession has a special bonus for
		 * @return std::map of const SkillCategoryData* and bonus value
		 */
		const std::map<const SkillCategoryData*, int>& skillCategorySpecialBonuses() const { return skill_category_special_bonuses_; }

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
		 * @brief Sets the object's skill category profession bonuses from the provided map, replacing any existing bonuses.
		 * @param bonuses A map from const SkillCategoryData* to int containing skill category profession bonuses. The map is assigned to the object's internal skill_category_profession_bonuses_ member, replacing its previous contents.
		 */
		void setSkillCategoryProfessionBonuses(std::map<const SkillCategoryData*, int> bonuses) { skill_category_profession_bonuses_ = std::move(bonuses); }

		/**
		 * @brief Get a container with the skill categories that the profession has a bonus for
		 * @return std::map of const SkillCategoryData* and bonus value
		 */
		const std::map<const SkillCategoryData*, int>& skillCategoryProfessionBonuses() const { return skill_category_profession_bonuses_; }

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
		void setSkillBonuses(std::map<SubcategoriedSkillData, int> bonuses) { skill_bonuses_ = std::move(bonuses); }

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
		rule::enums::SpellUserType::Type spell_user_type_{ rule::enums::SpellUserType::kNone }; /**< Spell user type */
		std::set< rule::enums::RealmType::Type> realms_{};/**< Realm(s) that the profession draws power from */
		std::vector<rule::enums::StatType::Type> stats_{}; /**< Stats providing a bonus to the profession */
		std::set<GameRuleDataChoice<SpellListData>> base_spell_list_choices_{}; /**< Set of spell lists that the profession base lists should be chosen from */

		// Skill bonuses
		std::map<SubcategoriedSkillData, int> skill_bonuses_{}; /** bonus to individual skills */
		std::map<const SkillCategoryData*, int> skill_category_profession_bonuses_{}; /** profession bonus to skill categories */
		std::map<const SkillCategoryData*, int> skill_category_special_bonuses_{}; /** special bonus to skills in a category */
		std::map<const SkillGroupData*, int> skill_group_profession_bonuses_{}; /** profession bonus to skills in a group */
		std::map<const SkillGroupData*, int> skill_group_special_bonuses_{}; /** special bonus to skills in a group */

		// Skill development types
		std::map<SubcategoriedSkillData, rule::enums::SkillDevelopmentType::Type> skill_development_types_{}; /** Skill with their development type changed */
		std::map<const SkillCategoryData*, rule::enums::SkillDevelopmentType::Type> skill_category_skill_development_types_{}; /** Skill categories that all skills within have their development type changed */
		std::map<const SkillGroupData*, rule::enums::SkillDevelopmentType::Type> skill_group_skill_development_types_{}; /** Skill groups that all skills within have their development type changed */

		// Skill development type choices
		std::map<GameRuleDataChoice<SkillData>, rule::enums::SkillDevelopmentType::Type> skill_subcategory_skill_development_type_choices_{}; /** Set of skills the character may select one or more subategories from to change their development type */
		std::map<GameRuleDataChoice<SubcategoriedSkillData>, rule::enums::SkillDevelopmentType::Type> skill_development_type_choices_{}; /** Set of skills that the character may select one or more from to change their development type */
		std::map<GameRuleDataChoice<SkillCategoryData>, rule::enums::SkillDevelopmentType::Type> skill_category_skill_development_type_choices_{}; /** Set of skill categories the character may select one or more skills from to change their development type */
		std::map<GameRuleDataChoice<SkillGroupData>, rule::enums::SkillDevelopmentType::Type> skill_group_skill_development_type_choices_{}; /** Set of skill groups the character may select one or more skills from to change their development type */

		std::map<const SkillCategoryData*, SkillDevelopmentCost> skill_category_development_costs_{}; /** Cost to purchase ranks for a skill category */
	};

} // namespace rm