#pragma once

#include <GameObject.h>
#include <CharacterCategory.h>
#include <CharacterSkill.h>
#include <CharacterSpellList.h>
#include <CharacterStat.h>
#include <CharacterLanguage.h>
#include <RaceData.h>
#include <CultureData.h>
#include <ProfessionData.h>
#include <RealmType.h>
#include <ResistanceType.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>
#include <unordered_map>
#include <string>

// Forward declaration to break the circular include with CharacterSerializer.h
namespace rm::serial {
class CharacterSerializer;
} // namespace rm::serial

namespace rm::game::character {

using namespace rm::rule;
using namespace rm::rule::enums;

/**
 * @class Character
 * @brief Represents a character in the game, which is a type of GameObject that has various stats and a name.
 *
 * Each character has a set of stats, which are represented by the `Stat` class and stored in an unordered map with the stat type as the key. The character also has a name, which is used for display purposes and may not be unique.
 */
class Character : public rm::game::GameObject {
	friend class rm::serial::CharacterSerializer; /**< CharacterSerializer is a friend to allow it access to the private members of this class for serialisation and deserialisation */
	friend class CharacterBuilder;                /*< CharacterBuilder is a friend to allow it access to the private members of this class for building characters with specific stats and names */
	friend class CharacterLeveller;               /*< CharacterLeveller is a friend to allow it access to the private members of this class for levelling characters with specific stats and names */
public:
	/** @brief string_view of the ID for the body development skill, which is used to identify the skill in the game data and to reference it when building characters. */
	static constexpr std::string_view BD_SKILL_ID{"SKILL_BODY_DEVELOPMENT"};
	/** @brief string_view of the ID for the power point development skill, which is used to identify the skill in the game data and to reference it when building characters. */
	static constexpr std::string_view PP_SKILL_ID{"SKILL_POWER_POINT_DEVELOPMENT"};

	/**
	 * @brief Get the character's stat for a given stat type (const version).
	 * @param stat_type The type of stat to retrieve (e.g., strength, dexterity, etc.).
	 * @return A constant reference to the character's stat for the specified stat type.
	 */
	const Stat& getStat(StatType::Type stat_type) const {
		return stats_.at(stat_type);
	}

	/**
	 * @brief Sets the stat for a specific stat type.
	 * @param stat_type The type of statistic to set.
	 * @param stat The statistic to store.
	 */
	void setStat(StatType::Type stat_type, Stat stat) {
		stats_[stat_type] = stat;
		updateStatDerivedData(stat_type);
	}

	/**
	 * @brief Updates the stat temp for a specific stat type by adding a temporary value to the existing temporary value.
	 * @param stat_type The type of statistic to update.
	 * @param difference The temporary value to add to the existing temporary value of the specified statistic.
	 */
	void updateStatTemporary(StatType::Type stat_type, int difference) {
		stats_[stat_type].updateTemporary(difference);
		updateStatDerivedData(stat_type);
	}

	/**
	 * @brief Get the bonus for a given stat type.
	 * @param stat_type The type of stat to retrieve the bonus for (e.g., strength, dexterity, etc.).
	 * @return The bonus value for the specified stat type.
	 */
	int statBonus(StatType::Type stat_type) const {
		return stats_.at(stat_type).bonus();
	}

	/**
	 * @brief Set the name of the character.
	 *
	 * The name is used for display purposes and may not be unique. It can be set at character creation and can be changed later if desired.
	 *
	 * @param name The name to set for the character.
	 */
	void setName(const std::string& name) {
		name_ = name;
	}

	/**
	 * @brief Get the name of the character.
	 *
	 * The name is used for display purposes and may not be unique. It can be set at character creation and can be changed later if desired.
	 *
	 * @return A constant reference to the name of the character.
	 */
	const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Set the race of the character.
	 *
	 * @param race The race data to set for the character.
	 */
	void setRace(const RaceData& race) {
		race_ = &race;
	}

	/**
	 * @brief Get the race data of the character.
	 *
	 * @return A pointer to the race data of the character.
	 */
	const RaceData* race() const {
		return race_;
	}

