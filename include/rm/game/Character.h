#pragma once

#include <GameObject.h>
#include <CharacterStat.h>
#include <StatType.h>
#include <unordered_map>
#include <string>

namespace rm::game::character {

using namespace rm::rule::enums;

class Character : public rm::game::GameObject {
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
	 * @brief Get the potential value of the character's stat for a given stat type.
	 * @param stat_type The type of stat to retrieve the potential value for (e.g., strength, dexterity, etc.).
	 * @return The potential value of the character's stat for the specified stat type.
	 */
	int getStatPotential(StatType::Type stat_type) const {
		return stats_.at(stat_type).potential();
	}

	/**
	 * @brief Get the temporary value of the character's stat for a given stat type.
	 * @param stat_type The type of stat to retrieve the temporary value for (e.g., strength, dexterity, etc.).
	 * @return The temporary value of the character's stat for the specified stat type.
	 */
	int getStatTemporary(StatType::Type stat_type) const {
		return stats_.at(stat_type).temporary();
	}

	/**
	 * @brief Get the total bonus of the character's stat for a given stat type.
	 * @param stat_type The type of stat to retrieve the total bonus for (e.g., strength, dexterity, etc.).
	 * @return The total bonus of the character's stat for the specified stat type, which includes the basic bonus, racial bonus, and any modifiers.
	 */
	int getStatBonus(StatType::Type stat_type) const {
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

private:
	std::string name_;                               /**< The name of the character. This is used for display purposes and may not be unique. */
	std::unordered_map<StatType::Type, Stat> stats_; // Map of stat types to their corresponding Stat objects for the character. Each character will have 10 stats, such as strength, dexterity, etc.
};

} // namespace rm::game::character