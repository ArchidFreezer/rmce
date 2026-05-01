#include <algorithm>
#include <cmath>
#include <ranges>
#include <CharacterBuilder.h>
#include <CharacterCategory.h>
#include <EnumIterator.h>
#include <Logger.h>
#include <TrainingPackageCostTable.h>
#include <SkillDevelopmentCost.h>

namespace rm::game::character {

/*
 * @brief Build and return a Character object based on the attributes and stats set in the builder.
 *
 * This method performs sanity checks to ensure that the builder has been properly set up before building the character.
 */
Character& CharacterBuilder::build() {
	// Do some sanity checking first to make sure that the builder has been properly set up before we start building the character.
	if (race_ == nullptr) {
		throw std::runtime_error("CharacterBuilder: Race must be set before building a character.");
	}

	Character& character = object_factory_->get<Character>();
	/* Basic Data */
	character.setName(name_);
	character.male_ = male_;
	character.player_character_ = pc_;
	character.level_ = 0;
	character.experience_points_ = 0; // Start with the minimum for level 0
	character.race_ = race_;
	character.culture_ = culture_;
	character.profession_ = profession_;
	character.stats_ = stats_;
	character.gold_ = total_gold_;
	character.items_ = std::move(total_items_);

	/* Physical characteristics */
	character.height_ = height_;
	character.weight_ = weight_;
	character.build_description_ = build_description_;
	character.lifespan_ = lifespan_;

	/* Derived data */
	character.development_points_ = development_points_;
	character.power_realms_ = magical_realms_;
	character.spell_list_categories_ = spell_list_categories_;

	/* Learned abilities */

	/* Apply category data */
	// The profession should define a cost for every category so this loop should create all the Category objects in the character.
	// We need to store some specific categories for late on so get them as we create them.
	Category* communication_category{nullptr};
	for (const auto& [category_data, dev_cost] : category_development_costs_) {
		auto [it, inserted] = character.categories_.try_emplace(category_data); // Create a new category if it doesn't exist otherwise get the existing category to update it.
		Category& category = it->second;
		if (!inserted) {
			throw std::runtime_error("CharacterBuilder: Duplicate category data found when building character. This should not happen as the builder should prevent this from happening when adding category data.");
		}
		category.category_data_ = category_data;
		category.development_cost_ = dev_cost;
		category.progression_type_ = &category_data->skillCategoryProgression();

		// Set the stats used for the category if they are realm stats, otherwise let the character get them directly from the category data.
		if (category_data->useRealmStats()) {
			for (const RealmType::Type& realm : magical_realms_) {
				for (const StatType::Type& stat : StatType::statsForRealm(realm)) {
					category.stats_.push_back(stat);
				}
			}
		}

		// Check for any group bonuses
		const SkillGroupData& group = category_data->group();
		auto group_prof_bonus_it = group_professional_bonuses_.find(&group);
		if (group_prof_bonus_it != group_professional_bonuses_.end()) {
			category.profession_bonus_ = std::max(category.profession_bonus_, group_prof_bonus_it->second);
		}

		auto group_special_bonus_it = group_special_bonuses_.find(&group);
		if (group_special_bonus_it != group_special_bonuses_.end()) {
			category.special_bonus_ = std::max(category.special_bonus_, group_special_bonus_it->second);
		}

		// Store off the specific categories we need for later so we can easily access them when we need to apply the appropriate bonuses to the skills.
		if (category_data->id() == "SKILLCATEGORY_COMMUNICATION") {
			communication_category = &category;
		}
	}

	for (const auto& [category_type, ranks] : category_ranks_) {
		auto [it, inserted] = character.categories_.try_emplace(category_type); // Create a new category if it doesn't exist otherwise get the existing category to update it.
		Category& category = it->second;
		category.ranks_ = ranks;
	}

	for (const auto& [category_type, prof_bonus] : category_professional_bonuses_) {
		auto [it, inserted] = character.categories_.try_emplace(category_type); // Create a new category if it doesn't exist otherwise get the existing category to update it.
		Category& category = it->second;
		category.profession_bonus_ = std::max(category.profession_bonus_, prof_bonus);
	}

	for (const auto& [category_type, special_bonus] : category_special_bonuses_) {
		auto [it, inserted] = character.categories_.try_emplace(category_type); // Create a new category if it doesn't exist otherwise get the existing category to update it.
		Category& category = it->second;
		category.special_bonus_ = std::max(category.special_bonus_, special_bonus);
	}

	/* Languages */
	for (const LanguageRanks& language_ability : language_abilities_) {
		Language language{};
		language.setLanguage(language_ability.language());
		language_ability.spokenRanks() > 0 ? language.updateSpokenRanks(language_ability.spokenRanks()) : void();    // Only update the ranks if they are greater than 0
		language_ability.writtenRanks() > 0 ? language.updateWrittenRanks(language_ability.writtenRanks()) : void(); // Only update the ranks if they are greater than 0
		language_ability.somaticRanks() > 0 ? language.updateSomaticRanks(language_ability.somaticRanks()) : void(); // Only update the ranks if they are greater than 0
		language.category_ = communication_category;                                                                 // All languages are in the communication category so we can just set this directly.
		if (communication_category != nullptr) {
			language.progression_type_ = &communication_category->category_data_->defaultSkillProgression(); // All languages use the default progression for the communication category so we can just set this directly.
		}
		character.setLanguageAbility(language);
	}

	/* Spell Lists */
	for (const auto& [spell_list_data, ranks] : spell_list_ranks_) {
		SpellList spell_list{};
		spell_list.spell_list_ = spell_list_data;
		const SkillCategoryData* category_data = spellListCategory(spell_list_categories_, *spell_list_data);
		if (category_data != nullptr) {
			spell_list.category_ = &character.categories_.at(category_data);
			spell_list.progression_type_ = &category_data->defaultSkillProgression();
		}
		spell_list.ranks_ = ranks;
		character.spell_lists_.emplace(spell_list_data, spell_list);
	}

	for (const auto& [spell_list_data, special_bonus] : spell_list_special_bonuses_) {
		auto& spell_list = character.spell_lists_.at(spell_list_data);
		spell_list.special_bonus_ = special_bonus;
	}

	/* Apply skill data */
	for (const auto& [skill_data, ranks] : skill_ranks_) {
		auto [it, inserted] = character.skills_.try_emplace(skill_data); // Create a new skill if it doesn't exist otherwise get the existing skill to update it.
		Skill& skill = it->second;
		skill.skill_data_ = skill_data;
		skill.ranks_ = ranks;
	}

	for (const auto& [skill_data, prof_bonus] : skill_professional_bonuses_) {
		auto [it, inserted] = character.skills_.try_emplace(skill_data); // Create a new skill if it doesn't exist otherwise get the existing skill to update it.
		Skill& skill = it->second;
		skill.profession_bonus_ = prof_bonus;
	}

	for (const auto& [skill_data, special_bonus] : skill_special_bonuses_) {
		auto [it, inserted] = character.skills_.try_emplace(skill_data); // Create a new skill if it doesn't exist otherwise get the existing skill to update it.
		Skill& skill = it->second;
		skill.special_bonus_ = special_bonus;
	}

	for (const auto& [skill_data, dev_type] : skill_development_types_) {
		auto [it, inserted] = character.skills_.try_emplace(skill_data); // Create a new skill if it doesn't exist otherwise get the existing skill to update it.
		Skill& skill = it->second;
		skill.development_type_ = dev_type;
	}

	// Now we have all the modified skills we can iterate through them and add the appropriate category.
	for (auto& [skill_data, skill] : character.skills_) {
		const SkillCategoryData& category_data = skill_data->skillData().category();
		auto category_it = character.categories_.find(&category_data);
		if (category_it != character.categories_.end()) {
			skill.category_ = &category_it->second;
			skill.progression_type_ = &category_data.defaultSkillProgression();
		}
	}

	{
		SubcategoriedSkillData& body_development_skill_data = object_factory_->subcategoriedSkillData(Character::BD_SKILL_ID);
		character.body_devlopment_skill_ = &body_development_skill_data;
		auto [it, inserted] = character.skills_.try_emplace(&body_development_skill_data); // Create a new skill if it doesn't exist otherwise get the existing skill to update it.
		Skill& body_development_skill = it->second;
		body_development_skill.skill_data_ = &body_development_skill_data;
		body_development_skill.progression_type_ = &race_->armsProgression();
		character.updateMaxHits();
		character.hits_ = character.max_hits_; // Set the current hits for the character to the max based on the body development skill.
	}

	{
		SubcategoriedSkillData& power_point_skill_data = object_factory_->subcategoriedSkillData(Character::PP_SKILL_ID);
		character.power_point_skill_ = &power_point_skill_data;
		auto [it, inserted] = character.skills_.try_emplace(&power_point_skill_data); // Create a new skill if it doesn't exist otherwise get the existing skill to update it.
		Skill& power_point_skill = it->second;
		power_point_skill.skill_data_ = &power_point_skill_data;
		power_point_skill.progression_type_ = getPpProgression();
		character.updateMaxPowerPoints();
		character.power_points_ = character.max_power_points_; // Set the current power points for the character to the max.
	}

	// Finally we need to update the derived data for the character now that we have all the base data set up. This will calculate all the derived data such as the stat bonuses, max HP & PP, etc.
	character.updateAllDerivedData();

	built_ = true;
	return character;
}

void CharacterBuilder::reset(bool aggregate_state_only, bool clear_auto_generated) {
	if (clear_auto_generated) {
		initial_stats_.clear();
		height_ = 0;
		weight_ = 0;
		lifespan_ = 0;
		build_description_.clear();
	}
	if (!aggregate_state_only) {
		built_ = false;
		name_.clear();
		race_ = nullptr;
		culture_ = nullptr;
		culture_type_ = nullptr;
		profession_ = nullptr;
		male_ = true;
		auto_build_modifier_ = true;
		entered_build_modifier_ = 0;
		auto_height_ = true;
		entered_height_ = 0;
		magical_realms_.clear();
		num_hobby_skill_ranks_ = 0;
		num_adolescent_language_ranks_ = 0;
		num_adolescent_spell_list_ranks_ = 0;
		development_points_ = 0;
		race_category_everyman_choices_.clear();
		culture_type_category_skill_ranks_.clear();
		prof_skill_development_type_choices_.clear();
		prof_category_development_type_choices_.clear();
		prof_group_development_type_choices_.clear();
		prof_base_spell_list_choices_.clear();
		// Hobby/Adolescent choices
		hobby_skill_ranks_.clear();
		hobby_category_ranks_.clear();
		adolescent_spell_list_choice_ = nullptr;
		adolescent_language_choices_.clear();
		// Background choices
		background_stats_.clear();
		background_extra_gold_ = 0;
		background_language_choices_.clear();
		background_skill_special_bonuses_.clear();
		background_category_special_bonuses_.clear();
		background_items_.clear();
	}
	// Clear the aggregated state regardless of whether we are resetting the entire builder or not, as the aggregated state needs to be recalculated based on the current choices after any reset.
	total_gold_ = 2; // This is the starting gold every character gets
	language_abilities_.clear();
	realm_progressions_.clear();
	stats_.clear();
	skill_ranks_.clear();
	skill_professional_bonuses_.clear();
	skill_special_bonuses_.clear();
	skill_development_types_.clear();
	category_ranks_.clear();
	category_professional_bonuses_.clear();
	category_special_bonuses_.clear();
	category_development_costs_.clear();
	group_professional_bonuses_.clear();
	group_special_bonuses_.clear();
	spell_list_ranks_.clear();
	total_items_.clear();
}

void CharacterBuilder::hardReset() {
	reset(false, true);
}

void CharacterBuilder::recalculateAggregatedState() {
	reset(true);             // We reset the builder to clear the aggregated state and then reapply the choices to recalculate the aggregated state based on the current choices. This is a simple way to ensure consistency.
	stats_ = initial_stats_; // We start with the initial stats as the base for the aggregated state and then apply the effects of the choices on top of this.
	calculateDevelopmentPoints(initial_stats_);
	applyRace();
	applyRaceChoices();
	applyCultureType();
	applyCultureTypeChoices();
	applyCulture();
	applyCultureChoices();
	applyProfession();
	applyProfessionChoices();
	applyPrimaryDependents();
	applyHobbyChoices();
	applyBackgroundChoices();
}

/* Primary choices */
void CharacterBuilder::applyRace() {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply race after character has been built.");
	}