	/**
	 * @brief Set the culture of the character.
	 *
	 * @param culture The culture data to set for the character.
	 */
	void setCulture(const CultureData& culture) {
		culture_ = &culture;
	}

	/**
	 * @brief Get the culture data of the character.
	 *
	 * @return A pointer to the culture data of the character.
	 */
	const CultureData* culture() const {
		return culture_;
	}

	/**
	 * @brief Set the profession of the character.
	 *
	 * @param profession The profession data to set for the character.
	 */
	void setProfession(const ProfessionData& profession) {
		profession_ = &profession;
	}

	/**
	 * @brief Get the profession data of the character.
	 *
	 * @return A pointer to the profession data of the character.
	 */
	const ProfessionData* profession() const {
		return profession_;
	}

	/**
	 * @brief Check if the character has a language ability for a specific language.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @param language_name The name of the language to check for.
	 * @return `true` if the character has a language ability for the specified language name, `false` otherwise.
	 */
	bool hasLanguageAbility(const std::string& language_name) const {
		return languages_.find(language_name) != languages_.end();
	}

	/**
	 * @brief Get the language abilities of the character.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @return A constant reference to the unordered map of language abilities for the character.
	 */
	const std::unordered_map<std::string, Language>& languageAbilities() const {
		return languages_;
	}

	/**
	 * @brief Get the language ability for a specific language.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @param language_name The name of the language to retrieve the ability for.
	 * @return A constant reference to the `Language` object for the specified language.
	 * @throw std::out_of_range if there is no language ability for the specified language name.
	 */
	const Language& languageAbility(const std::string& language_name) const {
		if (!hasLanguageAbility(language_name)) {
			throw std::out_of_range("There is no language ability for the language " + language_name);
		}
		return languages_.at(language_name);
	}

	/**
	 * @brief Set a language ability for the character.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @param language_ability The `Language` object to set for the character. The language name from the `Language` object will be used as the key in the unordered map.
	 */
	void setLanguageAbility(Language language_ability) {
		languages_.emplace(language_ability.languageName(), language_ability);
	}

	/**
	 * @brief Set whether the character is a player character or an NPC.
	 *
	 * This is used to differentiate between characters that are controlled by the player and those that are controlled by the game. Player characters may have different interactions and behaviors compared to NPCs, so this flag can be used
	 * to determine how the character should be treated in various situations.
	 *
	 * @param is_player_character `true` if the character is a player character, `false` if it is an NPC.
	 */
	void setPlayerCharacter(bool is_player_character) {
		player_character_ = is_player_character;
	}

	/**
	 * @brief Check if the character is a player character or an NPC.
	 *
	 * This is used to differentiate between characters that are controlled by the player and those that are controlled by the game. Player characters may have different interactions and behaviors compared to NPCs, so this flag can be used
	 * to determine how the character should be treated in various situations.
	 *
	 * @return `true` if the character is a player character, `false` if it is an NPC.
	 */
	bool isPlayerCharacter() const {
		return player_character_;
	}

	/**
	 * @brief Set whether the character is male or female.
	 *
	 * This is used to differentiate between male and female characters. Some interactions and behaviors may depend on the character's gender, so this flag can be used to determine how the character should be treated in various situations.
	 *
	 * @param is_male `true` if the character is male, `false` if the character is female.
	 */
	void setMale(bool is_male) {
		male_ = is_male;
	}

	/**
	 * @brief Check if the character is male or female.
	 *
	 * This is used to differentiate between male and female characters. Some interactions and behaviors may depend on the character's gender, so this flag can be used to determine how the character should be treated in various situations.
	 *
	 * @return `true` if the character is male, `false` if the character is female.
	 */
	bool isMale() const {
		return male_;
	}

	/**
	 * @brief Set the height of the character in inches.
	 *
	 * This is used to represent the physical height of the character. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @param height The height of the character in inches.
	 */
	void setHeight(int height) {
		height_ = height;
	}

	/**
	 * @brief Get the height of the character in inches.
	 *
	 * This is used to represent the physical height of the character. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @return The height of the character in inches.
	 */
	int height() const {
		return height_;
	}

