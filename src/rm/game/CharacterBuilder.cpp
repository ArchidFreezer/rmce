#include <CharacterBuilder.h>
#include <EnumIterator.h>

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

void CharacterBuilder::reset(bool aggregate_state_only) {
	if (!aggregate_state_only) {
		built_ = false;
		name_.clear();
		race_ = nullptr;
		culture_ = nullptr;
		culture_type_ = nullptr;
		profession_ = nullptr;
		magical_realms_.clear();
		num_hobby_skill_ranks_ = 0;
		num_spell_list_ranks_ = 0;
		race_category_everyman_choices_.clear();
		race_adolescent_language_choices_.clear();
		culture_type_category_skill_ranks_.clear();
		base_spell_list_choices_.clear();
		prof_skill_subcategory_development_type_choices_.clear();
		prof_skill_development_type_choices_.clear();
		prof_category_development_type_choices_.clear();
		prof_group_development_type_choices_.clear();
		hobby_skill_ranks_.clear();
		hobby_category_ranks_.clear();
		background_language_choices_.clear();
	}
	// Clear the aggregated state regardless of whether we are resetting the entire builder or not, as the aggregated state needs to be recalculated based on the current choices after any reset.
	language_abilities_.clear();
	realm_progressions_.clear();
	stats_.clear();
	everyman_skills_.clear();
	restricted_skills_.clear();
	everyman_skill_categories_.clear();
	restricted_skill_categories_.clear();
	skill_ranks_.clear();
	skill_professional_bonuses_.clear();
	skillsub_development_types_.clear();
	skill_development_types_.clear();
	category_ranks_.clear();
	category_professional_bonuses_.clear();
	category_special_bonuses_.clear();
	category_development_types_.clear();
	group_professional_bonuses_.clear();
	group_special_bonuses_.clear();
	group_development_types_.clear();
	spell_list_ranks_.clear();
}

void CharacterBuilder::recalculateAggregatedState() {
	reset(true); // We reset the builder to clear the aggregated state and then reapply the choices to recalculate the aggregated state based on the current choices. This is a simple way to ensure consistency when selections are
	             // modified, but it may not be the most efficient way to do this if there are a large number of choices or if the choices have complex interactions that require a lot of processing to apply.
	applyRace();
	applyRaceChoices();
	applyCultureType();
	applyCultureTypeChoices();
	applyCulture();
	applyCultureChoices();
	applyProfession();
	applyProfessionChoices();
}

void CharacterBuilder::setIntialChoices(rm::PersistentObjectManager& object_factory, const std::string& name, const std::string& race_id, const std::string& culture_id, const std::string& profession_id,
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
	recalculateAggregatedState();
}

/*
 * @brief Apply the effects of the given race data to the character being built.
 *
 * This includes setting the race for the character and applying any racial innate abilities and choices associated with the race.
 *
 * @param race The race data to apply to the character.
 */
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
		setBestLanguageAbility(language_ability);
	}

	// Everyman and restricted skills - we need to insert these into a set to ensure that there are no duplicates and that they are easily accessible for checking if a skill is an everyman skill or not when applying the effects of the
	// character's choices later on in the building process
	for (const SubcategoriedSkillData* skill : race_->everymanSkills()) {
		everyman_skills_.insert(skill);
	}

	for (const SubcategoriedSkillData* skill : race_->restrictedSkills()) {
		restricted_skills_.insert(skill);
	}

	for (const SkillCategoryData* category : race_->everymanSkillCategories()) {
		everyman_skill_categories_.insert(category);
	}

	for (const SkillCategoryData* category : race_->restrictedSkillCategories()) {
		restricted_skill_categories_.insert(category);
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
	for (const auto& [language_name, language_ability] : race_adolescent_language_choices_) {
		setBestLanguageAbility(language_ability);
	}
}