	// Stat bonuses
	for (const auto& [stat_type, bonus] : race_->statBonuses()) {
		Stat& stat = stats_[stat_type]; // This will default construct a new Stat object if the stat has not been touched yet.
		stat.setRacialBonus(bonus);
	}

	// language abilities, both fixed and adolescentchoices
	for (const auto& [language_name, language_ability] : race_->startingLanguageAbilities()) {
		applyLanguageAbility(language_ability);
	}

	// We are going to pre-populate the skill development type map with all skills for convenience to make like easier later on when we need to check the development type of a skill. We will set the development type to standard for all
	// skills to start with and then apply the everyman and restricted skills and categories on top of this, which will override the development type for those skills and categories as necessary.
	for (const auto& skill_wrapper : object_factory_->getAll<SkillData>()) {
		std::string skill_id = skill_wrapper.get().id();
		const SubcategoriedSkillData& skill = object_factory_->subcategoriedSkillData(skill_id);
		skill_development_types_.insert_or_assign(&skill, SkillDevelopmentType::Type::kStandard);
	}

	// Everyman and restricted skills - we need to insert these into a set to ensure that there are no duplicates and that they are easily accessible for checking if a skill is an everyman skill or not when applying the effects of the
	// character's choices later on in the building process
	for (const SubcategoriedSkillData* skill : race_->everymanSkills()) {
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, SkillDevelopmentType::Type::kEveryman));
	}

	for (const SubcategoriedSkillData* skill : race_->restrictedSkills()) {
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, SkillDevelopmentType::Type::kRestricted));
	}

	for (const SkillCategoryData* category : race_->everymanSkillCategories()) {
		// We have pre-populated skill_development_types_ with all base skills so we know they will exist
		for (const auto& [skill, current_type] : skill_development_types_) {
			if (&skill->skillData().category() == category) {
				skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, SkillDevelopmentType::Type::kEveryman));
			}
		}
	}

	for (const SkillCategoryData* category : race_->restrictedSkillCategories()) {
		// We have pre-populated skill_development_types_ with all base skills so we know they will exist
		for (const auto& [skill, current_type] : skill_development_types_) {
			if (&skill->skillData().category() == category) {
				skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, SkillDevelopmentType::Type::kRestricted));
			}
		}
	}

	// Realm progressions
	realm_progressions_.clear(); // Clear any existing realm progressions to ensure that if the race was previously set, the character does not end up with the progressions from both races
	realm_progressions_.insert_or_assign(RealmType::kArms, &race_->armsProgression());
	realm_progressions_.insert_or_assign(RealmType::kArcane, &race_->arcaneProgression());
	realm_progressions_.insert_or_assign(RealmType::kChanneling, &race_->channelingProgression());
	realm_progressions_.insert_or_assign(RealmType::kEssence, &race_->essenceProgression());
	realm_progressions_.insert_or_assign(RealmType::kMentalism, &race_->mentalismProgression());
}

