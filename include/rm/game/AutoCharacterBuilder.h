#pragma once

#include <map>
#include <PersistentObjectManager.h>
#include <CharacterBuilder.h>
#include <SpellListType.h>

namespace rm::serial {
class AutoCharacterBuilderSerializer;
} // namespace rm::serial

namespace rm::rule {
class ArmourTypeData;
class CultureTypeData;
class RaceData;
class SkillCategoryData;
class SkillDevelopmentCost;
class SkillGroupData;
class SpellListData;
class SubcategoriedSkillData;
class WeaponTypeData;
} // namespace rm::rule

namespace rm::game::character {
class CharacterBuilder; // Forward declaration to break the circular include with CharacterBuilder.h

using namespace rm::rule;

/**
 * @class AutoCharacterBuilder
 * @brief A helper class that provides a convenient interface for automatically generating a character with random choices for all attributes and stats.
 *
 * The AutoCharacterBuilder class is designed to simplify the process of creating a character by automatically generating random choices for all attributes and stats based on the rules and data defined in the game. It can be used for
 * quickly creating NPCs or for players who want a more randomized character creation experience.
 */
class AutoCharacterBuilder : public GameObject {
	/* We need to make the serializer a friend to allow it access to the private members of this class for serialisation and deserialisation */
	friend class rm::serial::AutoCharacterBuilderSerializer;

public:
	AutoCharacterBuilder() = default; /**< Default constructor */

	/**
	 * @brief Automatically roll stats and assign them to the character being built.
	 *
	 * This method is used to automate the stat generation step. It will roll temporary and potential values and assign them to character stats based on the chosen profession.
	 * The default parameter values are those used for PCs and may be adjusted for NPCs as desired based on whether we are creating MOBs or unique characters.
	 *
	 * @param builder The CharacterBuilder object for which to automatically generate stats. This builder will be modified by this function to set the generated stats for the character being built.
	 * @param min The minimum temporary value to roll for each stat.
	 * @param primeFloorMin The minimum value for the prime stats.
	 * @param numPrimeFloorMin The number of prime floor stats that must meet or exceed the primeFloorMin value.
	 */
	void autoStats(CharacterBuilder& builder, int min = 25, int primeFloorMin = 90, int numPrimeFloorMin = 2) const;

	/**
	 * @brief Get the name of this AutoCharacterBuilder object.
	 *
	 * The name of the object will be either the name of the associated CharacterBuilder object if it has been generated automatically, or may be
	 * @return A string representing the name of this AutoCharacterBuilder object, which is used for display purposes and may not be unique.
	 */
	std::string name() const {
		return name_;
	}

	/**
	 * @brief Set the name of this AutoCharacterBuilder object.
	 *
	 * The name of the object will be either the name of the associated CharacterBuilder object if it has been generated automatically, or may be set to a custom value for display purposes. This function allows for setting a custom name for
	 * the AutoCharacterBuilder object, which can be useful for distinguishing between different automatically generated characters or for providing more descriptive names in certain contexts.
	 *
	 * @param name A string representing the name to set for this AutoCharacterBuilder object, which is used for display purposes and may not be unique.
	 */
	void setName(const std::string& name) {
		name_ = name;
	}

	/**
	 * @brief Define the traits of the character being built.
	 *
	 * This method is used to define the traits of the character being built. The traits are defined on scales of 1 - 9 to allow for a median value of 5 to be used for random generation and to allow for more extreme values to be generated
	 * for unique characters or MOBs if desired. They are initialised to 0 to indicate that they have not been set yet and to allow for the auto generation of these values if they are not set by the caller.
	 *
	 * The values are used for weighting the skill selection rather than stats as the stats are more of a means to an end for the character's abilities and the skills are what define the character's playstyle and role in the game, so it
	 * makes more sense to use these values to weight the skill selection rather than the stat selection.
	 */
	void ensureTraits();

	/**
	 * @brief Automatically select initial choices for the character being built based on the traits defined in this AutoCharacterBuilder object.
	 *
	 * This methos deals with the base choices from the porofession and  culture of the character being built. The choices are governed by the traits defined in this AutoCharacterBuilder object, which are used to weight the selection of
	 * skills and other choices for the character being built. The traits are defined on scales of 1 - 9 to allow for a median value of 5 to be used for random generation and to allow for more extreme values to be generated for unique
	 * characters or MOBs if desired. They are initialised to 0 to indicate that they have not been set yet and to allow for the auto generation of these values if they are not set by the caller.
	 */
	void autoPrimaryChoices(CharacterBuilder& builder);

private:
	/* Basic data */
	std::string name_{""}; /**< The name of this AutoCharacterBuilder object, which is used for display purposes and may not be unique. */

