#pragma once

#include <GameObject.h>
#include <CharacterStat.h>
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
};

} // namespace rm::game::character