void CharacterBuilder::applyRaceChoices() {
	for (const auto& skill : race_category_everyman_choices_) {
		// This may be a skill with a subcategory defined so we can't assume it exists
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, SkillDevelopmentType::Type::kEveryman));
	}
}

void CharacterBuilder::applyCultureType() {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply culture type after character has been built.");
	}
	num_hobby_skill_ranks_ = culture_type_->hobbySkillRanks();
	num_adolescent_language_ranks_ = culture_type_->adolescentLanguageRanks();
	num_adolescent_spell_list_ranks_ = culture_type_->spellListRanks();
	skill_ranks_ = culture_type_->skillRanks();
	category_ranks_ = culture_type_->skillCategoryRanks();
}

void CharacterBuilder::applyCultureTypeChoices() {
	for (const auto& [skill, ranks] : culture_type_category_skill_ranks_) {
		int current_ranks = skill_ranks_.find(skill) != skill_ranks_.end() ? skill_ranks_.at(skill) : 0;
		skill_ranks_.insert_or_assign(skill, std::max(current_ranks, ranks));
	}
}

void CharacterBuilder::applyCulture() {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply culture after character has been built.");
	}

	// The culture defines also require the profession to be set so are calculated in the applyPrimaryDependents function
}

void CharacterBuilder::applyCultureChoices() {
	// There are no direct choices for cultures only hooby and background options
}

