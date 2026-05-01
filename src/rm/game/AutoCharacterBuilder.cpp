#include <AutoCharacterBuilder.h>
#include <algorithm>
#include <EnumIterator.h>
#include <Logger.h>

namespace rm::game::character {

void AutoCharacterBuilder::autoStats(CharacterBuilder& builder, int min, int primeFloorMin, int numPrimeFloorMin) const {
	if (builder.built_) {
		throw std::runtime_error("CharacterBuilder: Cannot auto stats after character has been built.");
	}

	builder.initial_stats_.clear(); // Clear any existing stats to ensure that there is no double-dippiong of the stat rolls if the function is called multiple times during the character building process.

	/*
	 * First we roll the stats and give them relative values to determine which are the 'best' rolls to help us choose where to assign them.
	 */
	std::vector<int> temp_stats;
	for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
		temp_stats.push_back(0);
	}
	ensureValidTemporaryStats(temp_stats, min, primeFloorMin, numPrimeFloorMin);

	// Create the Stat objects and give them a relative weighting based on the temporary/potential values
	std::multimap<int, Stat> stats;
	for (int temp_stat : temp_stats) {
		Stat stat;
		// Set the temporary and generate the potential
		stat.setTemporary(temp_stat);
		// We take the potential into account for the weighting as we are expecting the characters to gain a few levels at the end so the stat gains should push the temprary up before the character is actually used.
		int weight{stat.temporary() + (stat.potential() * 6 / 10)};
		stats.emplace(weight, stat);
	}

