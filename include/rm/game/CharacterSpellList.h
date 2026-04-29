#pragma once

#include <CharacterCategory.h>
#include <SkillDevelopmentType.h>
#include <SpellListData.h>

// Forward declaration to break the circular include with CharacterSerializer.h
namespace rm::serial {
class CharacterSerializer;
} // namespace rm::serial

namespace rm::game::character {

using namespace rm::rule;

/**
 * @class SpellList
 * @brief Represents a spell list for a character, which has a certain number of ranks allocated to it and may have various bonuses from the character's profession, category, and other sources.
 *
 * Each spell list is associated with a category, which provides a bonus based on the number of ranks allocated to it. The spell list also has its own progression type, which determines how the bonus from ranks is calculated. Additionally, the spell list
 * may have a profession bonus and a special bonus from other sources, which are added to the total bonus for the spell list.
 */
class SpellList {
	friend class CharacterBuilder;                /*< CharacterBuilder is a friend to allow it access to the private members of this class for building characters with specific stats and names */
	friend class CharacterLeveller;               /**< CharacterLeveller is a friend to allow it access to the private members of this class for level up calculations */
	friend class rm::serial::CharacterSerializer; /**< CharacterSerializer is a friend to allow it access to the private members of this class for serialisation and deserialisation */
public:
	/**
	 * @brief Get the bonus for this skill based on the number of ranks allocated to it and the category.
	 *
	 * This includes the bonus from ranks from the category.
	 *
	 * @return The total rank bonus for this skill.
	 */
	int bonus() const;

	/**
	 * @brief Get the stats associated with this skill, which are derived from the category.
	 * @return A vector of stat types associated with this skill.
	 */
	const std::vector<StatType::Type>& stats() const;

private:
	const Category* category_{nullptr};
	const SpellListData* spell_list_{nullptr};
	const SkillProgressionTypeData* progression_type_{nullptr};
	SkillDevelopmentType::Type development_type_{SkillDevelopmentType::kStandard};
	int profession_bonus_{0};
	int ranks_{0};
	int special_bonus_{0};
};

} // namespace rm::game::character
