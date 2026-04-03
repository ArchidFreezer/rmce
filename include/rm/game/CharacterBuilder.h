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
	Character& build();

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

	/**
	 * @brief Set the stat values for a specific stat type.
	 *
	 * This method is used to set the temporary and potential stat values for a specific stat type during character creation. The temporary value is typically set based on the initial rolls for the character's stats, while the potential
	 * value is calculated based on the temporary value and any racial bonuses or other modifiers. This method allows for setting both values at once, which can be useful during character creation when both values are determined at the same
	 * time.
	 *
	 * @param stat_type The type of stat to set (e.g., strength, dexterity, etc.).
	 * @param temp_value The temporary stat value to set for the specified stat type.
	 * @param potential_value The potential stat value to set for the specified stat type.
	 */
	void setStat(StatType::Type stat_type, int temp_value, int potential_value);

private:
	rm::PersistentObjectManager* object_factory_{nullptr};

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
	int num_adolescent_language_ranks_{0};         /**< An integer representing the number of adolescent language ranks for the character being built, which may be determined by the culture type. */
	int num_hobby_skill_ranks_{0};                 /**< An integer representing the number of hobby skill ranks for the character being built, which may be determined by the culture type. */
	int num_spell_list_ranks_{0};                  /**< An integer representing the number of spell list ranks for the character being built, which may be determined by the culture type. */

	/* ------------------------------------------------------------------ */
	/* Choices made                                                       */
	/* ------------------------------------------------------------------ */

	/*
	 * These are stored so that they can be applied to the character being built when the build method is called. They are stored separately from the aggregated state to allow for undoing of choices and to keep track of the specific choices
	 * made during character creation.
	 */

	std::set<const SkillCategoryData*> race_category_everyman_choices_{};              /**< A set of skill category data pointers representing the everyman skill category choices for the character being built. */
	std::map<std::string, const LanguageAbility> race_adolescent_language_choices_;    /**< Map of language names to their corresponding LanguageAbility objects for the character being built. */
	std::map<const SubcategoriedSkillData*, int> culture_type_category_skill_ranks_{}; /**< A map of skill category data pointers to integers representing the skill ranks for each skill category choice made during character creation. */
	std::set<SpellListData*> base_spell_list_choices_{};                               /**< A set of spell list data pointers representing the spell list choices for the character being built. */

	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_skill_subcategory_development_type_choices_{}; /**< Skill subcategories with their development type changed */
	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_skill_development_type_choices_{};             /**< Skill (base or subcategory) with their development type changed */
	std::map<const SkillData*, SkillDevelopmentType::Type> prof_category_development_type_choices_{};                       /**< Skills from a category with their development type changed */
	std::map<const SkillData*, SkillDevelopmentType::Type> prof_group_development_type_choices_{};                          /**< Skills from a group with their development type changed */

	std::map<const SubcategoriedSkillData*, int> hobby_skill_ranks_{}; /**< A map of skill data pointers to integers representing the skill ranks for each skill choice made during character creation. */
	std::map<const SkillCategoryData*, int> hobby_category_ranks_{};   /**< A map of skill category data pointers to integers representing the skill ranks for each skill category choice made during character creation. */

	std::map<std::string, const LanguageAbility> background_language_choices_; /**< Map of language names to their corresponding LanguageAbility objects for the character being built. */

	/* ------------------------------------------------------------------ */
	/* Aggregated state                                                   */
	/* ------------------------------------------------------------------ */
	std::map<std::string, LanguageAbility> language_abilities_;                                        /**< Aggregated map of language names to their corresponding LanguageAbility objects for the character being built. */
	std::unordered_map<RealmType::Type, const SkillProgressionTypeData*> realm_progressions_;          /**< Map of realm types to their corresponding SkillProgressionTypeData objects for the character. */
	std::unordered_map<StatType::Type, Stat> stats_;                                                   /**< Map of stat types to their corresponding Stat objects for the character. */
	std::set<const SubcategoriedSkillData*> everyman_skills_{};                                        /**< Skills that are considered everyman */
	std::set<const SubcategoriedSkillData*> restricted_skills_{};                                      /**< Skills that are considered restricted */
	std::set<const SkillCategoryData*> everyman_skill_categories_{};                                   /**< Skill categories that are considered everyman */
	std::set<const SkillCategoryData*> restricted_skill_categories_{};                                 /**< Skill categories that are considered restricted */
	std::map<const SubcategoriedSkillData*, int> skill_ranks_{};                                       /**< Skill ranks */
	std::map<const SubcategoriedSkillData*, int> skill_professional_bonuses_{};                        /**< Skill professional bonuses */
	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> skillsub_development_types_{}; /**< Skill (base or subcategory) with their development type changed */
	std::map<const SkillData*, SkillDevelopmentType::Type> skill_development_types_{};                 /**< Skill (base) with their development type changed */
	std::map<const SkillCategoryData*, int> category_ranks_{};                                         /**< Skill category ranks */
	std::map<const SkillCategoryData*, int> category_professional_bonuses_{};                          /**< Skill category professional bonuses */
	std::map<const SkillCategoryData*, int> category_special_bonuses_{};                               /**< Skill category special bonuses */
	std::map<const SkillCategoryData*, SkillDevelopmentType::Type> category_development_types_{};      /**< Skill categories that all skills within have their development type changed */
	std::map<const SkillGroupData*, int> group_professional_bonuses_{};                                /**< Skill group professional bonuses */
	std::map<const SkillGroupData*, int> group_special_bonuses_{};                                     /**< Skill group special bonuses */
	std::map<const SkillGroupData*, SkillDevelopmentType::Type> group_development_types_{};            /**< Skill groups that all skills within have their development type changed */
	std::map<const SpellListData*, int> spell_list_ranks_{};                                           /**< Spell list ranks */

	/* ------------------------------------------------------------------ */
	/* Helper functions                                                   */
	/* ------------------------------------------------------------------ */

	/**
	 * @brief Resets the builder to its initial state, clearing all choices and aggregated state.
	 *
	 * This is used to clear the builder when setting new initial choices or when recalculating the aggregated state after modifying selections. The `aggregate_state_only` parameter determines whether only the aggregated state should be
	 * cleared or if both the aggregated state and the specific choices made during character creation should be cleared, allowing for a complete reset.
	 *
	 * @param aggregate_state_only If `true`, only clears the aggregated state; if `false`, clears both the aggregated state and any selections made during character creation.
	 */
	void reset(bool aggregate_state_only = true);

	void recalculateAggregatedState(); /**< Recalculates the aggregated state based on the current choices made to ensure consistency when selections are modified. */

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
	 * @brief Determines which of two skill development types has higher precedence.
	 *
	 * When applying a skill development type to a skill, the precedence of the new development type is compared with the existing development type for that skill. The development type with the highest precedence will be applied to the
	 * skill. This ensures that if there are multiple sources of skill development types (e.g., from the profession and from the culture), the most significant one will take effect.
	 *
	 * The order is as follows, from highest precedence to lowest:
	 * 1. Restricted
	 * 2. Occupational
	 * 3. Everyman
	 * 4. Standard
	 *
	 * @param first The first skill development type to compare.
	 * @param second The second skill development type to compare.
	 * @return The skill development type with the highest precedence.
	 */
	SkillDevelopmentType::Type getHighestPrecedenceDevelopmentType(SkillDevelopmentType::Type first, SkillDevelopmentType::Type second) const;

	/**
	 * @brief Applies the effects of the given race data to the character being built.
	 *
	 * This includes setting the race for the character and applying any racial innate abilities and choices associated with the race.
	 *
	 */
	void applyRace();

	void applyRaceChoices(); /**< Applies the choices associated with the character's race. */

	/**
	 * @brief Applies the effects of the given culture type data to the character being built.
	 *
	 * This includes applying any adolescent skill ranks, everyman skill category choices, and adolescent language choices associated with the culture type.
	 *
	 */
	void applyCultureType();

	void applyCultureTypeChoices(); /**< Applies the choices associated with the character's culture type. */

	/**
	 * @brief Applies the effects of the given culture data to the character being built.
	 *
	 * This includes applying any culture-specific choices and abilities associated with the culture.
	 *
	 */
	void applyCulture();

	void applyCultureChoices(); /**< Applies the choices associated with the character's culture. */

	/**
	 * @brief Applies the effects of the given profession data to the character being built.
	 *
	 * This includes applying any profession-specific choices and abilities associated with the profession.
	 *
	 */
	void applyProfession();

	void applyProfessionChoices(); /**< Applies the choices associated with the character's profession. */

	/**
	 * @brief Gets the maximum number of hobby skill ranks that can be allocated to a given skill based on the character's choices and the rules for hobby skill ranks.
	 *
	 * This function calculates the maximum number of hobby skill ranks that can be allocated to a specific skill by considering the character's current choices (e.g., profession, culture type) and the rules governing hobby skill ranks. It
	 * takes into account any limitations or bonuses that may apply to the skill based on the character's background and choices, ensuring that the allocation of hobby skill ranks adheres to the game's character creation rules.
	 *
	 * @param skill The skill for which to calculate the maximum number of hobby skill ranks.
	 * @return The maximum number of hobby skill ranks that can be allocated to the given skill.
	 */
	int getMaxHobbyRanksForSkill(const SubcategoriedSkillData* skill) const;

	/**
	 * @brief Gets the maximum number of hobby skill ranks that can be allocated to a given skill category based on the character's choices and the rules for hobby skill ranks.
	 *
	 * This function calculates the maximum number of hobby skill ranks that can be allocated to a specific skill category by considering the character's current choices (e.g., profession, culture type) and the rules governing hobby skill ranks. It
	 * takes into account any limitations or bonuses that may apply to the skill category based on the character's background and choices, ensuring that the allocation of hobby skill ranks adheres to the game's character creation rules.
	 *
	 * @param category The skill category for which to calculate the maximum number of hobby skill ranks.
	 * @return The maximum number of hobby skill ranks that can be allocated to the given skill category.
	 */
	int getMaxHobbyRanksForCategory(const SkillCategoryData* category) const;

	/**
	 * @brief Gets the set of spell list choices that are available for the character based on their current realm(s).
	 *
	 * During adolescence some cultures allow characters to gain ranks in one open spell list from their own realm as long as the cost of the rank is <= 40 for their profession.
	 *
	 * @return A set of spell list data pointers representing the available spell list choices for the character.
	 */
	std::set<const SpellListData*> getAdolescentSpellListChoices() const;

};

} // namespace rm::game::character