	/**
	 * @brief Get the height of the character in feet and inches as a formatted string.
	 *
	 * This is used to represent the physical height of the character in a more human-readable format. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @return The height of the character in feet and inches as a formatted string (e.g., "5 ft 10 in").
	 */
	std::string heightInFeetAndInches() const {
		int feet = height_ / 12;
		int inches = height_ % 12;
		return std::to_string(feet) + " ft " + std::to_string(inches) + " in";
	}

	/**
	 * @brief Set the weight of the character in pounds.
	 *
	 * This is used to represent the physical weight of the character. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @param weight The weight of the character in pounds.
	 */
	void setWeight(int weight) {
		weight_ = weight;
	}

	/**
	 * @brief Get the weight of the character in pounds.
	 *
	 * This is used to represent the physical weight of the character. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @return The weight of the character in pounds.
	 */
	int weight() const {
		return weight_;
	}

	/**
	 * @brief Get the weight of the character in stone and pounds as a formatted string.
	 *
	 * This is used to represent the physical weight of the character in a more human-readable format. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @return The weight of the character in stone and pounds as a formatted string (e.g., "11 st 4 lb").
	 */
	std::string weightInStoneAndPounds() const {
		int stone = weight_ / 14;
		int pounds = weight_ % 14;
		return std::to_string(stone) + " st " + std::to_string(pounds) + " lb";
	}

	/**
	 * @brief Set the description of the character's build.
	 *
	 * This is used to represent the physical build of the character. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @param build_description A description of the character's build.
	 */
	void setBuildDescription(const std::string& build_description) {
		build_description_ = build_description;
	}

	/**
	 * @brief Get the description of the character's build.
	 *
	 * This is used to represent the physical build of the character. It can be used for display purposes and may also have an impact on certain interactions and behaviors in the game.
	 *
	 * @return A description of the character's build.
	 */
	const std::string& buildDescription() const {
		return build_description_;
	}

	/**
	 * @brief Set the expected lifespan of the character if no external factors apply.
	 *
	 * This is used to represent the expected lifespan of the character based on their race.
	 */
	void setLifespan(int lifespan) {
		lifespan_ = lifespan;
	}

	/**
	 * @brief Get the expected lifespan of the character if no external factors apply.
	 *
	 * This is used to represent the expected lifespan of the character based on their race.
	 */
	int lifespan() const {
		return lifespan_;
	}

	/**
	 * @brief Get the power realms that the character has access to.
	 *
	 * The power realms are represented as a set of `RealmType::Type` values, which indicate the different power realms that the character can access. This information can be used for display purposes and may also have an impact on
	 * certain interactions and behaviors in the game.
	 *
	 * @return A constant reference to the set of power realms that the character has access to.
	 */
	const std::set<RealmType::Type>& powerRealms() const {
		return power_realms_;
	}

	/**
	 * @brief Set the power realms that the character has access to.
	 *
	 * The power realms are represented as a set of `RealmType::Type` values, which indicate the different power realms that the character can access. This information can be used for display purposes and may also have an impact on
	 * certain interactions and behaviors in the game.
	 *
	 * @param power_realms A set of `RealmType::Type` values representing the power realms that the character has access to.
	 */
	void setPowerRealms(const std::set<RealmType::Type>& power_realms) {
		power_realms_ = power_realms;
	}

	/**
	 * @brief Check if the character has access to a specific power realm.
	 *
	 * The power realms are represented as a set of `RealmType::Type` values, which indicate the different power realms that the character can access. This information can be used for display purposes and may also have an impact on
	 * certain interactions and behaviors in the game.
	 *
	 * @param realm The `RealmType::Type` value representing the power realm to check for.
	 * @return `true` if the character has access to the specified power realm, `false` otherwise.
	 */
	bool isPowerRealm(RealmType::Type realm) const {
		return power_realms_.find(realm) != power_realms_.end();
	}

	/**
	 * @brief Add a power realm to the character's set of accessible power realms.
	 *
	 * The power realms are represented as a set of `RealmType::Type` values, which indicate the different power realms that the character can access. This information can be used for display purposes and may also have an impact on
	 * certain interactions and behaviors in the game.
	 *
	 * @param realm The `RealmType::Type` value representing the power realm to add to the character's set of accessible power realms.
	 */
	void addPowerRealm(RealmType::Type realm) {
		power_realms_.insert(realm);
	}