void CharacterBuilder::applyCultureType() {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply culture type after character has been built.");
	}
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
	// The culture only provides options and does not have any fixed effects so there is nothing to apply for the culture itself
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
		SkillDevelopmentType::Type current_type = skillsub_development_types_.find(skill) != skillsub_development_types_.end() ? skillsub_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skillsub_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
	}

	// Set the development type for all skills in a skil category
	for (const auto& [category, development_type] : profession_->skillCategorySkillDevelopmentTypes()) { // TODO should be all skills in category, not the category itself, we need to apply the development type to all skills in the category
		for (const SkillData& skill : object_factory_->getAll<SkillData>()) {
			if (&skill.category() == category) {
				SkillDevelopmentType::Type current_type = skill_development_types_.find(&skill) != skill_development_types_.end() ? skill_development_types_.at(&skill) : SkillDevelopmentType::Type::kStandard;
				skill_development_types_.insert_or_assign(&skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
			}
		}
	}

	// Set the development type for all skills in a skill group
	for (const auto& [group, development_type] : profession_->skillGroupSkillDevelopmentTypes()) {
		std::set<const SkillCategoryData*> categories_in_group; // We need to get all the categories in the group first so that we can then get all the skills in those categories and apply the development type to them
		for (const SkillCategoryData& category : object_factory_->getAll<SkillCategoryData>()) {
			if (&category.group() == group) {
				categories_in_group.insert(&category);
			}
		}
		for (const SkillData& skill : object_factory_->getAll<SkillData>()) {
			if (categories_in_group.find(&skill.category()) != categories_in_group.end()) {
				SkillDevelopmentType::Type current_type = skill_development_types_.find(&skill) != skill_development_types_.end() ? skill_development_types_.at(&skill) : SkillDevelopmentType::Type::kStandard;
				skill_development_types_.insert_or_assign(&skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
			}
		}
	}
}

void CharacterBuilder::applyProfessionChoices() {
	// We know the character knows these lists, but set the ranks to 0 as no ranks have been allocated yet.
	for (const auto& spellList : base_spell_list_choices_) {
		spell_list_ranks_.insert_or_assign(spellList, 0);
	}

	// Set of skill subcategories with their development type changed
	for (const auto& [skill, development_type] : prof_skill_subcategory_development_type_choices_) {
		SkillDevelopmentType::Type current_type = skillsub_development_types_.find(skill) != skillsub_development_types_.end() ? skillsub_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skillsub_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
	}

	// Set of skills (base or subcategories) with their development type changed
	for (const auto& [skill, development_type] : prof_skill_development_type_choices_) {
		SkillDevelopmentType::Type current_type = skillsub_development_types_.find(skill) != skillsub_development_types_.end() ? skillsub_development_types_.at(skill) : SkillDevelopmentType::Type::kStandard;
		skillsub_development_types_.insert_or_assign(skill, getHighestPrecedenceDevelopmentType(current_type, development_type));
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

/*
 * @brief Set the best language ability for a given language by comparing the existing ability with the new ability and keeping the highest ranks in each category (spoken, written, somantic).
 */
void CharacterBuilder::setBestLanguageAbility(const LanguageAbility& ability) {
	LanguageAbility best_ability = ability;
	if (language_abilities_.find(ability.language()) != language_abilities_.end()) {
		LanguageAbility existing_ability = language_abilities_.at(ability.language());
		best_ability.updateSpokenRanks(std::max(existing_ability.spoken(), ability.spoken()));
		best_ability.updateWrittenRanks(std::max(existing_ability.written(), ability.written()));
		best_ability.updateSomanticRanks(std::max(existing_ability.somatic(), ability.somatic()));
	}
	language_abilities_.insert_or_assign(ability.language(), best_ability);
}

/*
 * @brief Get the highest precedence skill development type between two given types.
 *
 * The precedence order is as follows: Restricted > Occupational > Everyman > Standard.
 *
 * @param first The first skill development type to compare.
 * @param second The second skill development type to compare.
 * @return The highest precedence skill development type between the two given types.
 */
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

void CharacterBuilder::setStat(StatType::Type stat_type, int temp_value, int potential_value) {
	Stat& stat = stats_[stat_type]; // This will default construct a new Stat object if the stat has not been touched yet.
	stat.setTemporary(temp_value);
	stat.setPotential(potential_value);
}

} // namespace rm::game::character