void CharacterBuilder::applyProfession() {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply profession after character has been built.");
	}

	for (const auto& [skill, bonus] : profession_->skillBonuses()) {
		int current_bonus = skill_professional_bonuses_.find(skill) != skill_professional_bonuses_.end() ? skill_professional_bonuses_.at(skill) : 0;
		skill_professional_bonuses_.insert_or_assign(skill, std::max(current_bonus, bonus));
	}

	for (const auto& [category, bonus] : profession_->skillCategoryProfessionBonuses()) {
		int current_ranks = category_professional_bonuses_.find(category) != category_professional_bonuses_.end() ? category_professional_bonuses_.at(category) : 0;
		category_professional_bonuses_.insert_or_assign(category, std::max(current_ranks, bonus));
	}

	for (const auto& [category, bonus] : profession_->skillCategorySpecialBonuses()) {
		int current_ranks = category_special_bonuses_.find(category) != category_special_bonuses_.end() ? category_special_bonuses_.at(category) : 0;
		category_special_bonuses_.insert_or_assign(category, current_ranks + bonus);
	}

	for (const auto& [group, bonus] : profession_->skillGroupProfessionBonuses()) {
		int current_ranks = group_professional_bonuses_.find(group) != group_professional_bonuses_.end() ? group_professional_bonuses_.at(group) : 0;
		group_professional_bonuses_.insert_or_assign(group, std::max(current_ranks, bonus));
	}

	for (const auto& [group, bonus] : profession_->skillGroupSpecialBonuses()) {
		int current_ranks = group_special_bonuses_.find(group) != group_special_bonuses_.end() ? group_special_bonuses_.at(group) : 0;
		group_special_bonuses_.insert_or_assign(group, current_ranks + bonus);
	}

	for (const auto& [skill, development_type] : profession_->skillDevelopmentTypes()) {
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
	}

	// Set the development type for all skills in a skill category
	for (const auto& [category, development_type] : profession_->skillCategorySkillDevelopmentTypes()) {
		// We have pre-populated skill_development_types_ with all base skills so we know they will exist
		for (const auto& [skill, current_type] : skill_development_types_) {
			if (&skill->skillData().category() == category) {
				skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
			}
		}
	}

	// Set the development type for all skills in a skill group
	for (const auto& [group, development_type] : profession_->skillGroupSkillDevelopmentTypes()) {
		// We have pre-populated skill_development_types_ with all base skills so we know they will exist
		for (const auto& [skill, current_type] : skill_development_types_) {
			if (&skill->skillData().category().group() == group) {
				skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
			}
		}
	}

	// Category costs
	for (const auto& category : profession_->skillCategoriesWithCost()) {
		category_development_costs_.insert_or_assign(category, profession_->skillCategoryDevelopmentCost(*category));
	}
}

void CharacterBuilder::applyProfessionChoices() {
	// Apply the chosen weapon category costs
	for (const auto& [category, cost] : weapon_development_cost_choices_) {
		category_development_costs_.insert_or_assign(category, cost);
	}

	// We know the character knows these lists, but set the ranks to 0 as no ranks have been allocated yet.
	for (const auto& spellList : prof_base_spell_list_choices_) {
		spell_list_ranks_.insert_or_assign(spellList, 0);
	}

	// Set of skills (base or subcategories) with their development type changed
	for (const auto& [skill, development_type] : prof_skill_development_type_choices_) {
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
	}

	// Set of skills from a category with their development type changed
	for (const auto& [skill, development_type] : prof_category_development_type_choices_) {
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
	}

	// Set of skills from a group with their development type changed
	for (const auto& [skill, development_type] : prof_group_development_type_choices_) {
		SkillDevelopmentType::Type current_type = skill_development_types_.find(skill) != skill_development_types_.end() ? skill_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skill_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
	}

	// Place the spell lists into their respective skill categories so we can determine the DP cost for increasing raks during the apprenticeship phase. This is expensive so we only do it when the drivers such as the realms or base spell
	// lists change and then we cache the results for quick access later on when we need to check the category of a spell list.
	if (set_spell_list_categories_) {
		setSpellListCategories();
		set_spell_list_categories_ = false;
	}
}

void CharacterBuilder::applyPrimaryDependents() {
	// Hobby skill/categories require the culture for the list of available choices and the profession for themax ranks for each choice
	for (const SubcategoriedSkillData* skill : culture_->hobbySkills()) {
		int max_ranks = getMaxHobbyRanksForSkill(skill);
		hobby_skill_rank_choices_.emplace(skill, max_ranks);
	}

	for (const SkillCategoryData* category : culture_->hobbySkillCategories()) {
		int max_ranks = getMaxHobbyRanksForCategory(category);
		hobby_category_rank_choices_.emplace(category, max_ranks);
	}
}

