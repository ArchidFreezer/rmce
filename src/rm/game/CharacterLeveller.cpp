#include <CharacterLeveller.h>
#include <EnumIterator.h>
#include <TrainingPackageCostTable.h>

namespace rm::game::character {

void CharacterLeveller::buildTrainingPackageCosts() {
	{
		using namespace rm::rule::table;
		training_package_costs_.clear();

		std::string id = "TRAININGPACKAGECOSTTABLE_TRAINING_PACKAGE_COST_TABLE";
		TrainingPackageCostTable& table = object_factory_->get<TrainingPackageCostTable>(id);
		for (const auto& training_package_wrapper : object_factory_->getAll<TrainingPackageData>()) {
			const TrainingPackageData* training_package = &training_package_wrapper.get();
			int cost = table.cell(character_->profession(), training_package);
			training_package_costs_.insert_or_assign(training_package, cost);
		}
	}
}

void CharacterLeveller::levelUp() {
	/*
	 * Stat gains
	 */
	// Apply stats gains from purchases or TPs.
	for (auto stat_type : stat_gains_) {
		character_->stats_[stat_type].performStatGainRoll();
	}
	// Once a character is level 1 level ups provide a stat gain roll for all attributes in addition to any purchases.
	if (character_->level_ > 0) {
		for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
			character_->stats_[stat_type].performStatGainRoll();
		}
	}

	/*
	 * Deal with apprenticeship only gains from packages. Gold and specials are not gained once a character reaches level 1 so we only apply these gains if the character is levelling up from level 0 to level 1.
	 */
	if (character_->level_ == 0) {
		for (const TrainingPackageData* training_package : training_packages_) {
			character_->gold_ += training_package->startingMoneyChange();

			// The specials may be items, favours owed, or anything else that doesn't fit into the other categories and may be represented as a string description of the special. We will just add these to the total items list for now but we
			// may want to separate them out into their own list if we want to display them differently in the character sheet or have different rules for how they are used.
			int divisor{1}; // Each time a special is gained the chance is halved so we double the divisor.
			for (const auto& [special, chance] : training_package->specials()) {
				// Short circuit for the last item.
				if (chance == 0) {
					character_->items_.push_back(special);
				} else {
					int roll = archid::Dice(100, 5).rollOpenHigh().result(); // Roll open-ended high d100 for the special item gain as per the rules for apprenticeship training package special gains.
					if (roll + (chance / divisor) >= 100) {
						character_->items_.push_back(special);
						divisor *= 2;
					}
				}
			}
		}
	}

	/*
	 * Deal with skills, categories and languages
	 * The values in the levellers are the total ranks and not the changed ranks so we can just set the character's ranks to these values. This allows us to handle any changes in a single place and simplifies the levelling up process as we
	 * don't have to worry about the order of applying changes or calculating the changes from the previous level.
	 */
	for (const auto& [category, rank] : category_ranks_) {
		Category& char_category = character_->categories_[category];
		char_category.ranks_ = rank;
	}

	for (const auto& [skill, rank] : skill_ranks_) {
		Skill& char_skill = character_->skills_[skill];
		char_skill.ranks_ = rank;
		// Check that the category and progression type are set for the skill, if not we need to set them based on the skill data. This is to handle the case where a skill is gained for the first time at level up and therefore does not have
		// a category or progression type set yet.
		if (char_skill.category_ == nullptr || char_skill.progression_type_ == nullptr) {
			const SkillCategoryData& category_data = skill->skillData().category();
			auto category_it = character_->categories_.find(&category_data);
			if (category_it != character_->categories_.end()) {
				char_skill.category_ = &category_it->second;
				char_skill.progression_type_ = &category_data.defaultSkillProgression();
			}
		}
	}

	for (const auto& [spell_list, rank] : spell_list_ranks_) {
		character_->spell_list_ranks_[spell_list] = rank;
	}

	for (const auto& language_ability : language_abilities_) {
		character_->setLanguageAbility(language_ability);
	}

	/*
	 * Finalize the level up by updating the character level and exp.
	 */
	character_->updateAllDerivedData();
	character_->hits_ = character_->max_hits_;
	character_->power_points_ = character_->max_power_points_;
	character_->level_++;
	character_->experience_points_ = getExpForLevel(character_->level_);
}

int CharacterLeveller::getExpForLevel(int level) {
	int exp{0};
	if (level < 6) {
		exp = 10000 * level;
	} else if (level < 11) {
		exp = 50000 + ((level - 5) * 20000);
	} else if (level < 16) {
		exp = 150000 + ((level - 10) * 30000);
	} else if (level < 21) {
		exp = 300000 + ((level - 15) * 40000);
	} else {
		exp = 500000 + ((level - 20) * 50000);
	}
	return exp;
}

} // namespace rm::game::character