#pragma once

#include <SkillCategoryData.h>
#include <SkillDevelopmentCost.h>
#include <SkillProgressionTypeData.h>

namespace rm::game::character {

using namespace rm::rule;

/**
 * @class Category
 * @brief Represents a category of skills for a character, which has a certain number of ranks allocated to it and may have various bonuses from the character's profession and other sources.
 *
 * Each category is associated with a skill category data object, which provides information about the category and its associated stats. The category also has its own progression type, which determines how the bonus from ranks is calculated. Additionally, the category may have
 * a profession bonus and a special bonus from other sources, which are added to the total bonus for the category.
 */
class Category {
	friend class CharacterBuilder; /*< CharacterBuilder is a friend to allow it access to the private members of this class for building characters with specific stats and names */
public:

	/**
	 * @brief Get the bonus for this category based on the number of ranks allocated to it.
	 * 
	 * This includes the profession and any special bonus.
	 * 
	 * @return The total bonus for this category, including any progression and/or special bonuses.
	*/
	int rankBonus() const;

	/**
	 * @brief Get the stats associated with this category, which are derived from the category data.
	 * 
	 * The stats will either be fixed by the catagory or may be the characters realm stats for magical based categories.
	 * @return A vector of stat types associated with this category.
	 */
	const std::vector<StatType::Type>& stats() const;

private:
	const SkillCategoryData* category_data_{nullptr};
	const SkillProgressionTypeData* progression_type_{nullptr};
	SkillDevelopmentCost development_cost_{};
	std::vector<StatType::Type> stats_{};
	int profession_bonus_{0};
	int ranks_{0};
	int special_bonus_{0}; /**< Any special bonus for the category, such as from a training package or other source. This is added to the total bonus for the category after calculating the bonus from ranks. */
};

}