/* Physique */
void CharacterBuilder::generatePhysique() {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot generate physique after character has been built.");
	}

	// We use this value for both height and weight to ensure they are consistent with each other as they are both based on the same underlying distribution of body sizes for the race
	float height_mean{(male_ ? race_->averageMaleHeight() : race_->averageFemaleHeight()) * 1.0f};

	// Get the height - if the auto height option is selected, we generate a height based on the average height for the race and sex
	int height{entered_height_};
	if (auto_height_) {
		float height_std_deviation{height_mean * 0.035f};
		std::normal_distribution height_distribution{height_mean, height_std_deviation};
		height = static_cast<int>(height_distribution(Random::mt));
	}

	height_ = height;

	// Get the individuals build/frame
	int char_build_modifier{entered_build_modifier_};
	if (auto_build_modifier_) {
		// Lets add some variance into this
		std::normal_distribution build_distribution{1.0f, 2.2f};
		char_build_modifier = static_cast<int>(build_distribution(Random::mt));
	}

	std::string build_label{};
	if (char_build_modifier <= -10)
		build_label = "Skeletal";
	else if (char_build_modifier <= -7)
		build_label = "Wasted";
	else if (char_build_modifier <= -4)
		build_label = "Thin";
	else if (char_build_modifier <= -2)
		build_label = "Slender";
	else if (char_build_modifier <= 2)
		build_label = "Normal";
	else if (char_build_modifier <= 4)
		build_label = "Stocky";
	else if (char_build_modifier <= 7)
		build_label = "Large";
	else if (char_build_modifier <= 10)
		build_label = "Obese";
	else
		build_label = "Blubbery";

	build_description_ = build_label + " build";

	// The weight is calculated using the following formula
	// Weight = ( k � H^3 ) � FM � BM � GM
	// H: Height in inches.
	// k: Racial Density Constant.
	// FM: Frame Modifier (The race's average skeletal "width").
	// GM: Gender Modifier (1.0 for Male, 0.9 for Female).
	// BM: Build Modifier (Individual character's physique).
	//
	// k = Race.buildModifier() / 100000 ; This is so we can use integers in the JSON file for the build modifier which is more intuitive to work with and then convert it to a small decimal value for the calculation
	//     Human = 46 / 100000 = 0.00046
	//
	// There are some general buckets for these values that may be used for different races:
	// Avian/Fey (k=0.00035 to 0.00040): High Elves, Kobolds, Sylphs.
	// Standard  (k = 0.00045 to 0.00050): Humans, Gnolls, Goblins, Orcs.
	// Solid     (k = 0.00055 to 0.00065): Dwarves, Troglodytes, Stone Giants.
	//
	// For our purposes GM is rolled into FM so that the Race.maleWeightModifier() and Race.femaleWeightModifier() functions will take into account both

	// The racial density constant is a value that represents the average density of the tissue and bone structure
	float racial_density{race_->buildModifier() / 100000.f};
	float racial_frame_modifier{(male_ ? race_->maleWeightModifier() : race_->femaleWeightModifier()) / 100.f};

	// Get the value as a percentage modifier based on the character's build modifier so that a value of 0 will be 1.0 (no change), a value of -10 will be 0.72 (28% lighter) and a value of 10 will be 1.28 (28% heavier).
	// We use the 2.8 to give a wider spread of values to make it more meaningful.
	float character_frame_modifier{(100.0f + (char_build_modifier * 2.8f)) / 100.0f};

	weight_ = static_cast<int>((racial_density * std::pow(height, 3)) * racial_frame_modifier * character_frame_modifier);

	// get the lifespan
	int constitution_potential = stats_.at(StatType::kConstitution).potential();
	float conMod = (constitution_potential - 50) / 100.0f;
	float lifespanModifiedAverage = (race_->averageLifespan() * (1.0f + conMod));
	float lifespanStdDeviation = race_->averageLifespan() / 10.0f;
	std::normal_distribution lifespanDistribution{lifespanModifiedAverage, lifespanStdDeviation};
	lifespan_ = static_cast<int>(lifespanDistribution(Random::mt));
}

/* Stat allocations */
void CharacterBuilder::setInitialStat(StatType::Type stat_type, int temp_value, int potential_value) {
	Stat& stat = initial_stats_[stat_type]; // This will default construct a new Stat object if the stat has not been touched yet.
	stat.updateTemporary(temp_value);
	stat.setPotential(potential_value);
}

/* Hobby data */
int CharacterBuilder::getMaxHobbyRanksForSkill(const SubcategoriedSkillData* skill) const {
	const SkillCategoryData& category = skill->skillData().category();
	return getMaxHobbyRanksForCategory(&category);
}

int CharacterBuilder::getMaxHobbyRanksForCategory(const SkillCategoryData* category) const {
	const SkillDevelopmentCost& cost = category_development_costs_.at(category);
	int max_ranks = cost.maxRanks();
	return max_ranks;
}

std::set<const SpellListData*> CharacterBuilder::getAdolescentSpellListOptions() const {
	std::set<const SpellListData*> spell_list_options;

	// First we need to get the characters realms to know which spell lists they have access to, as the spell list choices are based on the realms that the character has access to.
	std::set<RealmType::Type> character_realms = magical_realms_;
	for (const SpellListData& spell_list : object_factory_->getAll<SpellListData>()) {
		// Only open lists are allowed for selection
		if (spell_list.type() != SpellListType::kOpen) {
			continue;
		}

		// The character must have all the realms that the list has so they can't pick hybrid lists if they only know one realm.
		int count{0};
		for (RealmType::Type realm : spell_list.realms()) {
			if (character_realms.find(realm) != character_realms.end()) {
				count++;
			} else {
				break;
			}
		}
		if (count >= spell_list.realms().size()) {
			spell_list_options.insert(&spell_list);
		}
	} // end for all spell lists

	return spell_list_options;
}

