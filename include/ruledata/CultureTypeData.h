#pragma once

#include <stdexcept>
#include <ranges>
#include <set>
#include <string_view>
#include <ArmourType.h>
#include <GameRuleData.h>
#include <GameRuleDataChoice.h>
#include <SubcategoriedSkillData.h>
#include <SkillCategoryData.h>
#include <WeaponTypeData.h>

class CultureTypeData : public GameRuleData {
public:

	/**
	 * @brief Thrown when there is an attempt to set an invalid skill rank
	 */
	class InvalidSkillRank : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidSkillRank(const std::string& error) : std::runtime_error{ error } {}
	};

	/**
	 * @brief Delete the default constructor to ensure we initialise the base class
	 */
	CultureTypeData() = delete;

	/**
	 * @brief Basic constructor
	 * @param id Unique identifier of the culture type
	 */
	CultureTypeData(std::string_view id) : GameRuleData(id) {}

	/**
	* @brief Set the name of the race
	* @param name Race name
	*/
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the race
	 * @return Race name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of the race
	 * @param description std::string_view race description
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Get the description of the race
	 * @return std::string reference of the description
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set a description of some concepts for characters form this culture 
	 * @param concepts character concept description
	 */
	void setCharacterConcept(std::string_view concepts) { character_concepts_ = concepts; }

	/**
	 * @brief Get a description of some concepts for characters form this culture 
	 * @return Concepts for characters
	 */
	const std::string& characterConcept() const { return character_concepts_; }

	/**
	 * @brief Set the typical clothing worn by members of the culture
	 * @param clothing clothing description
	 */
	void setClothing(std::string_view clothing) { clothing_ = clothing; }

	/**
	 * @brief Get the typical clothing worn by members of the culture
	 * @return clothing description
	 */
	const std::string& clothing() const { return clothing_; }

	/**
	 * @brief Set the typical aspirations for members of this culture
	 * @param aspirations aspirations description
	 */
	void setAspirations(std::string_view aspirations) { aspirations_ = aspirations; }

	/**
	 * @brief Get the typical aspirations for members of this culture
	 * @return aspirations description
	 */
	const std::string& aspirations() const { return aspirations_; }

	/**
	 * @brief Set the collective fears of members of this culture
	 * @param fears collective fears
	 */
	void setFears(std::string_view fears) { fears_ = fears; }

	/**
	 * @brief Get the collective fears of members of this culture
	 * @return collective fears
	 */
	const std::string& fears() const { return fears_; }

	/**
	 * @brief Set how members of the culture typically view marriage
	 * @param marriage_patterns typical marriage patterns
	 */
	void setMarriagePatterns(std::string_view marriage_patterns) { marriage_patterns_ = marriage_patterns; }

	/**
	 * @brief Get how members of the culture typically view marriage
	 * @return typical marriage patterns
	 */
	const std::string& marriagePatterns() const { return marriage_patterns_; }

	/**
	 * @brief Set prejudices that are typical to members of the culture
	 * @param prejudices typical cultural prejudices
	 */
	void setPrejudices(std::string_view prejudices) { prejudices_ = prejudices; }

	/**
	 * @brief Get prejudices that are typical to members of the culture
	 * @return typical cultural prejudices
	 */
	const std::string& prejudices() const { return prejudices_; }

	/**
	 * @brief Set religious beliefs practiced by members of the culture
	 * @param religious_beliefs religious beliefs
	 */
	void setReligiousBeliefs(std::string_view religious_beliefs) { religion_ = religious_beliefs; }

	/**
	 * @brief Get religious beliefs practiced by members of the culture
	 * @return religious beliefs
	 */
	const std::string& religiousBeliefs() const { return religion_; }

	/**
	 * @brief Set the number of skill ranks that the character has to spend on hobbies during their adolescence
	 * @param hobby_skill_ranks number of skill ranks
	 */
	void setHobbySkillRanks(int hobby_skill_ranks) { hobby_skill_ranks_ = hobby_skill_ranks; }

	/**
	 * @brief Get the number of skill ranks that the character has to spend on hobbies during their adolescence
	 * @return number of skill ranks
	 */
	int hobbySkillRanks() const { return hobby_skill_ranks_; }

	/**
	 * @brief Set the number of ranks that the character receives on one open spell list of their realm
	 * @param spell_list_ranks number of spell list ranks
	 */
	void setSpellListRanks(int spell_list_ranks) { spell_list_ranks_ = spell_list_ranks; }

	/**
	 * @brief Get the number of skill ranks that the character receives on one open spell list of their realm
	 * @return number of spell list ranks
	 */
	int spellListRanks() const { return spell_list_ranks_; }

	/**
	 * @brief Add an armour type to the set of those preferred by the culture
	 * @param armour_type ArmourType::Type to add
	 */
	void addPreferredArmour(ArmourType::Type armour_type) { preferred_armour_.emplace(armour_type); }

	/**
	 * @brief Get a container with the armour types preferred by the culture
	 * @return std::set<ArmourType::Type> armour types
	 */
	const std::set<ArmourType::Type> preferredArmour() const { return preferred_armour_; }

	/**
	 * @brief Get whether an armour type is amongst those prefereed by the culture
	 * @param armour_type ArmourType::Type to check
	 * @return `true` if the armour type is preferred by the culture
	 * @return `true` if the armour type is not preferred by the culture
	 */
	bool isPreferredArmour(ArmourType::Type armour_type) const { return (preferred_armour_.find(armour_type) != preferred_armour_.end()); }

	/**
	 * @brief Add an weapon type to the set of those preferred by the culture
	 * @param weapon WeaponTypeData to add
	 */
	void addPreferredWeapon(WeaponTypeData& weapon) { preferred_weapons_.emplace(&weapon); }

	/**
	 * @brief Get a container with the weapons preferred by the culture
	 * @return std::set<WeaponTypeData> weapons
	 */
	const std::set<const WeaponTypeData*> preferredWeapons() const { return preferred_weapons_; }

	/**
	 * @brief Get whether an weapon type is amongst those prefereed by the culture
	 * @param weapon WeaponTypeData to check
	 * @return `true` if the weapon type is preferred by the culture
	 * @return `true` if the weapon type is not preferred by the culture
	 */
	bool isPreferredWeapon(WeaponTypeData& weapon) const { 
		for (auto& key : preferred_weapons_) {
			if (weapon.id() == key->id()) return true;
		}
		return false; 
	}

	/**
	 * @brief Set the number of ranks for a skill the culture provides during adolescence
	 * @param skill SubcategoriedSkillData to add the ranks to
	 * @param ranks int number of ranks
	 */
	void setSkillRanks(SubcategoriedSkillData skill, int ranks) {
		if (isRankSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillRank("There is already a rank set for skill " + skill.id());
		skill_ranks_.emplace(std::move(skill), ranks);
	}

	/**
	 * @brief Get the number of ranks for a skill the culture provides during adolescence
	 * @param skill SubcategoriedSkillData to get the ranks for
	 * @return number of ranks
	 */
	int skillBonus(const SubcategoriedSkillData& skill) const {
		for (auto& key : skill_ranks_) {
			if (key.first.id() == skill.id()) return key.second;
		}
		return 0;
	}

	/**
	 * @brief Get the number of ranks for a skill the culture provides during adolescence
	 * @param skill SkillData to get the ranks for
	 * @param subcategory optional subcategory of @a skill
	 * @return number of ranks
	 */
	int skillBonus(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		return skillBonus(SubcategoriedSkillData(skill, subcategory));
	}

	/**
	 * @brief Gets a container with the skills that the culture provides adolescent ranks for
	 * @return std::set of SkillData with adolescent ranks
	 */
	const std::set<SubcategoriedSkillData> skillsWithRanks() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& key : skill_ranks_) {
			const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if the culture provides adolescent skill ranks for the skill
	 * @param skill SkillData to check
	 * @param subcategory optional subcategory of @a skill
	 * @return `true` if the culture provides adolescent ranks
	 * @return `false` if the culture does not provide adolescent ranks
	 */
	bool isRankSkill(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& key : std::views::keys(skill_ranks_)) {
			if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add number of skill ranks a cetegory receives during adolescence
	 * @param category SkillCategoryData to add ranks for
	 * @param ranks int adolescent ranks
	 */
	void addSkillCategoryRanks(const SkillCategoryData& category, int ranks) { skill_category_ranks_.emplace(&category, ranks); }

	/**
	 * @brief Get a container of all the skill categories with a adolescent ranks
	 * @return std::set of categories with adolescent ranks
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithRanks() const {
		auto keys = std::views::keys(skill_category_ranks_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Check if the culture provides adolescent skill ranks for the category
	 * @param category SkillCategoryData to check
	 * @return `true` if the category has adolescent ranks
	 * @return `false` if the category does not have adolescent ranks
	 */
	bool isRankCategory(const SkillCategoryData& category) const {
		for (auto& cat : std::views::keys(skill_category_ranks_)) {
			if (cat->id() == category.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the number of ranks a category receives during adolescence
	 * @param category SkillCategoryData to check
	 * @return int number of ranks @a category receives
	 */
	int skillCategoryRanks(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_ranks_)) {
			if (cat->id() == category.id()) return skill_category_ranks_.at(cat);
		}
		return 0;
	}

	/**
	 * @brief Add number of skill ranks a skill in the category receives during adolescence
	 * @param category SkillCategoryData to add a bonus for
	 * @param ranks int number of adolescent ranks to add to a skill in the category
	 */
	void addSkillCategorySkillRanks(const SkillCategoryData& category, int ranks) { skill_category_skill_ranks_.emplace(&category, ranks); }

	/**
	 * @brief Get a container of all the skill categories with a adolescent ranks to add to a skill
	 * @return std::set of categories with adolescent ranks to be added to a skill
	 */
	const std::set<const SkillCategoryData*> skillCategoriesWithSkillRanks() const {
		auto keys = std::views::keys(skill_category_skill_ranks_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Check if the culture provides adolescent skill ranks to a skill in the category
	 * @param category SkillCategoryData to check
	 * @return `true` if the category has adolescent ranks applied to a skill
	 * @return `false` if the category does not have adolescent ranks applied to a skill
	 */
	bool isSkillRankCategory(const SkillCategoryData& category) const {
		for (auto& cat : std::views::keys(skill_category_skill_ranks_)) {
			if (cat->id() == category.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Get the number of ranks a skill in the category receives during adolescence
	 * @param category SkillCategoryData to check
	 * @return int number of ranks a skill in @a category receives
	 */
	int skillCategorySkillRanks(const SkillCategoryData& category) {
		for (auto& cat : std::views::keys(skill_category_skill_ranks_)) {
			if (cat->id() == category.id()) return skill_category_skill_ranks_.at(cat);
		}
		return 0;
	}

private:
	std::string name_{}; /**< Name of the culture type */
	std::string description_{}; /**< General description of the culture type */
	std::string character_concepts_{}; /**< Idea of a character from the cultures outlook */
	std::string clothing_{}; /**< Typical clothing of a member of the culture */
	std::string aspirations_{}; /**< Aspirations of typical members of the culture */
	std::string fears_{}; /** Collective fears of members of the culture */
	std::string marriage_patterns_{}; /** How members of teh culture typically view marriage */
	std::string prejudices_{}; /** Any prejudices that are typical to members of the culture */
	std::string religion_{}; /** Typical religious beliefs practiced by members of the culture */
	int hobby_skill_ranks_{}; /**< Number of hobby skill ranks available during adolescence */
	int spell_list_ranks_{}; /**< Number of ranks in an open spell list members of the culture receive */
	std::set<ArmourType::Type> preferred_armour_{}; /**< Armour type typically preferred by members of the culture */
	std::set<const WeaponTypeData*> preferred_weapons_{}; /**< Weapon type typically preferred by members of the culture */
	std::map<SubcategoriedSkillData, int> skill_ranks_{}; /** Number of skill ranks gained during adolescence */
	std::map<const SkillCategoryData*, int> skill_category_ranks_{}; /** Number of skill category ranks gained during adolescence */
	std::map<const SkillCategoryData*, int> skill_category_skill_ranks_{}; /** Number of skill ranks from a category gained during adolescence */

};