	/*
	 * If we have no profession yet then assign the stats randomly
	 */
	if (builder.profession_ == nullptr) {
		// 1. Extract values from the map into a vector
		std::vector<Stat> stat_vector;
		for (const auto& [weight, stat] : stats) {
			stat_vector.push_back(stat);
		}

		// 2. Shuffle the vector to randomize the stat assignment
		std::shuffle(stat_vector.begin(), stat_vector.end(), Random::mt);

		// 3. Assign the stats to the stat types
		int index = 0;
		for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
			builder.setInitialStat(stat_type, stat_vector[index].temporary(), stat_vector[index].potential());
			index++;
		}
		return;
	}

	/*
	 * Now we need to determine which stats to assign them to so we give weights to each stat based on the profession and other factors.
	 *
	 * The 'optimal' assignment is based on the prime stats of the profession, whether the profession is combat focused or not, and the development stats.
	 */
	std::map<StatType::Type, int> stat_weights;
	for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
		stat_weights[stat_type] = 0;
		if (std::find(builder.profession_->stats().begin(), builder.profession_->stats().end(), stat_type) != builder.profession_->stats().end()) {
			stat_weights[stat_type] += 10; // Prime stats get a big boost to ensure they are prioritized in the assignment
		}
		if (StatType::isDevelopment(stat_type)) {
			stat_weights[stat_type] += 3; // Development stats get a boost but should not be a major priority.
		}
	}

	// We give a boost to the hits/pp stats for the type of combat expected
	switch (builder.profession_->spellUserType()) {
	case SpellUserType::kPure:
	case SpellUserType::kHybrid:
	case SpellUserType::kChaotic: {
		std::set<StatType::Type> realm_stats{};
		for (RealmType::Type realm : builder.magical_realms_) {
			for (StatType::Type stat_type : rm::rule::enums::StatType::statsForRealm(realm)) {
				realm_stats.insert(stat_type);
			}
		}
		for (StatType::Type stat_type : realm_stats) {
			stat_weights[stat_type] += 7; // Give a significant boost to the stats that govern spellcasting for the character's realms to ensure they have a good spell point pool to work with.
		}
		break;
	}
	case SpellUserType::kSemi: {
		std::set<StatType::Type> semi_realm_stats{};
		for (RealmType::Type realm : builder.magical_realms_) {
			for (StatType::Type stat_type : rm::rule::enums::StatType::statsForRealm(realm)) {
				semi_realm_stats.insert(stat_type);
			}
		}
		for (StatType::Type stat_type : semi_realm_stats) {
			stat_weights[stat_type] += 5; // Semi spell usres hsould only have a single magical stat.
		}
		stat_weights[StatType::kConstitution] += 5; // Semi spell users still want to be somewhat competent in combat so we give a small boost to agility to help with that.
		break;
	}
	case SpellUserType::kNone: {
		// Boost the body development stats for non-spell users to ensure they have a good pool of development points to spend on combat skills and other abilities that will help them survive in combat.
		stat_weights[StatType::kConstitution] += 5;
		stat_weights[StatType::kSelfDiscipline] += 5;
		break;
	}
	default:
		break;
	}

	/*
	 * Now we have 2 weighted maps, one with the rolled stats and one with the stats types, so we can assign the rolled stats to the stat types based on the weights. We do this by sorting both maps by their weights and then assigning the
	 * highest rolled stat to the highest weighted stat type, the second highest rolled stat to the second highest weighted stat type, and so on. IN order to add some variance we only do this for the top number and then assign the rest
	 * randomly to ensure that we don't end up with the same stat always being the highest for a given profession which would make the character creation process less fun and more predictable.
	 */

	// First we need to reverse the rolled stats map to be weight -> stat type so we can sort it by weight and assign the highest rolled stat to the highest weighted stat type.
	std::multimap<int, StatType::Type> weighted_stat_types;
	for (const auto& [stat_type, weight] : stat_weights) {
		weighted_stat_types.insert({weight, stat_type});
	}
	// Now we can assign the rolled stats to the stat types based on the weights. We do this by iterating through the rolled stats in descending order and assigning them to the stat types in descending order of weight.
	// We know we have 10 of each so we can just iterate through them in order before we decide to go random
	int num_optimal_assignments = builder.pc_ ? 4 : 1; // These get placed in optimal order
	int num_medium_assignments = builder.pc_ ? 3 : 4;
	std::vector<StatType::Type> medium_stat_types;
	std::vector<Stat> medium_stats;
	int num_low_assignments = builder.pc_ ? 3 : 5;
	std::vector<StatType::Type> low_stat_types;
	std::vector<Stat> low_stats;

	int count{0};
	auto rolled_stat_it = stats.rbegin();
	auto weighted_stat_type_it = weighted_stat_types.rbegin();
	for (int i = 0; i < 10; ++i, ++rolled_stat_it, ++weighted_stat_type_it) {
		if (i < num_optimal_assignments) {
			builder.setInitialStat(weighted_stat_type_it->second, rolled_stat_it->second.temporary(), rolled_stat_it->second.potential());
		} else if (i < num_optimal_assignments + num_medium_assignments) {
			StatType::Type stat_type = weighted_stat_type_it->second;
			Stat stat = rolled_stat_it->second;
			medium_stat_types.push_back(stat_type);
			medium_stats.push_back(stat);
		} else {
			StatType::Type stat_type = weighted_stat_type_it->second;
			Stat stat = rolled_stat_it->second;
			low_stat_types.push_back(stat_type);
			low_stats.push_back(stat);
		}
	}
	// Now we randomize the medium and low assignments to add some variance to the stat assignment process and then assign them to the remaining stat types.
	std::shuffle(medium_stat_types.begin(), medium_stat_types.end(), Random::mt);
	for (size_t i = 0; i < medium_stat_types.size(); ++i) {
		builder.setInitialStat(medium_stat_types[i], medium_stats[i].temporary(), medium_stats[i].potential());
	}
	std::shuffle(low_stat_types.begin(), low_stat_types.end(), Random::mt);
	for (size_t i = 0; i < low_stat_types.size(); ++i) {
		builder.setInitialStat(low_stat_types[i], low_stats[i].temporary(), low_stats[i].potential());
	}

	// Debugging output to check the assigned stats and weights
	LOG_DEBUG("\nAssigned stats:");
	LOG_DEBUG("{:-<{}}", "", 45);
	LOG_DEBUG("| {:<17} | {:^5} | {:^5} | {:^5} |", "Stat", "Temp", "Pot", "Wgt");
	LOG_DEBUG("{:-<{}}", "", 45);
	for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
		const Stat& stat = builder.initial_stats_[stat_type];
		int weight = stat_weights[stat_type];
		LOG_DEBUG("| {:<17} | {:^5} | {:^5} | {:^5} |", StatType::toString(stat_type), stat.temporary(), stat.potential(), weight);
	}
	LOG_DEBUG("{:-<{}}\n", "", 45);
}

} // namespace rm::game::character