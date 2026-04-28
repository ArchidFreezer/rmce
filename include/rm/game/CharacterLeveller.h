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
	void levelUp(); /**< Level up the character by applying the stat gains and any bonuses or modifiers from training packages, professions, and other sources. */
	void buildTrainingPackageCosts(); /**< Build the map of training package costs based on the character's current stats and the available training packages. */
	int getExpForLevel(int level);   /**< Get the minimum experience points required to reach a specific level. */
private:
	rm::PersistentObjectManager* object_factory_{nullptr};

	Character* character_; /**< Reference to the character being levelled up. This is used to access and modify the character's stats, skills, and other attributes during the levelling process. */
	int development_points_{0}; /**< Development points available for the character during the levelling process. */
	std::map<const TrainingPackageData*, int> training_package_costs_{}; /**< A map of training package data pointers to integers representing the cost for each training package. */
	std::set<const TrainingPackageData*> training_packages_{};           /**< Set of training package data pointers representing the training packages taken by the character being levelled. */
	std::set<StatType::Type> stat_gains_{};                              /**< Set of stats requiring a stat gain roll made with levelling options. */
	std::map<const SubcategoriedSkillData*, int> skill_ranks_{};              /**< Skill ranks */
	std::map<const SkillCategoryData*, int> category_ranks_{};           /**< Skill category ranks */
	std::map<const SpellListData*, int> spell_list_ranks_{};                  /**< Spell list ranks */
	std::set<LanguageRanks> language_abilities_;                       /**< Aggregated map of language names to their corresponding LanguageRanks objects for the character being built. */
};

} // namespace rm::game::character