void CharacterBuilder::setSpellListCategories() {
	spell_list_categories_.clear();
	for (const SpellListData& spell_list : object_factory_->getAll<SpellListData>()) {
		const SkillCategoryData* category = nullptr;

		if (prof_base_spell_list_choices_.contains(&spell_list)) {
			// Short circuit if this is in the set of base lists
			category = &object_factory_->get<SkillCategoryData>("SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS");
		} else {
			category = getSkillCategoryForSpellList(magical_realms_, spell_list, *object_factory_);
		}

		spell_list_categories_[category].emplace(&spell_list);
	} // end for all spell lists
}

void CharacterBuilder::addHobbySkillRankChoice(const SubcategoriedSkillData& skill, int ranks) {
	if (ranks > getMaxHobbyRanksForSkill(&skill)) {
		throw std::runtime_error("CharacterBuilder: Cannot allocate more than the maximum hobby skill ranks for this skill.");
	}
	hobby_skill_ranks_.insert_or_assign(&skill, ranks);
}

void CharacterBuilder::addHobbyCategoryRankChoice(const SkillCategoryData& category, int ranks) {
	if (ranks > getMaxHobbyRanksForCategory(&category)) {
		throw std::runtime_error("CharacterBuilder: Cannot allocate more than the maximum hobby skill ranks for this category.");
	}
	hobby_category_ranks_.insert_or_assign(&category, ranks);
}

void CharacterBuilder::addAdolescentLanguageChoice(const LanguageRanks language) {
	adolescent_language_choices_.emplace(language);
	applyLanguageAbility(language);
}

void CharacterBuilder::setAdolescentSpellListChoice(const SpellListData& spell_list) {
	adolescent_spell_list_choice_ = &spell_list;
}

void CharacterBuilder::applyHobbyChoices() {
	for (const auto& [skill, ranks] : hobby_skill_ranks_) {
		int current_ranks = skill_ranks_.find(skill) != skill_ranks_.end() ? skill_ranks_.at(skill) : 0;
		skill_ranks_.insert_or_assign(skill, current_ranks + ranks);
	}

	for (const auto& [category, ranks] : hobby_category_ranks_) {
		int current_ranks = category_ranks_.find(category) != category_ranks_.end() ? category_ranks_.at(category) : 0;
		category_ranks_.insert_or_assign(category, current_ranks + ranks);
	}

	for (const auto& language_ability : adolescent_language_choices_) {
		applyLanguageAbility(language_ability);
	}

	if (num_adolescent_spell_list_ranks_ > 0 && adolescent_spell_list_choice_ != nullptr) {
		spell_list_ranks_.emplace(adolescent_spell_list_choice_, num_adolescent_spell_list_ranks_);
	}
}

/* Background data */

void CharacterBuilder::addBackgroundLanguageChoice(const LanguageRanks language) {
	background_language_choices_.emplace(language);
}

void CharacterBuilder::makeBackgroundStatGainRolls() {
	// First we need to prepopulate the background stats with the base choices
	background_stats_ = stats_; // We can just copy the current stats as the background stat gains are based on the current stats after the initial choices and hobby choices have been applied.
	for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
		background_stats_[stat_type].performStatGainRoll();
	}
}

void CharacterBuilder::backgroundMoneyRoll(int roll) {
	background_extra_gold_ = 0;
	// If the roll is less than 0 make a random d100 roll.
	if (roll <= 0) {
		roll = archid::Dice(100).roll().result();
	}
	if (roll < 3)
		background_extra_gold_ += 1;
	else if (roll < 6)
		background_extra_gold_ += 2;
	else if (roll < 16)
		background_extra_gold_ += 5;
	else if (roll < 26)
		background_extra_gold_ += 10;
	else if (roll < 36)
		background_extra_gold_ += 15;
	else if (roll < 46)
		background_extra_gold_ += 20;
	else if (roll < 56)
		background_extra_gold_ += 30;
	else if (roll < 66)
		background_extra_gold_ += 35;
	else if (roll < 71)
		background_extra_gold_ += 40;
	else if (roll < 76)
		background_extra_gold_ += 50;
	else if (roll < 81)
		background_extra_gold_ += 60;
	else if (roll < 86)
		background_extra_gold_ += 70;
	else if (roll < 91)
		background_extra_gold_ += 80;
	else if (roll < 95)
		background_extra_gold_ += 100;
	else if (roll < 98)
		background_extra_gold_ += 125;
	else if (roll < 100)
		background_extra_gold_ += 150;
	else
		background_extra_gold_ += 200;
}