	/*
	 * Traits of character being built
	 *
	 * These are defined on scales of 1 - 9 to allow for a median value of 5 to be used for random generation and to allow for more extreme values to be generated for unique characters or MOBs if desired. They are initialised to 0 to
	 * indicate that they have not been set yet and to allow for the auto generation of these values if they are not set by the caller.
	 *
	 * The values are used for weighting the skill selection rather than stats as the stats are more of a means to an end for the character's abilities and the skills are what define the character's playstyle and role in the game, so it
	 * makes more sense to use these values to weight the skill selection rather than the stat selection.
	 */
	int aggression_{0};       /**< An integer representing the aggression level of the character being built. */
	int combat_casting_{0};   /**< An integer representing whether the character prefers to use spells in combat (higher value) or not (lower value). */
	int combat_closeness_{0}; /**< An integer representing whether the character prefers ranged attacks (low value) or melee (higher value). */
	int focussed_{0};         /**< An integer representing whether the character prefers to focus on a single skill (higher value) or be more of a generalist (lower value). */

	/*
	 * Basic choices such as weapon and arnmour types
	 */
	const SubcategoriedSkillData* preferred_melee_{nullptr};
	const SubcategoriedSkillData* preferred_ranged_{nullptr};
	const ArmourTypeData* preferred_armour_{nullptr};

	/* ------------------------------------------------------------------ */
	/* Helper functions                                                   */
	/* ------------------------------------------------------------------ */

	void setPreferredArmour(CharacterBuilder& builder); /* Set the armour type that the character will focus on */

	void setRaceCategoryEverymanChoices(CharacterBuilder& builder); /* Select the skills that are Everyman from thos defined by the race, populating the builder.race_category_everyman_choices_ member */

	void setCultureTypeCategorySkillRanks(CharacterBuilder& builder); /* Select the culture type category choices, which are weapon choices, populating the builder.culture_type_category_skill_ranks_ member */

	void setProfessionSkillDevelopmentTypes(CharacterBuilder& builder); /* Select the profession skill development type choices, populating the builder.prof_skill_development_type_choices_ member */

	void setProfessionCategoryDevelopmentTypes(CharacterBuilder& builder); /* Select the profession skill development type choices for categories, populating the builder.prof_category_development_type_choices_ member */

	void setProfessionGroupDevelopmentTypes(CharacterBuilder& builder); /* Select the profession skill development type choices for groups, populating the builder.prof_group_development_type_choices_ member */

	void setBaseSpellLists(CharacterBuilder& builder); /* Select the base spell list choices for the character, populating the builder.prof_base_spell_list_choices_ member */

	void allocateWeaponCosts(CharacterBuilder& builder); /* Allocate the weapon costs for the character, updating the builder.category_development_costs_ member */

	std::vector<const SubcategoriedSkillData*> getSubcategoriesForSkill(CharacterBuilder& builder, const SkillData& skill); /* Get the common subcategories for a skill  */
};

std::vector<const SkillData*> getCategorySkills(const SkillCategoryData& category, rm::PersistentObjectManager& object_manager); /* Get the skills included in a category */

std::vector<const SkillData*> getGroupSkills(const SkillGroupData& group, rm::PersistentObjectManager& object_manager); /* Get the skills included in a group */

std::vector<const SkillCategoryData*> getGroupCategories(const SkillGroupData& group, rm::PersistentObjectManager& object_manager); /* Get the skill categories included in a group */

std::vector<const WeaponTypeData*> getSkillWeapons(const SkillData& skill, rm::PersistentObjectManager& object_manager); /* Get the weapon types that use a skill */

std::vector<const SubcategoriedSkillData*> getCultureMountSkills(const CultureTypeData& culture, rm::PersistentObjectManager& object_manager); /* Get the subcategories of the riding skill for the typical mounts of a culture */

std::vector<const SubcategoriedSkillData*> getRaceMountSkills(const RaceData& culture, rm::PersistentObjectManager& object_manager); /* Get the subcategories of the riding skill for the typical mounts of a race */

std::vector<const SpellListData*> getSpellLists(SpellListType::Type type, const std::set<RealmType ::Type>& realms, rm::PersistentObjectManager& object_manager); /* Get the spell lists from the open spell lists of the given type */

void logSkillCategoryCosts(std::map<const SkillDevelopmentCost, int>& category_costs); /* Log the skill category costs for debugging purposes */

void logSkillCategoryWeights(std::multimap<int, const SkillCategoryData*>& category_weights); /* Log the skill category weights for debugging purposes */

} // namespace rm::game::character
