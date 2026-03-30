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
class CharacterBuilder : public rm::game::GameObject {
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
	void setName(const std::string& name);

	/**
	 * @brief Set the race for the character being built.
	 * @param race The race data to set for the character.
	 */
	void setRace(const RaceData& race);

	/**
	 * @brief Set the everyman skill category choices for the character being built.
	 * @param everyman_skill_category_choices A set of skill category data pointers to set for the character.
	 */
	void setEverymanSkillCategoryChoices(std::set<const SkillCategoryData*> everyman_skill_category_choices) {
		everyman_skill_category_choices_ = std::move(everyman_skill_category_choices);
	}

	/**
	 * @brief Set the race adolescent language choices for the character being built as a map of language names to their corresponding LanguageAbility objects.
	 * @param race_adolescent_language_choices A map of language names to their corresponding LanguageAbility objects to set for the character.
	 */
	void setRaceAdolescentLanguageChoices(std::map<std::string, LanguageAbility> race_adolescent_language_choices) {
		race_adolescent_language_choices_ = std::move(race_adolescent_language_choices);
	}

private:
	Character character_;           /**< The Character object being built. This is the object that will be constructed and returned by the build() method after all the attributes and stats have been set. */
	bool built_{false};             /**< Flag to indicate whether the character has already been built. This is used to prevent building the character multiple times, which could lead to inconsistent state or unintended consequences. */
	std::string name_{};            /**< The name of the character being built. This is used for display purposes and may not be unique. */
	const RaceData* race_{nullptr}; /**< The race data for the character being built. */
	std::set<const SkillCategoryData*> everyman_skill_category_choices_{};    /**< A set of skill category data pointers representing the everyman skill category choices for the character being built. */
	std::map<std::string, LanguageAbility> race_adolescent_language_choices_; /**< Map of language names to their corresponding LanguageAbility objects for the character being built. Each character can have multiple language abilities,
	                                                               which represent the languages they can communicate using. */

	/* ------------------------------------------------------------------ */
	/* Helper functions                                                   */
	/* ------------------------------------------------------------------ */
		
	/**
	 * @brief Applies the given language ability to the character being built, ensuring that if the character already has an ability for the same language, the best ranks are retained.
	 * 
	 * Each of the language abilities (spoken, written, and somatic) from the given `LanguageAbility` object will be compared with the existing ability for the same language (if any) on the character being built. The best ranks will be
	 * retained for each ability type, ensuring that the character has the highest possible language proficiency based on the abilities being applied.
	 * 
	 * @param language_ability The language ability to apply.
	 */
	void setBestLanguageAbility(const LanguageAbility& language_ability);

	/**
	 * @brief Applies the effects of the given race data to the character being built.
	 *
	 * This includes setting the race for the character and applying any racial innate abilities and choices associated with the race.
	 *
	 * @param race The race data to apply to the character.
	 */
	void applyRace(const RaceData& race);
};

} // namespace rm::game::character