void CharacterBuilder::generateBackgroundItems(int item_count) {
	for (int i = 1; i <= item_count; i++) {
		int roll = archid::Dice(100).roll().result();
		if (roll <= 5)
			background_items_.push_back("+1 spell adder or one special bread/poison/herb");
		else if (roll <= 10)
			background_items_.push_back("+1 spell adder or two +5 non-magic items");
		else if (roll <= 20)
			background_items_.push_back("+1 spell adder or one +10 non-magic item");
		else if (roll <= 30)
			background_items_.push_back("+1 spell adder or two +5 magic items");
		else if (roll <= 65)
			background_items_.push_back("+1 spell adder or one +10 magic item");
		else if (roll == 66)
			background_items_.push_back("+3 spell adder or a loyal domesticated animal (e.g., a dog) or one +20 non-magic item");
		else if (roll <= 75)
			background_items_.push_back("a Daily III spell item or a +2 spell adder, or three +5 non-magic items or three doses of a potion (with a level 1-5 spell)");
		else if (roll <= 80)
			background_items_.push_back("Daily III spell item or a +2 spell adder, or one +15 non-magic item or three doses of a potion (with a level 1-5 spell)");
		else if (roll <= 85)
			background_items_.push_back("Daily IV spell item, or a +2 spell adder, or three +5 magic items, or five doses of a potion (with a level 1-5 spell)");
		else if (roll <= 90)
			background_items_.push_back("Daily IV spell item, or a +2 spell adder, or one +15 magic item, or five doses of a potion (with a level 1-5 spell)");
		else if (roll <= 95)
			background_items_.push_back("+3 spell adder or two +10 magic items or two Daily III items.");
		else if (roll <= 97)
			background_items_.push_back("+3 spell adder or one +20 magic item or a Daily IV item");
		else if (roll <= 98)
			background_items_.push_back("+3 spell adder or a Daily VI item or three +10 magic items");
		else if (roll <= 99)
			background_items_.push_back("+3 spell adder or a Daily VII item or two +20 magic items");
		else
			background_items_.push_back("+3 spell adder or a Daily VIII item or a loyal unusual creature (e.g., a monster).");
	}
}

void CharacterBuilder::applyBackgroundChoices() {
	if (background_stats_.size() > 0) {
		stats_ = background_stats_;
		calculateDevelopmentPoints(background_stats_); // We need to recalculate the development points after applying the background stat gains as the temporary values may have changed which would affect the total development points.
	}

	total_gold_ += background_extra_gold_;

	for (const auto& language_ability : background_language_choices_) {
		applyLanguageAbility(language_ability);
	}

	for (const auto& [skill, bonus] : background_skill_special_bonuses_) {
		int current_bonus = skill_special_bonuses_.find(skill) != skill_special_bonuses_.end() ? skill_special_bonuses_.at(skill) : 0;
		skill_special_bonuses_.insert_or_assign(skill, current_bonus + bonus);
	}

	for (const auto& [category, bonus] : background_category_special_bonuses_) {
		int current_bonus = category_special_bonuses_.find(category) != category_special_bonuses_.end() ? category_special_bonuses_.at(category) : 0;
		category_special_bonuses_.insert_or_assign(category, current_bonus + bonus);
	}

	for (const auto& item : background_items_) {
		total_items_.push_back(item);
	}
}

/* Utility functions */
void CharacterBuilder::calculateDevelopmentPoints(std::unordered_map<StatType::Type, Stat>& stats) {
	int development_points = 0;
	for (const auto& [stat_type, stat] : stats) {
		if (StatType::isDevelopment(stat_type)) {
			development_points += stat.temporary();
		}
	}
	development_points_ = (development_points / 5);
}

void CharacterBuilder::applyLanguageAbility(const LanguageRanks& ability) {
	// If the language already exists in the character's language abilities, we need to compare the existing ability with the new ability and keep the highest ranks in each category (spoken, written, somantic) to ensure that the character
	// has the best possible ability for that language based on their choices. If the language does not already exist in the character's language abilities, we can simply add the new ability as is.
	if (language_abilities_.contains(ability)) {
		// Since sets contain immutable objects we need to extract the existing ability, compare it with the new ability, and then reinsert the updated ability back into the set. This is a bit of a workaround to update the existing ability
		// in the set since we can't modify the objects in place.
		auto node = language_abilities_.extract(ability);
		if (ability.isSpoken() && ability.spokenRanks() > node.value().spokenRanks()) {
			node.value().updateSpokenRanks(ability.spokenRanks() - node.value().spokenRanks());
		}
		if (ability.isWritten() && ability.writtenRanks() > node.value().writtenRanks()) {
			node.value().updateWrittenRanks(ability.writtenRanks() - node.value().writtenRanks());
		}
		if (ability.isSomatic() && ability.somaticRanks() > node.value().somaticRanks()) {
			node.value().updateSomaticRanks(ability.somaticRanks() - node.value().somaticRanks());
		}
		language_abilities_.insert(std::move(node));
	} else {
		language_abilities_.emplace(ability);
	}
}

SkillDevelopmentType::Type CharacterBuilder::getHighestPrecedenceDevelopmentType(SkillDevelopmentType::Type first, SkillDevelopmentType::Type second) const {
	if (first == SkillDevelopmentType::Type::kRestricted || second == SkillDevelopmentType::Type::kRestricted) {
		return SkillDevelopmentType::Type::kRestricted;
	} else if (first == SkillDevelopmentType::Type::kOccupational || second == SkillDevelopmentType::Type::kOccupational) {
		return SkillDevelopmentType::Type::kOccupational;
	} else if (first == SkillDevelopmentType::Type::kEveryman || second == SkillDevelopmentType::Type::kEveryman) {
		return SkillDevelopmentType::Type::kEveryman;
	} else {
		return SkillDevelopmentType::Type::kStandard;
	}
}

void CharacterBuilder::addBackgroundSkillSpecialBonus(const SubcategoriedSkillData* skill, int bonus) {
	background_skill_special_bonuses_.emplace(skill, bonus);
}

void CharacterBuilder::addBackgroundCategorySpecialBonus(const SkillCategoryData* category, int bonus) {
	background_category_special_bonuses_.emplace(category, bonus);
}

