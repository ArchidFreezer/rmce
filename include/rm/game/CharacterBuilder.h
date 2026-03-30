#pragma once

#include <PersistentObjectManager.h>
#include <Character.h>
#include <string>

namespace rm::game::character {
using namespace rm::rule;

/**
 * @class CharacterBuilder
 * @brief Builder class for constructing Character objects with specific attributes and stats.
 *
 * The CharacterBuilder class provides a convenient interface for setting various attributes and stats of a Character object during its construction. It allows for a step-by-step approach to building a character, ensuring that all necessary
 * attributes are set before the character is used in the game.
 */
class CharacterBuilder : public rm::Persistent {
public:
	/**
	 * @brief Build and return a Character object based on the attributes and stats set in the builder.
	 * @return A Character object constructed from the choices selected in the builder.
	 */
	Character& build(rm::PersistentObjectManager& object_factory);

	/**
	 * @brief Set the name for the character being built.
	 * @param name The name to set for the character.
	 */
	void setName(const std::string& name) {
		name_ = name;
	}

	/**
	 * @brief Set the race for the character being built.
	 * @param race The race data to set for the character.
	 */
	void setRace(const RaceData& race) {
		race_ = &race;
	}

	/**
	 * @brief Set the everyman skill category choices for the character being built.
	 * @param everyman_skill_category_choices A set of skill category data pointers to set for the character.
	 */
	void setEverymanSkillCategoryChoices(std::set<const SkillCategoryData*> everyman_skill_category_choices) {
		everyman_skill_category_choices_ = std::move(everyman_skill_category_choices);
	}

private:
	std::string name_{};                                                   /**< The name of the character being built. This is used for display purposes and may not be unique. */
	const RaceData* race_{nullptr};                                        /**< The race data for the character being built. */
	std::set<const SkillCategoryData*> everyman_skill_category_choices_{}; /**< A set of skill category data pointers representing the everyman skill category choices for the character being built. */
};

} // namespace rm::game::character
