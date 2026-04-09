#pragma once

#include <PersistentObjectManager.h>
#include <Character.h>
#include <StatType.h>
#include <string>
#include <map>
#include <vector>
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
	 * @brief Reset the aggreagrated state of the builder to its initial state, then apply any choices that have been made to recalculate the aggregated state of the builder based on the current choices.
	 *
	 * This function allows the choices to be modified and reapplied safely. For example, if the player changes their background choice after making some other choices, the aggregated state can be reset and recalculated to reflect the new
	 * background choice and its effects on the character's stats and abilities without needing to reset all the choices
	 *
	 */
	void recalculateAggregatedState();

	/**
	 * @brief Sets the primary choices for character creation, including name, race, culture, profession, and magical realm(s).
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
	void setPrimaryDefinition(rm::PersistentObjectManager& object_factory, const std::string& name, const std::string& race_id, const std::string& culture_id, const std::string& profession_id,
	                          const std::set<RealmType::Type> magical_realms);

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
	void setInitialStat(StatType::Type stat_type, int temp_value, int potential_value);

	/**
	 * @brief Add a number of ranks to a skill chosen as a hobby skill during character creation.
	 *
	 * @param skill The skill data to add ranks to.
	 * @param ranks The number of ranks to add to the specified skill.
	 */
	void addHobbySkillRankChoice(const SubcategoriedSkillData& skill, int ranks);

	/**
	 * @brief Add a number of ranks to a skill category chosen as a hobby skill category during character creation.
	 *
	 * @param category The skill category data to add ranks to.
	 * @param ranks The number of ranks to add to the specified skill category.
	 */
	void addHobbyCategoryRankChoice(const SkillCategoryData& category, int ranks);

	/**
	 * @brief Add a language choice for the character during character creation.
	 *
	 * This method is used to add a language choice for the character being built. The language choice is represented by a LanguageAbility object, which contains information about the language and the number of ranks in spoken, written,
	 * and somatic components of the language. This method allows for adding multiple language choices for the character, which can be important for characters with high culture or specific backgrounds.
	 *
	 * @param language The LanguageAbility object representing the language choice to add for the character.
	 */
	void addAdolescentLanguageChoice(const LanguageAbility language);

	/**
	 * @ Set the spell list choice for the character during character creation.
	 *
	 * This method is used to set an adolescent spell list choice for the character being built. Only some cultures allow for adolescent spell list choices and the ranks all go into a single spell list.
	 *
	 * @param spell_list The SpellListData object representing the spell list choice to add for the character.
	 */
	void setAdolescentSpellListChoice(const SpellListData& spell_list);

	/**
	 * @brief Makes background stat gain rolls for all stats for the character being built.
	 */
	void makeBackgroundStatGainRolls();

	/**
	 * @brief Makes a background money roll for the character being built.
	 *
	 * This method is used to make a money roll for the character being built when spending a background option point. The roll parameter can be used to specify a specific roll result for testing purposes, or it can be left as the default
	 * value of -1 to indicate that a random roll should be made.
	 *
	 * @param roll An optional integer representing the result of the background money roll. If left as -1, a random roll will be made.
	 */
	void backgroundMoneyRoll(int roll = -1);

	/**
	 * @brief Add a language choice for the character during character creation.
	 *
	 * This method is used to add a language choice for the character being built. The language choice is represented by a LanguageAbility object, which contains information about the language and the number of ranks in spoken, written,
	 * and somatic components of the language. This method allows for adding multiple language choices for the character, which can be important for characters with high culture or specific backgrounds.
	 *
	 * @param language The LanguageAbility object representing the language choice to add for the character.
	 */
	void addBackgroundLanguageChoice(const LanguageAbility language);

	/**
	 * @brief Add a special bonus to a specific skill for the character being built.
	 *
	 * This method is used to add a special bonus to a specific skill for the character being built. The bonus is typically determined by the character's background or other choices made during character creation. This method allows for
	 * adding bonuses to specific skills, which can be important for characters with certain backgrounds or professions.
	 *
	 * @param skill The SubcategoriedSkillData object representing the skill to which the bonus should be applied.
	 * @param bonus An integer representing the amount of the bonus to add to the specified skill.
	 */
	void addBackgroundSkillSpecialBonus(const SubcategoriedSkillData* skill, int bonus);

	/**
	 * @brief Add a special bonus to a specific skill category for the character being built.
	 *
	 * This method is used to add a special bonus to a specific skill category for the character being built. The bonus is typically determined by the character's background or other choices made during character creation. This method
	 * allows for adding bonuses to specific skill categories, which can be important for characters with certain backgrounds or professions.
	 *
	 * @param category The SkillCategoryData object representing the skill category to which the bonus should be applied.
	 * @param bonus An integer representing the amount of the bonus to add to the specified skill category.
	 */
	void addBackgroundCategorySpecialBonus(const SkillCategoryData* category, int bonus);

	/**
	 * @brief Generate a number of random items for the character based on their background.
	 *
	 * This method is used to generate a number of random items for the character being built based on their background. The number of items to generate is determined by the item_count parameter, and the specific items generated may be
	 * influenced by the character's background and other choices made during character creation.
	 *
	 * @param item_count An integer representing the number of random items to generate for the character.
	 */
	void generateBackgroundItems(int item_count);

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
	int num_hobby_skill_ranks_{0};                 /**< An integer representing the number of hobby skill ranks for the character being built, which may be determined by the culture type. */
	int num_adolescent_language_ranks_{0};         /**< An integer representing the number of adolescent language ranks for the character being built, which may be determined by the culture type. */
	int num_adolescent_spell_list_ranks_{0};       /**< An integer representing the number of adolescent spell list ranks for the character being built, which may be determined by the culture type. */
	int development_points_{0};                    /**< An integer representing the number of development points available for the character to spend during their apprenticeship. */

	/* ------------------------------------------------------------------ */
	/* Choices made                                                       */
	/* ------------------------------------------------------------------ */

	/*
	 * These are stored so that they can be applied to the character being built when the build method is called. They are stored separately from the aggregated state to allow for undoing of choices and to keep track of the specific choices
	 * made during character creation.
	 */

	/* Primary choices */
	// Race
	std::set<const SubcategoriedSkillData*> race_category_everyman_choices_{}; /**< A set of skill category data pointers representing the everyman skill category choices for the character being built. */
	// Culture type
	std::map<const SubcategoriedSkillData*, int> culture_type_category_skill_ranks_{}; /**< A map of skill category data pointers to integers representing the skill ranks for each skill category choice made during character creation. */
	// Culture
	std::map<const SubcategoriedSkillData*, int> hobby_skill_rank_choices_{}; /**< A map of skill data pointers to integers representing the skill ranks for each skill choice made during character creation. */
	std::map<const SkillCategoryData*, int> hobby_category_rank_choices_{};          /**< A map of skill category data pointers to integers representing the skill ranks for each skill category choice made during character creation. */
	// Profession
	std::map<const SkillCategoryData*, rm::game::character::SkillDevelopmentCost> weapon_development_cost_choices_{}; /**< Cost to purchase ranks for weapon skill category */
	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_skill_development_type_choices_{};       /**< Skill (base or subcategory) with their development type changed */
	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_category_development_type_choices_{};    /**< Skills from a category with their development type changed */
	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_group_development_type_choices_{};       /**< Skills from a group with their development type changed */
	std::set<const SpellListData*> prof_base_spell_list_choices_{};           /**< A set of spell list data pointers representing the spell list choices for the character being built. */
	std::set<const SpellListData*> adolescent_spell_list_choices_{};                                                  /**< A set of spell list data pointers representing the adolescent spell list choices for the character being built. */

	/* Stats generation */
	std::unordered_map<StatType::Type, Stat> initial_stats_{}; /**< Map of stat types to their corresponding Stat objects for the character. */

	/* Hobby/Adolescent choices */
	std::map<const SubcategoriedSkillData*, int> hobby_skill_ranks_{}; /**< A map of skill data pointers to integers representing the skill ranks for each skill choice made during character creation. */
	std::map<const SkillCategoryData*, int> hobby_category_ranks_{};   /**< A map of skill category data pointers to integers representing the skill ranks for each skill category choice made during character creation. */
	const SpellListData* adolescent_spell_list_choice_{};              /**< A pointer to a SpellListData object representing the spell list choice for the character being built, which may be determined by the culture type. */
	std::set<LanguageAbility> adolescent_language_choices_{};          /**< A set of language abilities representing the hobby language choices for the character being built. */

	/* Background choices */
	std::unordered_map<StatType::Type, Stat> background_stats_{};                     /**< Map of stats, oonly populated if stat gain roll made with background options. */
	int background_extra_gold_{0};                                                    /**< An integer representing the amount of gold the character being built starts with. */
	std::set<LanguageAbility> background_language_choices_{};                         /**< A set of language abilities representing the background language choices for the character being built. */
	std::map<const SubcategoriedSkillData*, int> background_skill_special_bonuses_{}; /**< Skill special bonuses */
	std::map<const SkillCategoryData*, int> background_category_special_bonuses_{};   /**< Skill category special bonuses */
	std::vector<std::string> background_items_{};                                     /**< Items the character genetrates with background options */

	/* ------------------------------------------------------------------ */
	/* Aggregated state                                                   */
	/* ------------------------------------------------------------------ */
	int total_gold_{0};                            /**< An integer representing the total amount of gold the character being built starts with, including any background extra gold and any gold from items or other sources. */
	std::set<LanguageAbility> language_abilities_; /**< Aggregated map of language names to their corresponding LanguageAbility objects for the character being built. */
	std::unordered_map<RealmType::Type, const SkillProgressionTypeData*> realm_progressions_;                    /**< Map of realm types to their corresponding SkillProgressionTypeData objects for the character. */
	std::unordered_map<StatType::Type, Stat> stats_{};                                                           /**< Map of stat types to their corresponding Stat objects for the character. */
	std::map<const SubcategoriedSkillData*, int> skill_ranks_{};                                                 /**< Skill ranks */
	std::map<const SubcategoriedSkillData*, int> skill_professional_bonuses_{};                                  /**< Skill professional bonuses */
	std::map<const SubcategoriedSkillData*, int> skill_special_bonuses_{};                                       /**< Skill special bonuses */
	std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> skill_development_types_{};              /**< Skill (base or subcategory) with their development type changed */
	std::map<const SkillCategoryData*, int> category_ranks_{};                                                   /**< Skill category ranks */
	std::map<const SkillCategoryData*, int> category_professional_bonuses_{};                                    /**< Skill category professional bonuses */
	std::map<const SkillCategoryData*, int> category_special_bonuses_{};                                         /**< Skill category special bonuses */
	std::map<const SkillCategoryData*, rm::game::character::SkillDevelopmentCost> category_development_costs_{}; /**< Cost to purchase ranks for a skill category */
	std::map<const SkillGroupData*, int> group_professional_bonuses_{};                                          /**< Skill group professional bonuses */
	std::map<const SkillGroupData*, int> group_special_bonuses_{};                                               /**< Skill group special bonuses */
	std::map<const SpellListData*, int> spell_list_ranks_{};                                                     /**< Spell list ranks */
	std::vector<std::string> total_items_{};                                                                     /**< Items the character starts with */

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
	 * @param clear_stats If `true`, also clears the stats in the aggregated state. This is only used when fully resetting the builder
	 */
	void reset(bool aggregate_state_only = true, bool clear_stats = false);

	void hardReset(); /**< Fully resets the builder, clearing all choices and aggregated state, including stats. This is used when setting new initial choices to ensure a clean slate for building the character. */

	/**
	 * @brief Applies the given language ability to the character being built, ensuring that if the character already has an ability for the same language, the best ranks are retained.
	 *
	 * Each of the language abilities (spoken, written, and somatic) from the given `LanguageAbility` object will be compared with the existing ability for the same language (if any) on the character being built. The best ranks will be
	 * retained for each ability type, ensuring that the character has the highest possible language proficiency based on the abilities being applied.
	 *
	 * @param language_ability The language ability to apply.
	 */
	void applyLanguageAbility(const LanguageAbility& language_ability);

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

	void applyPrimaryDependents(); /**< Applies the effects that are dependent on multiple primary choices being set */

	void applyHobbyChoices(); /**< Applies the hobby choices made during character creation to the character being built. */

	void applyBackgroundChoices(); /**< Applies the background choices made during character creation to the character being built. */

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
	 * This function calculates the maximum number of hobby skill ranks that can be allocated to a specific skill category by considering the character's current choices (e.g., profession, culture type) and the rules governing hobby skill
	 * ranks. It takes into account any limitations or bonuses that may apply to the skill category based on the character's background and choices, ensuring that the allocation of hobby skill ranks adheres to the game's character creation
	 * rules.
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

	/**
	 * @brief Calculates the number of development points available for the character to spend during their apprenticeship based on their choices and the rules for development points.
	 *
	 * This function calculates the total number of development points available to the character based on the stats parameter. This allows for the calculation to be made on either thae base stats or with any background stat gain rolls.
	 *
	 * @param stats The map of stat types to their corresponding Stat objects for the character, which may be used in the calculation of development points.
	 */
	void calculateDevelopmentPoints(std::unordered_map<StatType::Type, Stat>& stats);
};

} // namespace rm::game::character