const SkillProgressionTypeData* CharacterBuilder::getPpProgression() {
	std::set<const SkillProgressionTypeData*> progressions{};
	std::set<std::string_view> realm_names{}; // We sort the realm names so we can generate a consistent ID for the progression type to avoid duplication.
	for (RealmType::Type realm : magical_realms_) {
		if (isMagical(realm)) {
			realm_names.insert(RealmType::toString(realm));
			progressions.insert(realm_progressions_.at(realm));
		}
	}

	if (progressions.size() == 1) {
		return *progressions.begin();
	} else if (progressions.size() > 1) {
		SkillProgressionTypeData* combined{};
		std::string id = "PP";
		id += "_" + std::string(race_->id());
		for (const auto& realm_name : realm_names) {
			id += "_" + std::string(realm_name);
		}
		combined = &object_factory_->get<SkillProgressionTypeData>(id);
		computeCombinedProgression(progressions, *combined);
		return combined;
	}
	return nullptr;
}

/* ------------------------------------------------------------------ */
/* Automation                                                         */
/* ------------------------------------------------------------------ */
void CharacterBuilder::autoStats(int min, int primeFloorMin, int numPrimeFloorMin) {
	auto_builder_->autoStats(*this, min, primeFloorMin, numPrimeFloorMin);
}

/* ------------------------------------------------------------------ */
/* Free functions                                                     */
/* ------------------------------------------------------------------ */

const SkillCategoryData* getSkillCategoryForSpellList(const std::set<RealmType::Type>& realms, const SpellListData& spell_list, PersistentObjectManager& object_factory) {
	// We have the categories pre-defined based on the realms and type of spell list in the setSpellListCategories function so we just need to find which category the spell list belongs to based on its realms and type.
	std::string arcane_base_id = "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS";
	std::string arcane_closed_id = "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS";
	std::string arcane_open_id = "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS";
	std::string other_base_id = "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS";
	std::string other_closed_id = "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS";
	std::string other_open_id = "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS";
	std::string other_training_id = "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE";
	std::string own_other_base_id = "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS";
	std::string own_closed_id = "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS";
	std::string own_open_id = "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS";
	std::string own_training_id = "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE";

	if (spell_list.realms().contains(RealmType::kArcane)) {
		if (spell_list.type() == SpellListType::kBase) {
			return &object_factory.get<SkillCategoryData>(arcane_base_id);
		} else if (spell_list.type() == SpellListType::kClosed) {
			return &object_factory.get<SkillCategoryData>(arcane_closed_id);
		} else if (spell_list.type() == SpellListType::kOpen) {
			return &object_factory.get<SkillCategoryData>(arcane_open_id);
		}
	} else {
		bool own_realm = std::ranges::includes(realms, spell_list.realms());
		if (own_realm) {
			if (spell_list.type() == SpellListType::kBase) {
				return &object_factory.get<SkillCategoryData>(own_other_base_id);
			} else if (spell_list.type() == SpellListType::kClosed) {
				return &object_factory.get<SkillCategoryData>(own_closed_id);
			} else if (spell_list.type() == SpellListType::kOpen) {
				return &object_factory.get<SkillCategoryData>(own_open_id);
			} else if (spell_list.type() == SpellListType::kTrainingPackage) {
				return &object_factory.get<SkillCategoryData>(own_training_id);
			}
		} else {
			if (spell_list.type() == SpellListType::kBase) {
				return &object_factory.get<SkillCategoryData>(other_base_id);
			} else if (spell_list.type() == SpellListType::kClosed) {
				return &object_factory.get<SkillCategoryData>(other_closed_id);
			} else if (spell_list.type() == SpellListType::kOpen) {
				return &object_factory.get<SkillCategoryData>(other_open_id);
			} else if (spell_list.type() == SpellListType::kTrainingPackage) {
				return &object_factory.get<SkillCategoryData>(other_training_id);
			}
		}
	}
	return nullptr;
}

void ensureValidTemporaryStats(std::vector<int>& temp_stats, int min, int primeFloorMin, int numPrimeFloorMin) {
	int floor = min - 1;              // We use this value to ensure that we can generate a number between min and 100 inclusive by adding it to the result of the dice roll which will give us a number between 1 and (100 - floor) inclusive.
	archid::Dice dice(100 - min + 1); // This will give us a number that when added to floor will result in a number between min and 100 inclusiove.

	// 1. Ensure that all stats are > min by re-rolling any stat that is below the minimum value until all stats are above the minimum. We do this before sorting to ensure that we have a good distribution of values to sort and select from.
	for (int& stat : temp_stats) {
		if (stat < min) {
			stat = dice.roll(false).result() + floor;
		}
	}

	if (numPrimeFloorMin > 0) {
		// 2. Partially sort to put numPrimeFloorMin largest elements at the front (descending)
		std::ranges::partial_sort(temp_stats, temp_stats.begin() + numPrimeFloorMin, std::ranges::greater());

		// 3. Check if we have at least numPrimeFloorMin values >= primeFloorMin, if not re-roll the necessary number of values to get at least numPrimeFloorMin values >= primeFloorMin
		for (int i = 0; i < numPrimeFloorMin; i++) {
			if (temp_stats[i] < primeFloorMin) {
				do {
					temp_stats[i] = dice.roll(false).result() + floor;
				} while (temp_stats[i] < primeFloorMin);
			}
		}
	}
}

} // namespace rm::game::character
