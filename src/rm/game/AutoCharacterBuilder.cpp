#include <AutoCharacterBuilder.h>
#include <algorithm>
#include <Logger.h>
#include <ranges>
#include <Dice.h>
#include <EnumIterator.h>

namespace rm::game::character {

/* --------------------------------------------------------------------------------- */
/* Automate the choices based on the initial data such as profession and culture.    */
/* --------------------------------------------------------------------------------- */
void AutoCharacterBuilder::autoPrimaryChoices(CharacterBuilder& builder) {
	if (builder.built_) {
		throw std::runtime_error("CharacterBuilder: Cannot auto initial choices after character has been built.");
	}

	// We randomise the traits if they have not already been initialised.
	ensureTraits(builder);

	// Set any racial everyman skill categories.
	// These define one of more skill categories where a defined number of skills within the category should be defined as Everyman skills and populate the builder.race_category_everyman_choices_ member
	setRaceCategoryEverymanChoices(builder);
	// Set the culture type weapon category/skill choices, populating the builder.culture_type_category_skill_ranks_ member
	setCultureTypeCategorySkillRanks(builder);
	// Set skills whose development type is chosen based on the profession choice, populating the builder.prof_skill_development_type_choices_ member
	setProfessionSkillDevelopmentTypes(builder);
	// Set skills whose development type is chosen based on the profession category choice, populating the builder.prof_category_development_type_choices_ member
	setProfessionCategoryDevelopmentTypes(builder);
	// Set skills whose development type is chosen based on the profession group choice, populating the builder.prof_group_development_type_choices_ member
	setProfessionGroupDevelopmentTypes(builder);
	// Set the base spell lists for the character based on the profession choice, populating the builder.prof_base_spell_list_choices_ member
	setBaseSpellLists(builder);
	// Update the 7 weapon category costs from the profession choice, populating the builder.weapon_category_costs_ member
	allocateWeaponCosts(builder);

	setPreferredArmour(builder);

	/* Culture choices */
	const CultureData* culture = builder.culture_;
	std::map<const SkillCategoryData*, int> culture_category_skill_ranks = culture->cultureType().skillCategorySkillRanks();
}

void AutoCharacterBuilder::setCultureTypeCategorySkillRanks(CharacterBuilder& builder) {
	builder.culture_type_category_skill_ranks_.clear();

	// Get any culture preferences for individual weapon skills
	std::vector<const SubcategoriedSkillData*> culture_preferred_weapon_skills;
	for (const WeaponTypeData* weapon_data : builder.culture_->cultureType().preferredWeapons()) {
		for (const SkillData* skill_data : weapon_data->skills()) {
			std::string id = skill_data->id();
			SubcategoriedSkillData* subcategoried_skill_data = &builder.object_factory_->subcategoriedSkillData(id, weapon_data->name());
			if (!std::ranges::contains(culture_preferred_weapon_skills, subcategoried_skill_data)) {
				culture_preferred_weapon_skills.push_back(subcategoried_skill_data);
			}
		}
	}

	for (const auto& [skill_category_data, skill_rank] : builder.culture_->cultureType().skillCategorySkillRanks()) {
		std::vector<const SkillData*> category_skills = getCategorySkills(*skill_category_data, *builder.object_factory_);
		if (category_skills.empty()) {
			LOG_WARN("AutoCharacterBuilder: No skills found for culture preferred skill category {}. Skipping.", skill_category_data->name());
			continue;
		}

		// Check for any intersection between the culture preferred weapon skills and the skills in this category. If there is an intersection then we pick one at random for now.
		// TODO Perform some weighting based on the character traits.
		std::vector<const SubcategoriedSkillData*> intersection;
		for (const SubcategoriedSkillData* preferred_weapon_skill : culture_preferred_weapon_skills) {
			if (std::ranges::contains(category_skills, &preferred_weapon_skill->skillData())) {
				intersection.push_back(preferred_weapon_skill);
			}
		}

		int num_intersections = intersection.size();
		if (num_intersections > 0) {
			const SubcategoriedSkillData* subcategoried_skill_data = intersection[Random::get(0, num_intersections - 1)];
			builder.culture_type_category_skill_ranks_.emplace(subcategoried_skill_data, skill_rank);
			LOG_TRACE("AutoCharacterBuilder: Setting {} skill ranks to weapon category {} for culture preferred weapon {}.", skill_rank, skill_category_data->name(), subcategoried_skill_data->subcategory().value());
		} else {
			int num_category_skills = category_skills.size();
			const SkillData* chosen_skill = category_skills[Random::get(0, num_category_skills - 1)];
			std::string skill_id = chosen_skill->id();
			// Pick a random weapon that uses the skill
			std::vector<const WeaponTypeData*> skill_weapons = getSkillWeapons(*chosen_skill, *builder.object_factory_);
			int num_skill_weapons = skill_weapons.size();
			const WeaponTypeData* chosen_weapon = skill_weapons[Random::get(0, num_skill_weapons - 1)];
			SubcategoriedSkillData* subcategoried_skill_data = &builder.object_factory_->subcategoriedSkillData(skill_id, chosen_weapon->name());
			builder.culture_type_category_skill_ranks_.emplace(subcategoried_skill_data, skill_rank);
			LOG_TRACE("AutoCharacterBuilder: Setting {} skill ranks to weapon category {} with no culture preferred weapon {}.", skill_rank, skill_category_data->name(), subcategoried_skill_data->subcategory().value());
		}
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

void AutoCharacterBuilder::setProfessionSkillDevelopmentTypes(CharacterBuilder& builder) {
	for (const auto& [skill_choices, development_type] : builder.profession_->skillDevelopmentTypeChoices()) {
		// We may need to manipulate some of the skill choices so we keep a separate list of the actual options that we will select from.
		std::vector<const SubcategoriedSkillData*> skill_options;
		for (const auto& skill_choice : skill_choices.options()) {
			skill_options.append_range(getSubcategoriesForSkill(builder, skill_choice->skillData()));
		}
		std::ranges::shuffle(skill_options, Random::mt);
		int num_options = std::min(skill_choices.numChoices(), (int)skill_options.size());
		for (int i = 0; i < num_options; ++i) {
			const SubcategoriedSkillData* skill_data = skill_options[i];
			LOG_TRACE("AutoCharacterBuilder: Setting profession skill development type choice for {} to {}.", skill_data->name(), toString(development_type));
			builder.prof_skill_development_type_choices_.emplace(skill_data, development_type);
		}
	}
}

void AutoCharacterBuilder::setProfessionCategoryDevelopmentTypes(CharacterBuilder& builder) {
	for (const auto& [category_choices, development_type] : builder.profession_->skillCategorySkillDevelopmentTypeChoices()) {
		// We may need to manipulate some of the skill choices so we keep a separate list of the actual options that we will select from.
		std::vector<const SubcategoriedSkillData*> skill_options;
		for (const auto& category_choice : category_choices.options()) {
			for (const auto& skill_choice : getCategorySkills(*category_choice, *builder.object_factory_)) {
				skill_options.append_range(getSubcategoriesForSkill(builder, *skill_choice));
			}
		}
		std::ranges::shuffle(skill_options, Random::mt);
		int num_options = std::min(category_choices.numChoices(), (int)skill_options.size());
		for (int i = 0; i < num_options; ++i) {
			const SubcategoriedSkillData* skill_data = skill_options[i];
			LOG_TRACE("AutoCharacterBuilder: Setting profession skill category skill development type choice for {} to {}.", skill_data->name(), toString(development_type));
			builder.prof_category_development_type_choices_.emplace(skill_data, development_type);
		}
	}
}

void AutoCharacterBuilder::setProfessionGroupDevelopmentTypes(CharacterBuilder& builder) {
	for (const auto& [group_choices, development_type] : builder.profession_->skillGroupSkillDevelopmentTypeChoices()) {
		// We may need to manipulate some of the skill choices so we keep a separate list of the actual options that we will select from.
		std::vector<const SubcategoriedSkillData*> skill_options;
		for (const auto& group_choice : group_choices.options()) {
			for (const auto& skill_choice : getGroupSkills(*group_choice, *builder.object_factory_)) {
				skill_options.append_range(getSubcategoriesForSkill(builder, *skill_choice));
			}
		}
		std::ranges::shuffle(skill_options, Random::mt);
		int num_options = std::min(group_choices.numChoices(), (int)skill_options.size());
		for (int i = 0; i < num_options; ++i) {
			const SubcategoriedSkillData* skill_data = skill_options[i];
			LOG_TRACE("AutoCharacterBuilder: Setting profession skill group development type choice for {} to {}.", skill_data->name(), toString(development_type));
			builder.prof_group_development_type_choices_.emplace(skill_data, development_type);
		}
	}
}

void AutoCharacterBuilder::setBaseSpellLists(CharacterBuilder& builder) {
	if (builder.profession_->spellUserType() == SpellUserType::kNone) {
		return; // No spell list choices to make if the profession is not a spell user.
	}

	// Logging buffer for table of spell list choices for debugging purposes.
	std::string buffer;
	buffer.reserve(1024);
	// Header
	buffer += std::format("\n| {:<35} |\n", "Spell List");
	buffer += std::format("| {:-<35} |\n", "");

	// Get the profession list choices
	for (const GameRuleDataChoice spell_list_choice : builder.profession_->baseSpellListChoices()) {
		int num_choices = std::min(spell_list_choice.numChoices(), spell_list_choice.numOptions());
		std::set<const SpellListData*> spell_list_options_set = spell_list_choice.options();
		std::vector<const SpellListData*> spell_list_options(spell_list_options_set.begin(), spell_list_options_set.end());
		std::ranges::shuffle(spell_list_options, Random::mt);
		for (int i = 0; i < num_choices; ++i) {
			const SpellListData* spell_list_data = spell_list_options[i];
			fmt::format_to(std::back_inserter(buffer), "| {:<35} |\n", spell_list_data->name());
			builder.prof_base_spell_list_choices_.emplace(spell_list_data);
		}
	}

	// Pure spell users get 4 more lists from their own realm open/closed lists in addition to the profession list choices.
	if (builder.profession_->spellUserType() == SpellUserType::kPure) {
		std::vector<const SpellListData*> pure_spell_list_options;
		pure_spell_list_options.append_range(getSpellLists(SpellListType::kOpen, builder.profession_->realms(), *builder.object_factory_));
		pure_spell_list_options.append_range(getSpellLists(SpellListType::kClosed, builder.profession_->realms(), *builder.object_factory_));

		std::ranges::shuffle(pure_spell_list_options, Random::mt);
		for (int i = 0; i < 4 && i < (int)pure_spell_list_options.size(); ++i) {
			const SpellListData* spell_list_data = pure_spell_list_options[i];
			fmt::format_to(std::back_inserter(buffer), "| {:<35} |\n", spell_list_data->name());
			builder.prof_base_spell_list_choices_.emplace(spell_list_data);
		}
	}
	LOG_DEBUG("Spell Lists:{}", buffer);
}

void AutoCharacterBuilder::allocateWeaponCosts(CharacterBuilder& builder) const {
	PersistentObjectManager& object_manager = *builder.object_factory_;

	// First get the weapon categories and the current costs. We don't care how they are allocated now we just need the set of costs as we will be reallocating them from scratch.
	int expected_weapon_categories = 7; // We expect there to be 7 weapon categories as of the current game rules
	std::vector<const SkillCategoryData*> weapon_categories = getGroupCategories(object_manager.get<SkillGroupData>("SKILLGROUP_WEAPON"), object_manager);
	if (weapon_categories.size() != expected_weapon_categories) {
		LOG_ERROR("AutoCharacterBuilder: Expected {} Weapon categories, but found {} for profession {}. Skipping weapon cost allocation.", expected_weapon_categories, weapon_categories.size(), builder.profession_->name());
		return;
	}

	// Get references to each of the categories to make the code more readable later. We will use these as keys into a map to store the weights for each category based on the character traits and culture preferences.
	const SkillCategoryData& concussion = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_1_H_CONCUSSION");
	const SkillCategoryData& edged = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_1_H_EDGED");
	const SkillCategoryData& two_handed = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_2_HANDED");
	const SkillCategoryData& missile = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_MISSILE");
	const SkillCategoryData& pole_arms = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_POLE_ARMS");
	const SkillCategoryData& thrown = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_THROWN");
	const SkillCategoryData& missile_artillery = object_manager.get<SkillCategoryData>("SKILLCATEGORY_WEAPON_MISSILE_ARTILLERY");

	// Sort the costs that we have to allocate and store how many categeories can be allocated to each.
	std::map<const SkillDevelopmentCost, int> weapon_category_cost_count;
	for (const SkillCategoryData* weapon_category : weapon_categories) {
		weapon_category_cost_count[builder.profession_->skillCategoryDevelopmentCost(*weapon_category)]++;
	}
	logSkillCategoryCosts(weapon_category_cost_count);

	// Missile Artillery is a very niche category and will never be used for the initial weapon choice as it is only used for siege weapons, so we can remove this from the options and allocate it the worst cost at the end.
	// We need to erase it after we have stored the costs to get all of them.
	std::erase(weapon_categories, &missile_artillery);

	// Store some other useful references for later.
	const SubcategoriedSkillData& transcend_armour_skill = object_manager.subcategoriedSkillData("SKILL_TRANSCEND_ARMOR");
	bool transcend_user = std::ranges::any_of(builder.profession_->skillBonuses(), [&transcend_armour_skill](const auto& skill_bonus) { return skill_bonus.first == &transcend_armour_skill && skill_bonus.second > 0; });

	// Use a map to store the weight for each category for use at the end when we allocate the costs.
	std::map<const SkillCategoryData*, int> weapon_category_weights;

	// We start with the culture preferences as these are more likely to be strong preferences that we want to honour and then we can weight the remaining categories based on the character traits.
	for (const auto& [skill, ranks] : builder.culture_type_category_skill_ranks_) {
		if (std::ranges::contains(weapon_categories, &skill->skillData().category())) {
			int weight = ranks * 50; // We give the culture preferred weapon categories a very high weight.
			LOG_TRACE("AutoCharacterBuilder: Adding weight {} for culture preferred weapon category {} from weapon {}.", weight, skill->skillData().category().name(), skill->subcategory().value());
			weapon_category_weights[&skill->skillData().category()] = weight; // We give the culture preferred weapon categories a very high weight.
		}
	}

	// First pass is based on the caster type of the profession as this will have the biggest impact on the weapon category used.
	SpellUserType::Type spell_user_type = builder.profession_->spellUserType();
	if (spell_user_type == SpellUserType::kPure || spell_user_type == SpellUserType::kHybrid) {
		// Pure casters are more likely to want to use quarterstaffs as they are a good weapon for casters which come under the 2-handed category.
		for (const SkillCategoryData* weapon_category : weapon_categories) {
			int weight = archid::Dice(10).roll().result(); // We give a small random weight to the categories to ensure we don't always end up with the same category at the top.
			if (weapon_category == &two_handed) {
				weight += 150; // We give a high weight to the 2-handed category for casters who are not transcend armour users as they are more likely to want to use quarterstaffs.
			} else if (weapon_category == &missile) {
				weight += 20; // We give a small weight to missile weapons as slings and bows can be good for casters who want to be able to attack from range.
			} else if (weapon_category == &thrown) {
				weight += 10; // We give a small weight to thrown weapons as these are better than melee.
			} else {
				weight += 0; // We give no weight to the other categories as they are not particularly good for casters.
			}
			weapon_category_weights[weapon_category] += weight;
		}
	} else if (spell_user_type == SpellUserType::kSemi || spell_user_type == SpellUserType::kChaotic) {
		// Semi spell users are more likely to prefer ranged weapons unless they have the transcend armour skill that allows them to be effective in melee while still being able to cast.
		if (transcend_user) {
			// We can use any weapon type here as the character has specifically chosen to wear armour and is therefore expecting to be in closer combat.
			for (const SkillCategoryData* weapon_category : weapon_categories) {
				int weight = archid::Dice(10).roll().result(); // We give a small random weight to the categories to ensure we don't always end up with the same category at the top.
				if (weapon_category == &missile) {
					weight += 20; // Missile weapons aren;t a bad choice, just unlikely to be the optimal choice for a semi spell user who is a transcend armour user.
				} else if (weapon_category == &thrown) {
					weight += 10; // Thrown weapons are an unlikely choice for a semi spell user who is a transcend armour user.
				} else {
					weight += 100; // The rest are melee choices and we have no data to diffrentiate between them yet, we may update this in future.
				}
				weapon_category_weights[weapon_category] += weight;
			}
		} else {
			for (const SkillCategoryData* weapon_category : weapon_categories) {
				int weight = archid::Dice(10).roll().result(); // We give a small random weight to the categories to ensure we don't always end up with the same category at the top.
				if (weapon_category == &missile) {
					weight += 100; // Missile weapons are the likeliest choice for semi spell users.
					if (combat_closeness_ < 4) {
						weight += 50; // If the character is more ranged focused then we give an extra weight to missile weapons.
					}
				} else if (weapon_category == &thrown) {
					weight += 50; // We give a small weight to thrown weapons as these are better than melee.
					if (combat_closeness_ > 4 && combat_closeness_ < 8) {
						weight += 50; // If the character is medium ranged focused then we give an extra weight to missile weapons.
					}
				} else {
					weight += 25; // We give no weight to the other categories as they are not particularly good for casters.
					if (combat_closeness_ > 8) {
						weight += 100; // If the character insists on being in melee range then bump this up.
					}
				}
				weapon_category_weights[weapon_category] += weight;
			}
		}
	} else {
		// Non spell users have no reason to prefer one weapon category over another based on their caster type so we give them all a small weight to ensure they are not at the bottom of the allocation order if there are culture
		// preferences.
		for (const SkillCategoryData* weapon_category : weapon_categories) {
			int weight = archid::Dice(10).roll().result(); // We give a small random weight to the categories to ensure we don't always end up with the same category at the top.
			if (weapon_category == &missile) {
				if (combat_closeness_ < 4) {
					weight += 100; // If the character is ranged focused then we give an extra weight to missile weapons.
				}
			} else if (weapon_category == &thrown) {
				if (combat_closeness_ > 4 && combat_closeness_ < 8) {
					weight += 25; // If the character is medium ranged focused then we give some weight to thrown weapons, but they are niche.
				}
			} else {
				weight += 50; // The melee categories all get a decent weighting.

				if (combat_closeness_ > 6) {
					weight += 100; // If the character insists on being in melee range then bump this up.
				}
			}
			weapon_category_weights[weapon_category] += weight;
		}
	}

	// Now we reverse the weights into a multimap so that we can allocate the costs starting with the highest weighted categories first. We use a multimap in case there are multiple categories with the same weight as this allows us to
	// randomise the order of those categories.
	std::multimap<int, const SkillCategoryData*> weighted_weapon_categories;
	for (const auto& [weapon_category, weight] : weapon_category_weights) {
		weighted_weapon_categories.emplace(weight, weapon_category);
	}
	weighted_weapon_categories.emplace(-10, &missile_artillery);
	logSkillCategoryWeights(weighted_weapon_categories);

	// Now we have all the data we can start to process and output to the debug log.
	// Pre-allocate a reasonable buffer to avoid multiple reallocations
	std::string buffer;
	buffer.reserve(1024);

	// Header
	buffer += std::format("\n| {:<8} | {:<18} |\n", "Cost", "Category");
	buffer += "|----------|--------------------|\n";

	for (const auto& [cost, count] : weapon_category_cost_count) {
		for (int i = 0; i < count; ++i) {
			if (weighted_weapon_categories.empty()) {
				LOG_ERROR("AutoCharacterBuilder: No more weapon categories to allocate for profession {}. This should never happen.", builder.profession_->name());
				return;
			}
			// Get the category with the highest weight and allocate the current cost to it.
			auto it = weighted_weapon_categories.end();
			--it; // We need to decrement the iterator as end() points to one past the last element.
			const SkillCategoryData* weapon_category = it->second;
			builder.category_development_costs_[weapon_category] = cost;
			fmt::format_to(std::back_inserter(buffer), "| {:<8} | {:<18} |\n", cost.toString(), weapon_category->name());
			weighted_weapon_categories.erase(it);
		}
	}
	LOG_DEBUG("Weapon Skill Category Costs:{}", buffer);
}

void AutoCharacterBuilder::setPreferredArmour(CharacterBuilder& builder) {
	// We randomise the traits if they have not already been initialised.
	ensureTraits(builder);

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
	if (std::ranges::contains(builder.magical_realms_, RealmType::kEssence) && !transcend_armour) {
		// Essence users cannot use any armour so limit to AT1 (Skin) and AT2 (Robes).
		std::erase_if(armour_weights, [](const auto& pair) {
			const ArmourTypeData* skill_category_data = pair.first;
			return skill_category_data->type() != ArmourType::kAT1 && skill_category_data->type() != ArmourType::kAT2;
		});
	} else if (std::ranges::contains(builder.magical_realms_, RealmType::kChanneling) && !transcend_armour) {
		// Channeling users struggle to cast spells in metal armour so AT13 and above are removed.
		std::erase_if(armour_weights, [](const auto& pair) {
			const ArmourTypeData* skill_category_data = pair.first;
			return skill_category_data->type() >= ArmourType::kAT13;
		});
	}

	// Reduce the weight of armours with a missile attack penalty if the character is not expected to be in close combat and is not using combat casting as they are more likely to want to avoid the missile attack penalty.
	if (combat_casting_ < 3 || combat_closeness_ < 4) {
		for (const auto& [armour_type_data, weight] : armour_weights) {
			if (armour_type_data->missileAttackPenalty() <= 0) {
				continue;
			}
			LOG_TRACE("AutoCharacterBuilder: Reducing weight of {} with missile attack penalty of {} for ranged character.", armour_type_data->name(), armour_type_data->missileAttackPenalty());
			armour_weights[armour_type_data] -= armour_type_data->missileAttackPenalty();
		}
	}

	// Now check for any preferred armours from the culture that are still available and if so then weigh the options towards those.
	for (const ArmourTypeData* preferred_armour : builder.culture_->cultureType().preferredArmour()) {
		// Only increase the weight if the preferred armour is still available after any previous filtering.
		if (armour_weights.contains(preferred_armour)) {
			LOG_TRACE("AutoCharacterBuilder: Increasing weight of preferred {} for character culture.", preferred_armour->name());
			armour_weights[preferred_armour] *= 10; // Heavily weight culture preferred armours.
		}
	}

	// If the character has high Quickness bonus weight armour in favour of those with a smaller penalty unless going full melee.
	if (traits_.combat_ < 7 && combat_closeness_ < 7) {
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
			LOG_DEBUG("AutoCharacterBuilder: Selected preferred armour type {} for character with random weight {} and current weight {}.", armour_type_data->name(), random_weight, current_weight);
			preferred_armour_ = armour_type_data;
			return;
		}
	}
}

void AutoCharacterBuilder::ensureTraits(CharacterBuilder& builder) {
	/* Core characteristics that drive preferred abilities */
	if (!traits_.combat_) {
		traits_.combat_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set combat to {}.", traits_.combat_);
	}
	if (!traits_.information_) {
		traits_.information_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set information to {}.", traits_.information_);
	}
	if (!traits_.stealth_) {
		traits_.stealth_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set stealth to {}.", traits_.stealth_);
	}
	if (!traits_.support_) {
		traits_.support_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set support to {}.", traits_.support_);
	}
	if (!traits_.utility_) {
		traits_.utility_ = archid::Dice(9).roll().result();
		LOG_TRACE("AutoCharacterBuilder: Set utility to {}.", traits_.utility_);
	}

	/* How combat is conducted */
	if (!combat_casting_) {
		// This is a function of the caster type and their spell lists.
		if (traits_.caster_ > 1) {
			// We take the how big a focus the character puts on casting, plus a random element to determine how likely they are to want to cast in combat. This means that characters with a higher castering trait are more likely to want to
			// cast in combat, but there is still a random element to allow for some variation and for some characters to be more focused on casting outside of combat.
			int value = traits_.caster_ * 2;
			value += Random::get(1, 9);
			combat_casting_ = value / 3; // The more a character uses spells the more likely they are to want to cast in combat
		} else {
			combat_casting_ = 1; // Non spell users are very unlikely to want to cast in combat.
		}
		LOG_TRACE("AutoCharacterBuilder: Set combat casting to {}.", combat_casting_);
	}
	if (!combat_closeness_) {
		if (builder.profession_->spellUserType() == SpellUserType::kPure || builder.profession_->spellUserType() == SpellUserType::kHybrid) {
			combat_closeness_ = Random::get(1, 3); // Pure and hybrid casters are unlikely to want to get close to combat.
		} else if (builder.profession_->spellUserType() == SpellUserType::kSemi) {
			combat_closeness_ = Random::get(3, 9); // Semi casters are more likely to want to be in medium range or close combat.
		} else {
			combat_closeness_ = Random::get(1, 9); // Non spell users can have a wide range of preferences for combat closeness so we give them a full range of random options.
		}
		LOG_TRACE("AutoCharacterBuilder: Set combat closeness to {}.", combat_closeness_);
	}

	/* Whether to spend DPs on a small set of skills or a wider range */
	if (!focussed_) {
		focussed_ = Random::get(1, 9);
		LOG_TRACE("AutoCharacterBuilder: Set focussed to {}.", focussed_);
	}

	/* Whether to keep closely aligned to the traits or not */
	if (!aligned_) {
		aligned_ = Random::get(1, 9);
		LOG_TRACE("AutoCharacterBuilder: Set aligned to {}.", aligned_);
	}
}

/* ------------------------------------------------------------------ */
/* Automate stat generation and allocation                            */
/* ------------------------------------------------------------------ */
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

/* ------------------------------------------------------------------ */
/* Automate hobby choices                                             */
/* ------------------------------------------------------------------ */
void AutoCharacterBuilder::autoHobbyChoices(CharacterBuilder& builder) {
	if (builder.built_) {
		throw std::runtime_error("CharacterBuilder: Cannot auto hobbies after character has been built.");
	}

	auto logger = rm::util::Logger::get();

	// This may be the first auto function called so we need to ensure the traits have been initialised
	ensureTraits(builder);

	/*
	 * Skill ranks - This is a blunt approach for now where we get the weight of each skill from the available choices and pick the highest ranked skill/category until we run out of ranks to allocate.
	 */

	// Sort both skills and categories with the highest weights at the top se we can use .begin() to pick the highest weighted option.
	std::multimap<int, const SubcategoriedSkillData*, std::greater<int>> skill_rank_weights;
	for (const SubcategoriedSkillData* skill : builder.culture_->hobbySkills()) {
		if (logger && logger->should_log(spdlog::level::trace)) {
			LOG_TRACE("AutoCharacterBuilder: Calculating weight for hobby skill {}.", skill->name());
		}
		int weight = traitComparisonWeighting(traits_, skill->skillData().traits());
		skill_rank_weights.emplace(weight, skill);
	}
	std::multimap<int, const SkillCategoryData*, std::greater<int>> category_rank_weights;
	for (const SkillCategoryData* category : builder.culture_->hobbySkillCategories()) {
		if (logger && logger->should_log(spdlog::level::trace)) {
			LOG_TRACE("AutoCharacterBuilder: Calculating weight for hobby skill category {}.", category->name());
		}
		int weight = traitComparisonWeighting(traits_, category->traits());
		category_rank_weights.emplace(weight, category);
	}

	// We are going to look at the skills first and then check if the corresponding category has any ranks and if not adding one to ensure there is no negative bonus and then add the skill bonus.
	int hobby_skill_ranks = builder.num_hobby_skill_ranks_;
	while (hobby_skill_ranks > 0) {
		if (skill_rank_weights.empty() && category_rank_weights.empty()) {
			LOG_WARN("AutoCharacterBuilder: No hobby skills or categories to allocate for profession {}. This should not happen as there should always be at least some culture preferences to allocate, but we break out of the loop "
			         "just in case.",
			         builder.profession_->name());
			break;
		}

		// Pick the top weighted
		int top_skill = skill_rank_weights.empty() ? -1 : skill_rank_weights.begin()->first;
		int top_category = category_rank_weights.empty() ? -1 : category_rank_weights.begin()->first;

		if (top_category > top_skill) {
			const SkillCategoryData* category = category_rank_weights.begin()->second;
			category_rank_weights.erase(category_rank_weights.begin());
			builder.hobby_category_ranks_[category]++;
			hobby_skill_ranks--;
			// We add the category back with half weight to allow for multiple ranks in the same category to be allocated if there are still ranks left to allocate and the category is still the highest weighted option.
			if (logger && logger->should_log(spdlog::level::debug)) {
				LOG_DEBUG("AutoCharacterBuilder: Allocated hobby category rank to {}. Remaining hobby skill ranks: {}.", category->name(), hobby_skill_ranks);
			}
			category_rank_weights.emplace(top_category / 2, category);
		} else {
			const SubcategoriedSkillData* skill = skill_rank_weights.begin()->second;
			skill_rank_weights.erase(skill_rank_weights.begin());
			builder.hobby_skill_ranks_[skill]++;
			hobby_skill_ranks--;
			// We add the skill back with half weight to allow for multiple ranks in the same category to be allocated if there are still ranks left to allocate and the skill is still the highest weighted option.
			if (logger && logger->should_log(spdlog::level::debug)) {
				LOG_DEBUG("AutoCharacterBuilder: Allocated hobby skill rank to {}. Remaining hobby skill ranks: {}.", skill->name(), hobby_skill_ranks);
			}
			skill_rank_weights.emplace(top_skill / 2, skill);
		}
	}

	/*
	 * Language ranks - We randomize these selections as this would be game world specific and we have no data on that.
	 */
	std::vector<std::string> languages;
	for (const auto& [language, ranks] : builder.race_->adolescentLanguageAbilities()) {
		languages.push_back(language);
	}

	int num_language_ranks = builder.num_adolescent_language_ranks_;
	while (num_language_ranks > 0) {
		std::ranges::shuffle(languages, Random::mt);
		// Select a language at random to add ranks to
		std::string language_name = languages.back();

		// Get the max ranks we can have in the language
		LanguageRanks max_language_ranks = builder.race_->adolescentLanguageAbilities().at(language_name);

		// Initialize our chosen language ranks with the correct language, which will set all the ranks to 0
		LanguageRanks current_ranks{max_language_ranks.language()};

		// Check if we can add any more ranks to this language
		for (const LanguageRanks& existing_ranks : builder.language_abilities_) {
			if (existing_ranks.languageId() == max_language_ranks.languageId()) {
				current_ranks = existing_ranks;
				break;
			}
		}

		// Now check if we have added any adolescent ranks
		for (const LanguageRanks& adolescent_ranks : builder.adolescent_language_choices_) {
			if (adolescent_ranks.languageId() == max_language_ranks.languageId()) {
				if (adolescent_ranks.spokenRanks() > current_ranks.spokenRanks() || adolescent_ranks.writtenRanks() > current_ranks.writtenRanks()) {
					current_ranks = adolescent_ranks;
					break;
				}
			}
		}

		// Add the rank to the character if we have not already reached the max ranks for the language. We check both spoken and written ranks as they can be different and we want to ensure we don't exceed the max for either.
		if (current_ranks.spokenRanks() < max_language_ranks.spokenRanks()) {
			if (logger && logger->should_log(spdlog::level::debug)) {
				LOG_DEBUG("AutoCharacterBuilder: Allocated spoken language rank to {}. Remaining language ranks: {}.", current_ranks.language().name(), num_language_ranks - 1);
			}
			current_ranks.updateSpokenRanks(1);
			builder.adolescent_language_choices_.emplace(current_ranks);
			num_language_ranks--;
		} else if (current_ranks.writtenRanks() < max_language_ranks.writtenRanks()) {
			if (logger && logger->should_log(spdlog::level::debug)) {
				LOG_DEBUG("AutoCharacterBuilder: Allocated written language rank to {}. Remaining language ranks: {}.", current_ranks.language().name(), num_language_ranks - 1);
			}
			current_ranks.updateWrittenRanks(1);
			builder.adolescent_language_choices_.emplace(current_ranks);
			num_language_ranks--;
		}
	}

	/*
	 * Spell lists - We pick a random list from the own realm open lists
	 */
	std::vector<const SpellListData*> list_options = getSpellLists(SpellListType::kOpen, builder.magical_realms_, *builder.object_factory_);
	if (!list_options.empty()) {
		std::ranges::shuffle(list_options, Random::mt);
		const SpellListData* list = list_options.front();
		if (logger && logger->should_log(spdlog::level::debug)) {
			LOG_DEBUG("AutoCharacterBuilder: Added adolescent spell list {}.", list->name());
		}
		builder.adolescent_spell_list_choice_ = list;
	}
}

/* ------------------------------------------------------------------ */
/* Helper functions                                                   */
/* ------------------------------------------------------------------ */
std::vector<const SubcategoriedSkillData*> AutoCharacterBuilder::getSubcategoriesForSkill(CharacterBuilder& builder, const SkillData& skill) {
	// We currently only handle the riding skill, but this could be expanded later if needed.
	std::vector<const SubcategoriedSkillData*> subcategories;

	if (skill.id() == "SKILL_RIDING") {
		subcategories.append_range(getCultureMountSkills(*builder.culture_type_, *builder.object_factory_));
		subcategories.append_range(getRaceMountSkills(*builder.race_, *builder.object_factory_));
	} else {
		SubcategoriedSkillData& subcategoried_skill_data = builder.object_factory_->subcategoriedSkillData(skill);
		subcategories.push_back(&subcategoried_skill_data);
	}

	return subcategories;
}

/* ------------------------------------------------------------------ */
/* Free functions                                                     */
/* ------------------------------------------------------------------ */
std::vector<const SkillData*> getCategorySkills(const SkillCategoryData& category, rm::PersistentObjectManager& object_manager) {
	std::vector<const SkillData*> category_skills;
	for (const SkillData& skill : object_manager.getAll<SkillData>()) {
		if (&skill.category() == &category) {
			category_skills.emplace_back(&skill);
		}
	}
	return category_skills;
}

std::vector<const SkillData*> getGroupSkills(const SkillGroupData& group, rm::PersistentObjectManager& object_manager) {
	std::vector<const SkillData*> group_skills;
	for (const SkillData& skill : object_manager.getAll<SkillData>()) {
		if (&skill.category().group() == &group) {
			group_skills.emplace_back(&skill);
		}
	}
	return group_skills;
}

std::vector<const SkillCategoryData*> getGroupCategories(const SkillGroupData& group, rm::PersistentObjectManager& object_manager) {
	std::vector<const SkillCategoryData*> group_categories;
	for (const SkillCategoryData& category : object_manager.getAll<SkillCategoryData>()) {
		if (&category.group() == &group) {
			group_categories.emplace_back(&category);
		}
	}
	return group_categories;
}

std::vector<const WeaponTypeData*> getSkillWeapons(const SkillData& skill, rm::PersistentObjectManager& object_manager) {
	std::vector<const WeaponTypeData*> skill_weapons;
	for (const WeaponTypeData& weapon : object_manager.getAll<WeaponTypeData>()) {
		if (std::ranges::contains(weapon.skills(), &skill)) {
			skill_weapons.emplace_back(&weapon);
		}
	}
	return skill_weapons;
}

std::vector<const SubcategoriedSkillData*> getCultureMountSkills(const CultureTypeData& culture, rm::PersistentObjectManager& object_manager) {
	std::vector<const SubcategoriedSkillData*> culture_mounts{};

	// We are looking for the typical mounts that a culture uses and then apply these as subcategories of the riding skill, so we need that first.
	std::string riding_id = "SKILL_RIDING";
	SkillData& riding_skill = object_manager.get<SkillData>(riding_id);

	std::vector<std::string> mounts{};
	// The typical mounts are not defined in the culture data so we hard code some values based on the culture name for now.
	// TODO Add culture mount preferences to the culture data to make this more data driven.
	if (culture.id() == "CULTURETYPE_AQUATIC") {
		mounts.push_back("Shark");
		mounts.push_back("Dolphin");
	} else if (culture.id() == "CULTURETYPE_DESERT") {
		mounts.push_back("Camel");
		mounts.push_back("Horse");
	} else {
		mounts.push_back("Horse");
	}

	for (const std::string& mount : mounts) {
		SubcategoriedSkillData& subcategoried_skill_data = object_manager.subcategoriedSkillData(riding_skill, mount);
		culture_mounts.push_back(&subcategoried_skill_data);
	}

	return culture_mounts;
}

std::vector<const SubcategoriedSkillData*> getRaceMountSkills(const RaceData& race, rm::PersistentObjectManager& object_manager) {
	std::vector<const SubcategoriedSkillData*> race_mounts{};

	// We are looking for the typical mounts that a race uses and then apply these as subcategories of the riding skill, so we need that first.
	std::string riding_id = "SKILL_RIDING";
	SkillData& riding_skill = object_manager.get<SkillData>(riding_id);

	std::vector<std::string> mounts{};
	// The typical mounts are not defined in the race data so we hard code some values based on the race name for now.
	// TODO Add racial mount preferences to the race data to make this more data driven.

	if (race.id() == "RACE_DWARVES") {
		mounts.push_back("Wolf");
		mounts.push_back("Bear");
	} else if (race.id() == "RACE_GOBLINS") {
		mounts.push_back("Boar");
	} else if (race.name().starts_with("Halflings")) {
		mounts.push_back("Pony");
	} else if (race.id() == "RACE_HOBGOBLINS") {
		mounts.push_back("Boar");
		mounts.push_back("Breliss");
	} else if (race.name().starts_with("Orcs")) {
		mounts.push_back("Bat");
		mounts.push_back("Wolf");
	}

	for (const std::string& mount : mounts) {
		SubcategoriedSkillData& subcategoried_skill_data = object_manager.subcategoriedSkillData(riding_skill, mount);
		race_mounts.push_back(&subcategoried_skill_data);
	}

	return race_mounts;
}

std::vector<const SpellListData*> getSpellLists(SpellListType::Type type, const std::set<RealmType ::Type>& realms, rm::PersistentObjectManager& object_manager) {
	std::vector<const SpellListData*> spell_lists;

	for (const SpellListData& spell_list : object_manager.getAll<SpellListData>()) {
		if (spell_list.type() == type && std::ranges::equal(realms, spell_list.realms())) {
			spell_lists.push_back(&spell_list);
		}
	}

	return spell_lists;
}

int traitComparisonWeighting(const CharacterTraits& creature, const CharacterTraits& other) {
	auto logger = rm::util::Logger::get();

	// First we get the alignment which is a measure of how close the traits of the creature and the other are. We start with a base value of 48 which is the minimum alignment possible as the trait values are between 1 and 9 and there are 6
	// traits so the maximum difference is 8 for each trait which gives a total of 48. We then subtract the absolute difference between each trait value for the creature and the other from this base value to get the final alignment score.
	//
	// This gives us a theoretical range of between 0 and 48, but in reality we are much more likely to see values closer to 30 to 40 given that few trait values will have a value of greater than 7 and certainly not for all traits.
	int alignment{48};
	alignment -= std::abs(creature.combat_ - other.combat_);
	alignment -= std::abs(creature.information_ - other.information_);
	alignment -= std::abs(creature.stealth_ - other.stealth_);
	alignment -= std::abs(creature.support_ - other.support_);
	alignment -= std::abs(creature.utility_ - other.utility_);
	alignment -= std::abs(creature.caster_ - other.caster_);

	// Next we look at the priorities of the creature and see if the ability meets these, we are looking for a high score here so we take the minimum of the trait values for each trait as this will give us a high score if both the creature
	// and the other have a high value for that trait, but a low score if either of them has a low value for that trait. This means that we are looking for a good match on the traits that are important to the creature.
	int priority_score{0};
	priority_score += std::min(creature.combat_, other.combat_);
	priority_score += std::min(creature.information_, other.information_);
	priority_score += std::min(creature.stealth_, other.stealth_);
	priority_score += std::min(creature.support_, other.support_);
	priority_score += std::min(creature.utility_, other.utility_);
	priority_score += std::min(creature.caster_, other.caster_);

	int weight{0};
	// We use the creature's trait as the baseline
	weight = alignment + (priority_score * 3);

	// Gate the logic to TRACE
	if (logger && logger->should_log(spdlog::level::trace)) {
		LOG_TRACE("Trait Comparison: alignment={}, priority_score={}, weight={}", alignment, priority_score, weight);
	}

	return weight;
}

void logSkillCategoryCosts(std::map<const SkillDevelopmentCost, int>& category_costs) {
	auto logger = rm::util::Logger::get();

	// Gate the logic to TRACE
	if (logger && logger->should_log(spdlog::level::trace)) {
		// Pre-allocate a reasonable buffer to avoid multiple reallocations
		std::string buffer;
		buffer.reserve(1024);

		// Header
		buffer += std::format("\n| {:<18} | {:<8} |\n", "Cost", "Count");
		buffer += std::format("| {:-<18} | {:-<8} |\n", "", "");

		for (const auto& [cost, val] : category_costs) {
			// Append formatted row
			fmt::format_to(std::back_inserter(buffer), "| {:<18} | {:<8} |\n", cost.toString(), val);
		}

		LOG_TRACE("Skill Category Cost Counts:{}", buffer);
	}
}

void logSkillCategoryWeights(std::multimap<int, const SkillCategoryData*>& category_weights) {
	auto logger = rm::util::Logger::get();

	// Gate the logic to TRACE
	if (logger && logger->should_log(spdlog::level::trace)) {
		// Pre-allocate a reasonable buffer to avoid multiple reallocations
		std::string buffer;
		buffer.reserve(1024);

		// Header
		buffer += std::format("\n| {:<18} | {:<8} |\n", "Category", "Weight");
		buffer += std::format("| {:-<18} | {:-<8} |\n", "", "");

		for (const auto& [val, ptr] : category_weights) {
			// Safety check for the pointer
			std::string name = ptr ? ptr->name() : "nullptr";

			// Append formatted row
			fmt::format_to(std::back_inserter(buffer), "| {:<18} | {:<8} |\n", name, val);
		}

		LOG_TRACE("Skill Category Weights:{}", buffer);
	}
}

} // namespace rm::game::character