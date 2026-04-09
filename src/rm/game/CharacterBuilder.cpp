#include <CharacterBuilder.h>
#include <EnumIterator.h>
#include <TrainingPackageCostTable.h>

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
	character.setName(name_);
	// TODO: Apply the complted builder properites to a new character.
	built_ = true;
	return character;
}

void CharacterBuilder::reset(bool aggregate_state_only, bool clear_stats) {
	if (clear_stats) {
		initial_stats_.clear();
	}
	if (!aggregate_state_only) {
		built_ = false;
		name_.clear();
		race_ = nullptr;
		culture_ = nullptr;
		culture_type_ = nullptr;
		profession_ = nullptr;
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
	total_gold_ = 0;
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
	reset(true); // We reset the builder to clear the aggregated state and then reapply the choices to recalculate the aggregated state based on the current choices. This is a simple way to ensure consistency.
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

void CharacterBuilder::setPrimaryDefinition(rm::PersistentObjectManager& object_factory, const std::string& name, const std::string& race_id, const std::string& culture_id, const std::string& profession_id,
                                        const std::set<RealmType::Type> magical_realms) {
	// This should always be the first call so we set the object factory here so it is available for the rest of the character creation process.
	object_factory_ = &object_factory;

	// First we need to completely reset the builder to ensure that any previous choices are cleared and do not interfere with the new choices being set. This is important as the initial choices drive the rest of the character creation
	// process and if there are leftover choices from a previous setup it could lead to inconsistent state or unintended consequences.
	reset(false);

	name_ = name;
	race_ = &object_factory.get<RaceData>(race_id);
	culture_ = &object_factory.get<CultureData>(culture_id);
	culture_type_ = &culture_->cultureType();
	profession_ = &object_factory.get<ProfessionData>(profession_id);
	magical_realms_ = magical_realms;
	adolescent_spell_list_choices_ = getAdolescentSpellListChoices();
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

	// Training package costs
	{
		using namespace rm::rule::table;
		std::string id = "TRAININGPACKAGECOSTTABLE_TRAINING_PACKAGE_COST_TABLE";
		TrainingPackageCostTable& table = object_factory_->get<TrainingPackageCostTable>(id);
		for (const auto& training_package_wrapper : object_factory_->getAll<TrainingPackageData>()) {
			const TrainingPackageData* training_package = &training_package_wrapper.get();
			int cost = table.cell(profession_, training_package);
			training_package_costs_.insert_or_assign(training_package, cost);
		}
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

/* Stat allocations */
void CharacterBuilder::setInitialStat(StatType::Type stat_type, int temp_value, int potential_value) {
	Stat& stat = initial_stats_[stat_type]; // This will default construct a new Stat object if the stat has not been touched yet.
	stat.setTemporary(temp_value);
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

std::set<const SpellListData*> CharacterBuilder::getAdolescentSpellListChoices() const {
	std::set<const SpellListData*> spell_list_choices;

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
			spell_list_choices.insert(&spell_list);
		}
	} // end for all spell lists

	return spell_list_choices;
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

void CharacterBuilder::addAdolescentLanguageChoice(const LanguageAbility language) {
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

void CharacterBuilder::addBackgroundLanguageChoice(const LanguageAbility language) {
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

void CharacterBuilder::applyLanguageAbility(const LanguageAbility& ability) {
	// If the language already exists in the character's language abilities, we need to compare the existing ability with the new ability and keep the highest ranks in each category (spoken, written, somantic) to ensure that the character
	// has the best possible ability for that language based on their choices. If the language does not already exist in the character's language abilities, we can simply add the new ability as is.
	if (language_abilities_.contains(ability)) {
		// Since sets contain immutable objects we need to extract the existing ability, compare it with the new ability, and then reinsert the updated ability back into the set. This is a bit of a workaround to update the existing ability
		// in the set since we can't modify the objects in place.
		auto node = language_abilities_.extract(ability);
		if (ability.isSpoken() && ability.spoken() > node.value().spoken()) {
			node.value().updateSpokenRanks(ability.spoken() - node.value().spoken());
		}
		if (ability.isWritten() && ability.written() > node.value().written()) {
			node.value().updateWrittenRanks(ability.written() - node.value().written());
		}
		if (ability.isSomatic() && ability.somatic() > node.value().somatic()) {
			node.value().updateSomanticRanks(ability.somatic() - node.value().somatic());
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

} // namespace rm::game::character