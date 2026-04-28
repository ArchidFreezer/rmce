#pragma once
#include <CharacterCategory.h>
#include <LanguageRanks.h>
#include <SkillDevelopmentType.h>

// Forward declaration to break the circular include with CharacterSerializer.h
namespace rm::serial {
class CharacterSerializer;
} // namespace rm::serial

namespace rm::game::character {

using namespace rm::rule;

/**
 * @class Language
 * @brief Represents a language for a character, which has a certain number of ranks allocated to it and may have various bonuses from the character's profession and other sources.
 *
 * Each language is associated with a category, which provides a bonus based on the number of ranks allocated to it. The language also has its own progression type, which determines how the bonus from ranks is calculated. Additionally, the
 * language may have a profession bonus and a special bonus from other sources, which are added to the total bonus for the language.
 */
class Language : public LanguageRanks {
	friend class CharacterBuilder;                /**< CharacterBuilder is a friend to allow it access to the private members of this class for building a character */
	friend class CharacterLeveller;               /**< CharacterLeveller is a friend to allow it access to the private members of this class for level up calculations */
	friend class rm::serial::CharacterSerializer; /**< CharacterSerializer is a friend to allow it access to the private members of this class for serialisation and deserialisation */
public:
	/**
	 * @brief Get the somatic bonus for this language based on the number of ranks allocated to it.
	 *
	 * This includes the profession and any special bonus.
	 *
	 * @return The total somatic bonus for this language, including any progression and/or special bonuses.
	 */
	int somaticBonus() const;

	/**
	 * @brief Get the spoken bonus for this language based on the number of ranks allocated to it.
	 *
	 * This includes the profession and any special bonus.
	 *
	 * @return The total spoken bonus for this language, including any progression and/or special bonuses.
	 */
	int spokenBonus() const;

	/**
	 * @brief Get the written bonus for this language based on the number of ranks allocated to it.
	 *
	 * This includes the profession and any special bonus.
	 *
	 * @return The total written bonus for this language, including any progression and/or special bonuses.
	 */
	int writtenBonus() const;

	/**
	 * @brief Get the stats associated with this skill, which are derived from the category.
	 * @return A vector of stat types associated with this skill.
	 */
	const std::vector<StatType::Type>& stats() const;

private:
	const Category* category_{nullptr};
	const SkillProgressionTypeData* progression_type_{nullptr};
	SkillDevelopmentType::Type development_type_{SkillDevelopmentType::kStandard};
	int profession_bonus_{0};
	int special_bonus_{0};

	/**
	 * @brief Get the  bonus for this language that is shared across the somatic, spoken and written forms of the language.
	 *
	 * This includes the category, profession and any special bonus, but does not include any bonus from the ranks as these are calculated separately for each component of the language.
	 *
	 * @return The total shared bonus for this language, including any special bonuses but excluding any bonuses from the ranks.
	 */
	int sharedBonus() const;
};

} // namespace rm::game::character
