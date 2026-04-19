#pragma once

#include <GameObject.h>
#include <CharacterCategory.h>
#include <CharacterSkill.h>
#include <CharacterStat.h>
#include <LanguageAbility.h>
#include <RaceData.h>
#include <CultureData.h>
#include <ProfessionData.h>
#include <RealmType.h>
#include <ResistanceType.h>
#include <SkillProgressionTypeData.h>
#include <StatType.h>
#include <unordered_map>
#include <string>

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
	friend class CharacterBuilder; /*< CharacterBuilder is a friend to allow it access to the private members of this class for building characters with specific stats and names */
public:
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
	 * @brief Set the Body Development progression type for the character.
	 *
	 * @param progression The SkillProgressionTypeData to set for the character's Body Development progression.
	 */
	void setBodyDevelopmentProgression(const SkillProgressionTypeData& progression) {
		bd_progression_ = &progression;
	}

	/**
	 * @brief Get the Body Development progression type for the character.
	 *
	 * @return A pointer to the SkillProgressionTypeData for the character's Body Development progression.
	 */
	const SkillProgressionTypeData* bodyDevelopmentProgression() const {
		return bd_progression_;
	}

	/**
	 * @brief Set the Power Point progression type for the character.
	 *
	 * @param progression The SkillProgressionTypeData to set for the character's Power Point progression.
	 */
	void setPowerPointProgression(const SkillProgressionTypeData& progression) {
		pp_progression_ = &progression;
	}

	/**
	 * @brief Get the Power Point progression type for the character.
	 *
	 * @return A pointer to the SkillProgressionTypeData for the character's Power Point progression.
	 */
	const SkillProgressionTypeData* powerPointProgression() const {
		return pp_progression_;
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
		return language_abilities_.find(language_name) != language_abilities_.end();
	}

	/**
	 * @brief Get the language abilities of the character.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @return A constant reference to the unordered map of language abilities for the character.
	 */
	const std::unordered_map<std::string, LanguageAbility>& languageAbilities() const {
		return language_abilities_;
	}

	/**
	 * @brief Get the language ability for a specific language.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @param language_name The name of the language to retrieve the ability for.
	 * @return A constant reference to the `LanguageAbility` object for the specified language.
	 * @throw std::out_of_range if there is no language ability for the specified language name.
	 */
	const LanguageAbility& languageAbility(const std::string& language_name) const {
		if (!hasLanguageAbility(language_name)) {
			throw std::out_of_range("There is no language ability for the language " + language_name);
		}
		return language_abilities_.at(language_name);
	}

	/**
	 * @brief Set a language ability for the character.
	 *
	 * The language abilities are stored in an unordered map with the language name as the key and the corresponding `LanguageAbility` object as the value. Each character can have multiple language abilities, which represent the languages
	 * they can communicate using.
	 *
	 * @param language_ability The `LanguageAbility` object to set for the character. The language name from the `LanguageAbility` object will be used as the key in the unordered map.
	 */
	void setLanguageAbility(LanguageAbility language_ability) {
		language_abilities_.emplace(language_ability.language(), language_ability);
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

private:
	/* Basic data */
	std::string name_;                               /**< The name of the character. This is used for display purposes and may not be unique. */
	bool male_{false};                               /**< Whether the character is male or female. */
	bool player_character_{false};                   /**< Whether the character is a player character or an NPC. */
	const RaceData* race_{nullptr};                  /**< The race of the character. */
	const CultureData* culture_{nullptr};            /**< The culture of the character. */
	const ProfessionData* profession_{nullptr};      /**< The profession of the character. */
	std::unordered_map<StatType::Type, Stat> stats_; /**< Map of stat types to their corresponding Stat objects for the character. Each character will have 10 stats, such as strength, dexterity, etc. */

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

	/* Progression types */
	const SkillProgressionTypeData* bd_progression_{nullptr}; /**< The SkillProgressionTypeData for the character's Body Development progression. */
	const SkillProgressionTypeData* pp_progression_{nullptr}; /**< The SkillProgressionTypeData for the character's Power Point progression. */

	/* Learned abilities */
	std::unordered_map<std::string, LanguageAbility> language_abilities_; /**< Map of language names to their corresponding LanguageAbility objects for the character. */
	std::map<const SubcategoriedSkillData*, Skill> skills_{};             /**< A map of SkillData pointers to Skill objects representing the character's skills. */
	std::map<const SkillCategoryData*, Category> skill_categories_{};     /**< A map of SkillCategoryData pointers to Category objects representing the categories of skills the character has. */

	/* Utility functions */
	void updateStatDerivedData(StatType::Type stat_type);
};

} // namespace rm::game::character