	/**
	 * @brief Sets the development points value.
	 *
	 * The development points represent the number of points the character has for development and progression and are based on the character stats and are spent during the level up process. Not all points need to be spent each level up and
	 * any unspent points may be arried forward to the next level up.
	 *
	 * @param development_points The new development points value to set.
	 */
	void setDevelopmentPoints(int development_points) {
		development_points_ = development_points;
	}

	/**
	 * @brief Modifies the development points value by adding a specified difference.
	 *
	 * The development points represent the number of points the character has for development and progression and are based on the character stats and are spent during the level up process. Not all points need to be spent each level up and
	 * any unspent points may be arried forward to the next level up.
	 *
	 * @param difference The amount to add to the current development points value. This can be a positive or negative value, depending on whether you want to increase or decrease the development points.
	 */
	void modifyDevelopmentPoints(int difference) {
		development_points_ += difference;
	}

	/**
	 * @brief Get the current development points value.
	 *
	 * The development points represent the number of points the character has for development and progression and are based on the character stats and are spent during the level up process. Not all points need to be spent each level up and
	 * any unspent points may be arried forward to the next level up.
	 *
	 * @return The current development points value for the character.
	 */
	int developmentPoints() const {
		return development_points_;
	}

	/**
	 * @brief Get the bonus for a given skill based on the number of ranks allocated to it and the category.
	 *
	 * This includes the bonus from ranks from the category and the applicable stats.
	 *
	 * @param skill The `SubcategoriedSkillData` object representing the skill to calculate the bonus for.
	 * @return The total rank bonus for the specified skill.
	 */
	int skillBonus(const SubcategoriedSkillData& skill) const;

	/**
	 * @brief Get the bonus for a given skill category based on the applicable stats.
	 *
	 * This includes the bonus from ranks from the category and the applicable stats.
	 *
	 * @param category The `SkillCategoryData` object representing the skill category to calculate the bonus for.
	 * @return The total rank bonus for the specified skill category.
	 */
	int categoryBonus(const SkillCategoryData& category) const;

	/**
	 * @brief Get the bonus for a given spell list based on the applicable stats.
	 *
	 * This includes the bonus from ranks from the spell list and the applicable stats.
	 *
	 * @param spell_list The `SpellListData` object representing the spell list to calculate the bonus for.
	 * @return The total rank bonus for the specified spell list.
	 */
	int spellListBonus(const SpellListData& spell_list) const;

	/**
	 * @brief Get the bonus for the somatic component of a given language based on the applicable stats.
	 *
	 * This includes the bonus from ranks from the language and the applicable stats.
	 *
	 * @param language_name The name of the language to calculate the bonus for.
	 * @return The total rank bonus for the specified language.
	 */
	int languageSomaticBonus(const std::string& language_name) const;

	/**
	 * @brief Get the bonus for the spoken component of a given language based on the applicable stats.
	 *
	 * This includes the bonus from ranks from the language and the applicable stats.
	 *
	 * @param language_name The name of the language to calculate the bonus for.
	 * @return The total rank bonus for the specified language.
	 */
	int languageSpokenBonus(const std::string& language_name) const;

	/**
	 * @brief Get the bonus for the written component of a given language based on the applicable stats.
	 *
	 * This includes the bonus from ranks from the language and the applicable stats.
	 *
	 * @param language_name The name of the language to calculate the bonus for.
	 * @return The total rank bonus for the specified language.
	 */
	int languageWrittenBonus(const std::string& language_name) const;

	/**
	 * @brief Get the amount of gold the character has.
	 *
	 * This is used to represent the wealth of the character.
	 *
	 * @return The amount of gold the character has.
	 */
	int gold() const {
		return gold_;
	}

	/**
	 * @brief Set the amount of gold the character has.
	 *
	 * This is used to represent the wealth of the character.
	 *
	 * @param gold The new amount of gold for the character.
	 */
	void setGold(int gold) {
		gold_ = gold;
	}

	/**
	 * @brief Modifies the amount of gold the character has by adding a specified difference.
	 *
	 * This is used to represent the wealth of the character.
	 *
	 * @param difference The amount to add to the current gold value. This can be a positive or negative value, depending on whether you want to increase or decrease the character's gold.
	 */
	void changeGold(int difference) {
		gold_ += difference;
	}

