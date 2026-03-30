#pragma once

#include <GameObject.h>
#include <CharacterStat.h>
#include <LanguageAbility.h>
#include <RaceData.h>
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
	 * @brief Sets the stat temp for a specific stat type.
	 * @param stat_type The type of statistic to set.
	 * @param temp_value The statistic temporary value to store.
	 */
	void setStat(StatType::Type stat_type, int temp_value) {
		stats_[stat_type].setTemporary(temp_value);
	}

	/**
	 * @brief Updates the stat temp for a specific stat type by adding a temporary value to the existing temporary value.
	 * @param stat_type The type of statistic to update.
	 * @param difference The temporary value to add to the existing temporary value of the specified statistic.
	 */
	void updateStat(StatType::Type stat_type, int difference) {
		stats_[stat_type].updateTemporary(difference);
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
	 * @brief Get the race data of the character.
	 *
	 * @return A pointer to the race data of the character.
	 */
	const RaceData* race() const {
		return race_;
	}

private:
	std::string name_;                               /**< The name of the character. This is used for display purposes and may not be unique. */
	const RaceData* race_{nullptr};                  /**< The race data for the character. */
	std::unordered_map<StatType::Type, Stat> stats_; // Map of stat types to their corresponding Stat objects for the character. Each character will have 10 stats, such as strength, dexterity, etc.
	std::unordered_map<std::string, LanguageAbility>
	    language_abilities_; /**< Map of language names to their corresponding LanguageAbility objects for the character. Each character can have multiple language abilities, which represent the languages they can communicate using. */
};

} // namespace rm::game::character