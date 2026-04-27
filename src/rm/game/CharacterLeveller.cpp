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
	// First make any stat gain rolls if applicable.
	if (!stat_gains_.empty()) {
		for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
			character_->stats_[stat_type].performStatGainRoll();
		}
	}

	// Next process the training package data that is not already dealt with. The skills, spells, and languages are already applied to the aggregated data so we only need to deal with what is left.
	for (const TrainingPackageData* training_package : training_packages_) {
		// The training package may have some special bonuses that we need to apply such as extra gold or items that are not already accounted for in the aggregated data.
		character_->gold_ += training_package->startingMoneyChange();

		// The specials may be items, favours owed, or anything else that doesn't fit into the other categories and may be represented as a string description of the special. We will just add these to the total items list for now but we may
		// want to separate them out into their own list if we want to display them differently in the character sheet or have different rules for how they are used.
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

}