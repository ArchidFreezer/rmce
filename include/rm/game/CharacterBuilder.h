#pragma once

#include <PersistentObjectManager.h>
#include <Character.h>
#include <StatType.h>
#include <string>
#include <map>
#include <set>
#include <unordered_map>

// Forward declaration to break the circular include with CharacterBuilderSerializer.h
namespace rm::serial {
class CharacterBuilderSerializer;
} // namespace rm::serial

namespace rm::game::character {
using namespace rm::rule;

/**
 * @class CharacterBuilder
 * @brief Builder class for constructing Character objects with specific attributes and stats.
 *
 * The CharacterBuilder class provides a convenient interface for setting various attributes and stats of a Character object during its construction. It allows for a step-by-step approach to building a character, ensuring that all necessary
 * attributes are set before the character is used in the game.
 */
class CharacterBuilder : public GameObject {
	friend class rm::serial::CharacterBuilderSerializer; /**< Serializer class is a friend to allow it access to the private members of this class for serialisation and deserialisation */

public:
	/**
	 * @brief Build and return a Character object based on the attributes and stats set in the builder.
	 * @return A Character object constructed from the choices selected in the builder.
	 */
	Character& build(rm::PersistentObjectManager& object_factory);

	/**
	 * @brief Sets the initial choices for character creation, including name, race, culture, profession, and magical realm(s).
	 *
	 * These are the primary choices that define the character that are made first and drive the choices available for the rest of the character creation process. The name is used for display purposes and may not be unique. The
	 * object factory is used to retrieve the necessary data for the character's choices.
	 *
	 * @param object_factory The object manager used to retrieve the necessary data for the character's choices.
	 * @param name The name of the character.
	 * @param race_id The identifier for the character's race.
	 * @param culture_id The identifier for the character's culture.
	 * @param profession_id The identifier for the character's profession.
	 * @param magical_realms The set of magical realms for the character. This may be fixed by the profession or for non-magical professions decided by the player from the available realm choices for the character.
	 */
	void setIntialChoices(rm::PersistentObjectManager& object_factory, const std::string& name, const std::string& race_id, const std::string& culture_id, const std::string& profession_id, const std::set<RealmType::Type> magical_realms);

private:
	/* ------------------------------------------------------------------ */
	/* Basic data                                                         */
	/* ------------------------------------------------------------------ */
	bool built_{false};             /**< Flag to indicate whether the character has already been built. This is used to prevent building the character multiple times, which could lead to inconsistent state or unintended consequences. */
	std::string name_{};            /**< The name of the character being built. This is used for display purposes and may not be unique. */
	const RaceData* race_{nullptr}; /**< The race data for the character being built. */
	const CultureData* culture_{nullptr};          /**< The culture data for the character being built. */
	const CultureTypeData* culture_type_{nullptr}; /**< The culture type data for the character being built. This is derived from the culture and may be used for certain choices during character creation. */
	const ProfessionData* profession_{nullptr};    /**< The profession data for the character being built. */
	std::set<RealmType::Type> magical_realms_{};   /**< A set of realm types representing the magical realm choices for the character being built. */

	/* ------------------------------------------------------------------ */
	/* Choices made                                                       */
	/* ------------------------------------------------------------------ */

	/*
	 * These are stored so that they can be applied to the character being built when the build method is called. They are stored separately from the aggregated state to allow for undoing of choices and to keep track of the specific choices
	 * made during character creation.
	 */

	std::set<const SkillCategoryData*> race_category_everyman_choices_{};           /**< A set of skill category data pointers representing the everyman skill category choices for the character being built. */
	std::map<std::string, const LanguageAbility> race_adolescent_language_choices_; /**< Map of language names to their corresponding LanguageAbility objects for the character being built. */

	/* ------------------------------------------------------------------ */
	/* Aggregated state                                                   */
	/* ------------------------------------------------------------------ */
	std::map<std::string, LanguageAbility> language_abilities_;                               /**< Aggregated map of language names to their corresponding LanguageAbility objects for the character being built. */
	std::unordered_map<RealmType::Type, const SkillProgressionTypeData*> realm_progressions_; /**< Map of realm types to their corresponding SkillProgressionTypeData objects for the character. */
	std::unordered_map<StatType::Type, Stat> stats_;                                          /**< Map of stat types to their corresponding Stat objects for the character. */
	std::set<const SubcategoriedSkillData*> everyman_skills_{};                               /**< Skills that are considered everyman */
	std::set<const SubcategoriedSkillData*> restricted_skills_{};                             /**< Skills that are considered restricted */
	std::set<const SkillCategoryData*> everyman_skill_categories_{};                          /**< Skill categories that are considered everyman */
	std::set<const SkillCategoryData*> restricted_skill_categories_{};                        /**< Skill categories that are considered restricted */
	std::map<const SubcategoriedSkillData*, int> skill_bonuses_{};                            /**< Skill bonuses */

	/* ------------------------------------------------------------------ */
	/* Helper functions                                                   */
	/* ------------------------------------------------------------------ */

	void reset(); /**< Resets the builder to its initial state, clearing all choices and aggregated state. This is used to allow for undoing of choices and to start fresh if the character creation process needs to be restarted. */

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
	 */
	void applyRace();
};

} // namespace rm::game::character