	/**
	 * @brief Add an item to the character's inventory.
	 *
	 * The items are represented as a vector of strings, where each string is the name of an item.
	 *
	 * @param item_name The name of the item to add to the character's inventory.
	 */
	void addItem(const std::string& item_name) {
		items_.push_back(item_name);
	}

	/**
	 * @brief Remove an item from the character's inventory.
	 *
	 * The items are represented as a vector of strings, where each string is the name of an item.
	 *
	 * @param item_name The name of the item to remove from the character's inventory. If there are multiple items with the same name, only the first one will be removed.
	 */
	void removeItem(const std::string& item_name) {
		auto it = std::find(items_.begin(), items_.end(), item_name);
		if (it != items_.end()) {
			items_.erase(it);
		}
	}

	/**
	 * @brief Get the items that the character has.
	 *
	 * The items are represented as a vector of strings, where each string is the name of an item.
	 *
	 * @return A vector of strings representing the names of the items that the character has.
	 */
	std::vector<std::string> items() const {
		return items_;
	}

	/**
	 * @brief Get the maximum number of hit points the character has when fully rested.
	 * @return int The maximum number of hit points the character has when fully rested, which is calculated based on the character's stats and other factors.
	 */
	int maxHits() const {
		return max_hits_;
	}

	/**
	 * @brief Get the number of hit points the character has.
	 *
	 * @return number of hits the character has.
	 */
	int hits() const {
		return hits_;
	}

	/**
	 * @brief Get the maximum number of power points the character has when fully rested.
	 * @return int The maximum number of power points the character has when fully rested, which is calculated based on the character's stats and other factors.
	 */
	int maxPowerPoints() const {
		return max_power_points_;
	}

	/**
	 * @brief Get the number of power points the character has.
	 *
	 * @return number of power points the character has.
	 */
	int powerPoints() const {
		return power_points_;
	}

	/**
	 * @brief Get the level of the character.
	 * @return int The level of the character, which represents their overall power and progression in the game.
	 */
	int level() const {
		return level_;
	}

	/**
	 * @brief Sets the experience points value.
	 * @param experience_points The new experience points value to set.
	 */
	void setExperiencePoints(int experience_points) {
		experience_points_ = experience_points;
	}

	/**
	 * @brief Modifies the experience points value by adding a specified difference.
	 * @param difference The amount to add to the current experience points value. This can be a positive or negative value, depending on whether you want to increase or decrease the experience points.
	 */
	void addExperiencePoints(int difference) {
		experience_points_ += difference;
	}

	/**
	 * @brief Get the current experience points value.
	 * @return The current experience points value for the character, which is used to determine when they level up.
	 */
	int experiencePoints() const {
		return experience_points_;
	}

	/**
	 * @brief Get the character's spell lists.
	 *
	 * The spell lists are represented as a map of `SpellListData` pointers to `SpellList` objects, which represent the character's spell lists and their associated data. Each spell list has a certain number of ranks allocated to it, which contribute
	 * to the character's abilities and bonuses in various situations.
	 *
	 * @return A constant reference to the map of spell lists for the character.
	 */
	const std::map<const SpellListData*, SpellList>& spellLists() const {
		return spell_lists_;
	}

private:
	/* Basic data */
	std::string name_;                               /**< The name of the character. This is used for display purposes and may not be unique. */
	bool male_{false};                               /**< Whether the character is male or female. */
	bool player_character_{false};                   /**< Whether the character is a player character or an NPC. */
	int level_{1};                                   /**< The level of the character, which represents their overall power and progression in the game. */
	int experience_points_{0};                       /**< The amount of experience points the character has, which is used to determine when they level up. */
	int gold_{0};                                    /**< The amount of gold the character has. */
	const RaceData* race_{nullptr};                  /**< The race of the character. */
	const CultureData* culture_{nullptr};            /**< The culture of the character. */
	const ProfessionData* profession_{nullptr};      /**< The profession of the character. */
	std::unordered_map<StatType::Type, Stat> stats_; /**< Map of stat types to their corresponding Stat objects for the character. Each character will have 10 stats, such as strength, dexterity, etc. */
	std::vector<std::string> items_{};               /**< A vector of item names representing the items that the character has. */
	int max_hits_{0};                                /**< The maximum number of hit points the character has when fully rested, which is calculated based on the character's stats and other factors. */
	int hits_{0};                                    /**< The number of hit points the character has, which is calculated based on the character's stats and other factors. */
	int max_power_points_{0};                        /**< The maximum number of power points the character has when fully rested, which is calculated based on the character's stats and other factors. */
	int power_points_{0};                            /**< The number of power points the character has, which is calculated based on the character's stats and other factors. */

