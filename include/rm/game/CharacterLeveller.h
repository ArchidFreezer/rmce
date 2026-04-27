#pragma once

#include <PersistentObjectManager.h>
#include <Character.h>
#include <StatType.h>

// Forward declaration to break the circular include with CharacterLevellerSerializer.h
namespace rm::serial {
class CharacterLevellerSerializer;
} // namespace rm::serial

namespace rm::game::character {
using namespace rm::rule;

/**
 * @class CharacterLeveller
 * @brief Class responsible for handling the levelling up process of a character, including the calculation of stat gains and the application of any bonuses or modifiers from training packages, professions, and other sources.
 *
 */
class CharacterLeveller : public GameObject {
	friend class rm::serial::CharacterLevellerSerializer; /**< Serializer class is a friend to allow it access to the private members of this class for serialisation and deserialisation */
public:
	void levelUp(); /**< Level up the character by applying the stat gains and any bonuses or modifiers from training packages, professions, and other sources. This function should be called when the character levels up to apply the changes
	                   to the character's stats and skills. */
	void buildTrainingPackageCosts(); /**< Build the map of training package costs based on the character's current stats and the available training packages. This function calculates the cost for each training package and stores it in the
	                                     `training_package_costs_` map for later use during the levelling process. */
private:
	rm::PersistentObjectManager* object_factory_{nullptr};

	Character* character_; /**< Reference to the character being levelled up. This is used to access and modify the character's stats, skills, and other attributes during the levelling process. */
	std::map<const TrainingPackageData*, int> training_package_costs_{}; /**< A map of training package data pointers to integers representing the cost for each training package. */
	std::set<const TrainingPackageData*> training_packages_{};           /**< Set of training package data pointers representing the training packages taken by the character being levelled. */
	std::set<StatType::Type> stat_gains_{};                              /**< Set of stats requiring a stat gain roll made with levelling options. */
	std::map<const SubcategoriedSkillData*, int> skill_ranks_{};              /**< Skill ranks */
	std::map<const SkillCategoryData*, int> category_ranks_{};           /**< Skill category ranks */
	std::map<const SpellListData*, int> spell_list_ranks_{};                  /**< Spell list ranks */
	std::set<LanguageAbility> language_abilities_;                       /**< Aggregated map of language names to their corresponding LanguageAbility objects for the character being built. */
};

} // namespace rm::game::character
