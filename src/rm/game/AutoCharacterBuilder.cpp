#include <AutoCharacterBuilder.h>
#include <algorithm>
#include <Logger.h>
#include <ranges>
#include <Dice.h>
#include <EnumIterator.h>

namespace rm::game::character {

void AutoCharacterBuilder::autoInitialChoices(CharacterBuilder& builder) {
	if (builder.built_) {
		throw std::runtime_error("CharacterBuilder: Cannot auto initial choices after character has been built.");
	}

	// We randomise the traits if they have not already been initialised.
	ensureTraits();

	// Set any racial everyman skill categories.
	// These define one of more skill categories where a defined number of skills within the category should be defined as Everyman skills and populate the builder.race_category_everyman_choices_ member
	setRaceCategoryEverymanChoices(builder);
	// Set the culture type weapon category/skill choices
	setCultureTypeCategorySkillRanks(builder);



	setPreferredArmour(builder);

	/* Culture choices */
	const CultureData* culture = builder.culture_;
	std::map<const SkillCategoryData*, int> culture_category_skill_ranks = culture->cultureType().skillCategorySkillRanks();
}

void AutoCharacterBuilder::setCultureTypeCategorySkillRanks(CharacterBuilder& builder) {
	// Get any culture preferences for weapon skills
	std::vector<const SkillData*> culture_preferred_weapon_skills;
	for (const WeaponTypeData* weapon_data : builder.culture_->cultureType().preferredWeapons()) {
		for (const SkillData* skill_data : weapon_data->skills()) {
			if (!std::ranges::contains(culture_preferred_weapon_skills, skill_data)) {
				culture_preferred_weapon_skills.push_back(skill_data);
			}
		}
	}
	// We are going to be performing an intersection om the vector so we need to sort it first. We can use the pointer address for the comparison so we can use the basic sort.
	std::ranges::sort(culture_preferred_weapon_skills);

	for (const auto& [skill_category_data, skill_rank] : builder.culture_->cultureType().skillCategorySkillRanks()) {
		std::vector<const SkillData*> category_skills = getCategorySkills(*skill_category_data, *builder.object_factory_);
		int num_category_skills = category_skills.size();
		std::ranges::sort(category_skills);
		std::string chosen_id{};

		// Check for any intersection between the culture preferred weapon skills and the skills in this category. If there is an intersection then we pick one at rindom for now.
		// TODO Perform some weighting based on the character traits.
		std::vector<const SkillData*> intersection;
		std::ranges::set_intersection(culture_preferred_weapon_skills, category_skills, std::back_inserter(intersection));
		int num_intersections = intersection.size();
		if (num_intersections > 0) {
			const SkillData* chosen_skill = intersection[Random::get(0, num_intersections - 1)];
			chosen_id = chosen_skill->id();
			LOG_TRACE("AutoCharacterBuilder: Setting culture type category skill rank choice for {} to {} based on preferred weapon skill {}.", skill_category_data->name(), skill_rank, chosen_skill->name());
		} else {
			const SkillData* chosen_skill = category_skills[Random::get(0, num_category_skills - 1)];
			chosen_id = chosen_skill->id();
			LOG_TRACE("AutoCharacterBuilder: Setting culture type category skill rank choice for {} to {} with no preferred weapon skill {}.", skill_category_data->name(), skill_rank, chosen_skill->name());
		}
		SubcategoriedSkillData* skill_choice = &builder.object_factory_->subcategoriedSkillData(chosen_id);
		builder.culture_type_category_skill_ranks_.emplace(skill_choice, skill_rank);
	}
}

void AutoCharacterBuilder::setRaceCategoryEverymanChoices(CharacterBuilder& builder) {
	// We need to check for any racial everyman skill categories and if so then we need to make choices for those.
	for (const auto& category_choice : builder.race_->categoryEverymanSkillChoices()) {
		// Create the list of skills that are available in the category options to choose from.
		std::vector<const SkillData*> skill_options;
		for (const auto& category : category_choice.options()) {
			skill_options.append_range(getCategorySkills(*category, *builder.object_factory_));
		}

		// Now we need to randomly select the required number of skills from the list of options to be the everyman skills for this category choice and add those to the builder member that tracks the everyman skill
		std::ranges::shuffle(skill_options, Random::mt);
		int num_choices = std::min(static_cast<int>(skill_options.size()), category_choice.numChoices());
		for (int i = 0; i < num_choices; ++i) {
			const SkillData* skill_data = skill_options[i];
			std::string id = skill_data->id();
			SubcategoriedSkillData& choice = builder.object_factory_->subcategoriedSkillData(id);
			builder.race_category_everyman_choices_.insert(&choice);
		}
	}
}

void AutoCharacterBuilder::setPreferredArmour(CharacterBuilder& builder) {
	// We randomise the traits if they have not already been initialised.
	ensureTraits();

	// Create a map to store the weights for each armour type which we seed with a valure of 100.
	std::map<const ArmourTypeData*, int> armour_weights;
	for (const ArmourTypeData& armour_type_data : builder.object_factory_->getAll<ArmourTypeData>()) {
		// We skip animal only armours unless the culture specifies them as they are not typical for characters.
		if (armour_type_data.animalOnly() && !std::ranges::contains(builder.culture_->cultureType().preferredArmour(), &armour_type_data)) {
			continue;
		}
		armour_weights[&armour_type_data] = 100;
	}

	// Before we check professions we need to determine if the profession provides and Transcend Armour skill bonus as this indicates the character is expected to wear armour and be able to cast.
	bool transcend_armour{false};
	for (auto& [skill_category_data, bonus] : builder.profession_->skillBonuses()) {
		if (skill_category_data->id() == "SKILL_TRANSCEND_ARMOR") {
			LOG_TRACE("AutoCharacterBuilder: Profession {} provides Transcend Armour skill bonus.", builder.profession_->name());
			transcend_armour = true;
			break;
		}
	}

	// We need to consider the magic realm to determine the restrictions on metal or inert material.
	if (std::ranges::contains(builder.magical_realms_, RealmType::Type::kEssence) && !transcend_armour) {
		// Essence users cannot use any armour so limit to AT1 (Skin) and AT2 (Robes).
		for (auto& [skill_category_data, weight] : armour_weights) {
			if (skill_category_data->type() != ArmourType::kAT1 && skill_category_data->type() != ArmourType::kAT2) {
				LOG_TRACE("AutoCharacterBuilder: Removing {} for Essence caster character with no Transcend Armour bonus.", skill_category_data->name());
				armour_weights.erase(skill_category_data);
			}
		}
	} else if (std::ranges::contains(builder.magical_realms_, RealmType::Type::kChanneling) && !transcend_armour) {
		// Channeling users struggle to cast spells in metal armour so AT13 and above are removed.
		for (auto& [skill_category_data, weight] : armour_weights) {
			for (ArmourType::Type armour_type : archid::enum_range(ArmourType::kAT13, ArmourType::kAT20)) {
				if (skill_category_data->type() == armour_type) {
					LOG_TRACE("AutoCharacterBuilder: Removing {} for Channeling caster character with no Transcend Armour bonus.", skill_category_data->name());
					armour_weights.erase(skill_category_data);
				}
			}
		}
	}

	// Reduce the weight of armours with a missile attack penalty if the character is not expected to be in close combat and is not using combat casting as they are more likely to want to avoid the missile attack penalty.
	if (combat_casting_ < 3 || combat_closeness_ < 4) {
		for (const auto& [armour_type_data, weight] : armour_weights) {
			LOG_TRACE("AutoCharacterBuilder: Reducing weight of {} with missile attack penalty of {} for ranged character.", armour_type_data->name(), armour_type_data->missileAttackPenalty());
			armour_weights[armour_type_data] -= armour_type_data->missileAttackPenalty();
		}
	}

	// Now check for any preferred armours from the culture that are still available and if so then weigh the options towards those.
	for (const ArmourTypeData* preferred_armour : builder.culture_->cultureType().preferredArmour()) {
		LOG_TRACE("AutoCharacterBuilder: Increasing weight of preferred {} for character culture.", preferred_armour->name());
		armour_weights[preferred_armour] *= 10; // Heavily weight culture preferred armours.
	}

	// If the character has high Quickness bonus weight armour in favour of those with a smaller penalty unless going full melee.
	if (aggression_ < 7 && combat_closeness_ < 7) {
		int quickness_db_bonus = builder.stats_[StatType::kQuickness].bonus() * 3;
		if (quickness_db_bonus > 0) {
			for (const auto& [armour_type_data, weight] : armour_weights) {
				if (armour_type_data->quicknessPenalty() > 0) {
					LOG_TRACE("AutoCharacterBuilder: Decreasing weight of {} with Qui penalty of {} for character with Qui DB bonus of {}.", armour_type_data->name(), armour_type_data->quicknessPenalty(), quickness_db_bonus);
					int armour_actual_penalty = std::max(0, armour_type_data->quicknessPenalty() - quickness_db_bonus);
					armour_weights[armour_type_data] -= armour_actual_penalty * 2;
				}
			}
		}
	}

	/* Pick a random AT from those available */
	int sum_weights{0};
	for (const auto& [armour_type_data, weight] : armour_weights) {
		LOG_TRACE("AutoCharacterBuilder: Weight: {} - {}.", armour_type_data->name(), weight);
		sum_weights += weight;
	}
	int random_weight{archid::Dice(sum_weights).roll().result()};
	int current_weight{0};
	for (const auto& [armour_type_data, weight] : armour_weights) {
		current_weight += weight;
		if (random_weight < current_weight) {
			LOG_TRACE("AutoCharacterBuilder: Selected preferred armour type {} for character with random weight {} and current weight {}.", armour_type_data->name(), random_weight, current_weight);
			preferred_armour_ = armour_type_data;
			return;
		}
	}
}

void AutoCharacterBuilder::ensureTraits() {
	// We randomise these if they have not already been initialised.
	if (!aggression_) {
		aggression_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set aggression to {}.", aggression_);
	}
	if (!combat_casting_) {
		combat_casting_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set combat casting to {}.", combat_casting_);
	}
	if (!combat_closeness_) {
		combat_closeness_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set combat closeness to {}.", combat_closeness_);
	}
	if (!focussed_) {
		focussed_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set focussed to {}.", focussed_);
	}
}

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
		if (std::ranges::contains(builder.profession_->stats(), stat_type)) {
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
			stat_weights[stat_type] += 5; // Semi spell usres should only have a single magical stat.
		}
		stat_weights[StatType::kConstitution] += 5; // Semi spell users still want to be somewhat competent in combat so we give a small boost to their hits to help with that.
		break;
	}
	case SpellUserType::kNone: {
		// Boost the body development stats for non-spell users to ensure they have a good pool of hit points that will help them survive in combat.
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
	std::ranges::shuffle(medium_stat_types, Random::mt);
	for (size_t i = 0; i < medium_stat_types.size(); ++i) {
		builder.setInitialStat(medium_stat_types[i], medium_stats[i].temporary(), medium_stats[i].potential());
	}
	std::ranges::shuffle(low_stat_types, Random::mt);
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

std::vector<const SkillData*> getCategorySkills(const SkillCategoryData& category, PersistentObjectManager& object_manager) {
	std::vector<const SkillData*> category_skills;
	for (const SkillData& skill : object_manager.getAll<SkillData>()) {
		if (&skill.category() == &category) {
			category_skills.emplace_back(&skill);
		}
	}
	return std::move(category_skills);
}

} // namespace rm::game::character