	/* Physical characteristics */
	int height_{0};                 /**< The height of the character in inches. */
	int weight_{0};                 /**< The weight of the character in pounds. */
	std::string build_description_; /**< A description of the character's build, which may be used for display purposes. */
	int lifespan_{0};               /**< The expected lifespan of the character in years, which may be used for display purposes and to determine age-related effects in the game. */

	/* Derived data */
	int development_points_{0};                                                                  /**< The number of development points the character has, which may be used for character progression and development. */
	std::set<RealmType::Type> power_realms_{};                                                   /**< A set of realm types representing the magical realm choices for the character being built. */
	std::map<ResistanceType::Type, int> resistances_{};                                          /**< A map of resistance roll bonuses for the character. */
	std::map<const SkillCategoryData*, std::set<const SpellListData*>> spell_list_categories_{}; /**< A map of skill categories to sets of spell lists representing the spell lists sorted into their respective skill categories. */

	/* Hits and PPs */
	const SubcategoriedSkillData* body_devlopment_skill_{nullptr};
	const SubcategoriedSkillData* power_point_skill_{nullptr};

	/* Learned abilities */
	std::unordered_map<std::string, Language> languages_;       /**< Map of language names to their corresponding Language objects for the character. */
	std::map<const SubcategoriedSkillData*, Skill> skills_{};   /**< A map of SkillData pointers to Skill objects representing the character's skills. */
	std::map<const SkillCategoryData*, Category> categories_{}; /**< A map of SkillCategoryData pointers to Category objects representing the categories of skills the character has. */
	std::map<const SpellListData*, SpellList> spell_lists_{};   /**< A map of SpellListData pointers to SpellList objects representing the character's spell lists. */

	/* Utility functions */
	void updateAllDerivedData();
	void updateStatDerivedData(StatType::Type stat_type);
	void updateDevelopmentPoints();
	void updateMaxHits();
	void updateMaxPowerPoints();
};

/* ------------------------------------------------------------------ */
/* Free functions                                                     */
/* ------------------------------------------------------------------ */

/**
 * @brief Get the skill category for a given spell list.
 *
 * This function takes a map of skill categories to sets of spell lists and a specific spell list, and returns the skill category that the spell list belongs to. If the spell list is not found in any of the categories, it returns
 * `nullptr`.
 *
 * @param spell_list_categories A map of skill categories to sets of spell lists representing the spell lists sorted into their respective skill categories.
 * @param spell_list The `SpellListData` object representing the spell list to find the category for.
 * @return A pointer to the `SkillCategoryData` object representing the category that the specified spell list belongs to, or `nullptr` if the spell list is not found in any category.
 */
const SkillCategoryData* spellListCategory(std::map<const SkillCategoryData*, std::set<const SpellListData*>> spell_list_categories, const SpellListData& spell_list);

/**
 * @brief Get a map of spell lists to their corresponding rank costs for the character.
 *
 * This function calculates the cost for purchasing ranks during a level up for each spell list. This is based on the profession, the current number of ranks, and the type of spell list. The costs are returned as a map of `SpellListData`
 * pointers to strings representing the rank costs for each spell list for the given character.
 * 
 * The map only contains the spell lists that the character already has ranks in as for all others the cost in the category is correct.
 *
 * @param character The character for which to calculate the spell list rank costs.
 * @return A map of `SpellListData` pointers to strings representing the rank costs for each spell list for the given character.
 */
std::map<const SpellListData*, std::string> spellListsRankCosts(const Character& character);

} // namespace rm::game::character