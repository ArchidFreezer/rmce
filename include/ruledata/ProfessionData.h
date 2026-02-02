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
#include <SpellListData.h>
#include <StatType.h>
#include <SubcategoriedSkillData.h>

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
	 * @brief Add a skill that is considered everyman for the profession
	 * @param skill SubcategoriedSkillData unique pointer to the everyman skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addEverymanSkill(SubcategoriedSkillData skill) {
		if (isOccupationalSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as everyman as it is already set as occupational");
		if (isRestrictedSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as everyman as it is already set as restricted");
		if (isStandardSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as everyman as it is already set as standard");
		if (isEverymanSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as everyman as it is already defined");
		everyman_skills_.insert(std::move(skill));
	}

	/**
	 * @brief Get a container of the skills that are considered everyman
	 * @return std::set container of everyman skills
	 * @see SkillDevelopmentType
	 */
	const std::set<SubcategoriedSkillData> everymanSkills() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& skill : everyman_skills_) {
			const SubcategoriedSkillData data(skill.skillData(), skill.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if a skill is everyman for the profession
	 * @param other Skill to check
	 * @param subcategory optional subcategory of @a other
	 * @return `true` if the skill is everyman
	 * @return `false` if the skill is not everyman
	 * @see SkillDevelopmentType
	 */
	bool isEverymanSkill(const SkillData& other, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& skill : everyman_skills_) {
			if (skill.skillData().id() == other.id() && (subcategory ? subcategory.value() == skill.subcategory() : !skill.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a skill that is considered occupational for the profession
	 * @param skill SubcategoriedSkillData unique pointer to the occupational skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addOccupationalSkill(SubcategoriedSkillData skill) {
		if (isEverymanSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as occupational as it is already set as everyman");
		if (isRestrictedSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as occupational as it is already set as restricted");
		if (isStandardSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as occupational as it is already set as standard");
		if (isOccupationalSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as occupational as it is already defined");
		occupational_skills_.insert(std::move(skill));
	}

	/**
	 * @brief Get a container of the skills that are considered occupational
	 * @return std::set container of occupational skills
	 * @see SkillDevelopmentType
	 */
	const std::set<SubcategoriedSkillData> occupationalSkills() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& skill : occupational_skills_) {
			const SubcategoriedSkillData data(skill.skillData(), skill.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if a skill is occupational for the profession
	 * @param other Skill to check
	 * @param subcategory optional subcategory of @a other
	 * @return `true` if the skill is occupational
	 * @return `false` if the skill is not occupational
	 * @see SkillDevelopmentType
	 */
	bool isOccupationalSkill(const SkillData& other, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& skill : occupational_skills_) {
			if (skill.skillData().id() == other.id() && (subcategory ? subcategory.value() == skill.subcategory() : !skill.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a skill that is considered restricted for the profession
	 * @param skill SubcategoriedSkillData unique pointer to the restricted skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addRestrictedSkill(SubcategoriedSkillData skill) {
		if (isEverymanSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as restricted as it is already set as everyman");
		if (isOccupationalSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as restricted as it is already set as occupational");
		if (isStandardSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as restricted as it is already set as standard");
		if (isRestrictedSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as restricted as it is already defined");
		restricted_skills_.insert(std::move(skill));
	}

	/**
	 * @brief Get a container of the skills that are considered restricted
	 * @return std::set container of restricted skills
	 */
	const std::set<SubcategoriedSkillData> restrictedSkills() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& skill : restricted_skills_) {
			const SubcategoriedSkillData data(skill.skillData(), skill.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if a skill is restricted for the profession
	 * @param other Skill to check
	 * @param subcategory optional subcategory of @a other
	 * @return `true` if the skill is restricted
	 * @return `false` if the skill is not restricted
	 * @see SkillDevelopmentType
	 */
	bool isRestrictedSkill(const SkillData& other, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& skill : restricted_skills_) {
			if (skill.skillData().id() == other.id() && (subcategory ? subcategory.value() == skill.subcategory() : !skill.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a skill that is considered standard for the profession
	 * 
	 * Some skills by default are restricted so this is used if a profession removes the restricted status and allows the
	 * skill to be developed as normal.
	 * 
	 * @param skill SubcategoriedSkillData unique pointer to the standard skill
	 * @throw InvalidSkillDevelopment If the skill is set as standard
	 * @see SkillDevelopmentType
	 */
	void addStandardSkill(SubcategoriedSkillData skill) {
		if (isEverymanSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as standard as it is already set as everyman");
		if (isOccupationalSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as standard as it is already set as occupational");
		if (isRestrictedSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as standard as it is already set as restricted");
		if (isStandardSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as standard as it is already defined");
		standard_skills_.insert(std::move(skill));
	}

	/**
	 * @brief Get a container of the skills that are considered standard
	 * @return std::set container of standard skills
	 */
	const std::set<SubcategoriedSkillData> standardSkills() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& skill : standard_skills_) {
			const SubcategoriedSkillData data(skill.skillData(), skill.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if a skill is standard for the profession
	 * @param other Skill to check
	 * @param subcategory optional subcategory of @a other
	 * @return `true` if the skill is standard
	 * @return `false` if the skill is not standard
	 * @see SkillDevelopmentType
	 */
	bool isStandardSkill(const SkillData& other, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& skill : standard_skills_) {
			if (skill.skillData().id() == other.id() && (subcategory ? subcategory.value() == skill.subcategory() : !skill.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a choice that defines the skill that a character may select one or more from to become everyman
	 * @param choice GameRuleDataChoice choice definition
	 * @see SkillDevelopmentType
	 */
	void addEverymanSkillChoice(GameRuleDataChoice<SkillData> choice) { everyman_skill_choices_.push_back(std::move(choice)); }

	/**
	 * @brief Get the number of choices a character has to make regarding everyman skills
	 *
	 * During character development it is sometimes necessary to choose one or more skills to have
	 * everyman development. This returns how many choices need to be made. This is not the number of options to be selected
	 * in a single choice.
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numEverymanSkillChoices() const { return everyman_skill_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skills that may be made everyman
	 * @return vector of GameRuleDataChoice objects with the choices to be made
	 * @see SkillDevelopmentType
	 */
	const std::vector<GameRuleDataChoice<SkillData>>& everymanSkillChoices() const { return everyman_skill_choices_; }

	/**
	 * @brief Add a choice that defines the skill categories that a character may select one or more skills from to become everyman
	 * @param choice GameRuleDataChoice choice definition
	 * @see SkillDevelopmentType
	 */
	void addCategoryEverymanSkillChoice(GameRuleDataChoice<SkillCategoryData> choice) { category_everyman_skill_choices_.push_back(std::move(choice)); }

	/**
	 * @brief Get the number of choices a character has to make regarding everyman skills in skill cetegories
	 *
	 * During character development it is sometimes necessary to choose one or more skills from a skill category to have
	 * everyman development. This returns how many choices need to be made. This is not the number of options to be selected
	 * in a single choice.
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numCategoryEverymanSkillChoices() const { return category_everyman_skill_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skills within a skill
	 * category that may be made everyme skills
	 * @return vector of GameRuleDataChoice objects with the choices to be made
	 * @see SkillDevelopmentType
	 */
	const std::vector<GameRuleDataChoice<SkillCategoryData>>& categoryEverymanSkillChoices() const { return category_everyman_skill_choices_; }

	/**
	 * @brief Add a bonus to all skills in a group
	 * @param group Name of the group
	 * @param bonus bonus value
	 */
	void addSkillGroupBonus(std::string_view group, int bonus) { group_bonuses_.emplace(group, bonus); }

	/**
	 * @brief Get a container with the names of groups that have bonuses
	 * @return std::set of group names
	 */
	const std::set<std::string> skillGroupsWithBonus() const {
		auto keys = std::views::keys(group_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Check if a group has a bonus
	 * @param group_name name of the group
	 * @return `true` if the group has a bonus
	 * @return `false` if the group does not has a bonus
	 */
	bool isBonusSkillGroup(const std::string& group_name) const { return (group_bonuses_.find(group_name) != group_bonuses_.end()); }

	/**
	 * @brief Get the bonus for a group
	 * @param group_name name of the group to get the bonus for
	 * @return bonus for the group
	 */
	int skillGroupBonus(const std::string& group_name) const { return (isBonusSkillGroup(group_name) ? group_bonuses_.at(group_name) : 0); }

	/**
	 * @brief Add bonus for all skills in a category
	 * @param category SkillCategoryData to add a bonus for
	 * @param bonus int bonus value
	 */
	void addSkillCategoryBonus(const SkillCategoryData& category, int bonus) { skill_category_bonuses_.emplace(&category, bonus); }

	/**
	 * @brief Get a container of all the skill categories with a bonus
	 * @return std::set of categories with a bonus
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithBonus() const {
		auto keys = std::views::keys(skill_category_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Get whether a skill category has a bonus
	 * @param category SkillCategoryData to check
	 * @return `true` if the category has a bonus
	 * @return `false` if the category does not have a bonus
	 */
	bool isBonusSkillCategory(const SkillCategoryData& category) const {
		for (auto& cat : std::views::keys(skill_category_bonuses_)) {
			if (cat->id() == category.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the bonus a category has
	 * @param category SkillCategoryData to check
	 * @return int bonus @a category has
	 */
	int skillCategoryBonus(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_bonuses_)) {
			if (cat->id() == category.id()) return skill_category_bonuses_.at(cat);
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
	void addCategoryDevelopmentCost(const SkillCategoryData& category, SkillDevelopmentCost cost) { skill_category_development_costs_.emplace(&category, std::move(cost)); }

	/**
	 * @brief Get the cost to develop ranks in a skill category
	 * @param category SkillCategoryData to get the costs for
	 * @return SkillDevelopmentCost costs to develop ranks
	 * @throws InvalidCategoryDevelopmentCost if there are no costs for the category
	 */
	const SkillDevelopmentCost& categoryDevelopmentCost(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_development_costs_)) {
			if (cat->id() == category.id()) return skill_category_development_costs_.at(cat);
		}
		throw InvalidCategoryDevelopmentCost("There is no skill development cost for the category: " + category.id());
	}

private:
	std::string name_{}; /**< Name of the profession */
	std::string description_{}; /**< General description of the profession */
	std::optional<const BookData*> book_{ std::nullopt }; /**< Book that the profession is described in */
	std::set< RealmType::Type> realms_{};/**< Realm(s) that the profession draws power from */
	std::vector<StatType::Type> stats_{}; /**< Stats providing a bonus to the profession */
	std::vector<GameRuleDataChoice<SpellListData>> base_spell_list_choices_{}; /**< Set of spell lists that the profession base lists should be chosen from */
	std::set<SubcategoriedSkillData> everyman_skills_{}; /**< Skills that are considered everyman for the profession */
	std::set<SubcategoriedSkillData> occupational_skills_{}; /**< Skills that are considered occupational for the profession */
	std::set<SubcategoriedSkillData> restricted_skills_{}; /**< Skills that are considered restricted for the profession */
	std::set<SubcategoriedSkillData> standard_skills_{}; /**< Skills that are considered standard for the profession */
	std::vector<GameRuleDataChoice<SkillData>> everyman_skill_choices_{}; /** Set of skills that the character may select one or more from to become everyman */
	std::vector<GameRuleDataChoice<SkillCategoryData>> category_everyman_skill_choices_{}; /** Set of skill categories that the character may select one or more skills from to become everyman */
	std::map<std::string, int> group_bonuses_{}; /** bonus to skill categories in a group */
	std::map<const SkillCategoryData*, int> skill_category_bonuses_{}; /** bonus to skill categories */
	std::map<SubcategoriedSkillData, int> skill_bonuses_{}; /** bonus to individual skills */
	std::map<const SkillCategoryData*, SkillDevelopmentCost> skill_category_development_costs_{}; /** Cost to purchase ranks for a skill category */
};