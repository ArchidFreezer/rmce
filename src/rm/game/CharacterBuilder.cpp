#include <CharacterBuilder.h>
#include <EnumIterator.h>

namespace rm::game::character {

/*
 * @brief Build and return a Character object based on the attributes and stats set in the builder.
 *
 * This method performs sanity checks to ensure that the builder has been properly set up before building the character.
 */
Character& CharacterBuilder::build(rm::PersistentObjectManager& object_factory) {
	// Do some sanity checking first to make sure that the builder has been properly set up before we start building the character.
	if (race_ == nullptr) {
		throw std::runtime_error("CharacterBuilder: Race must be set before building a character.");
	}

	Character& character = object_factory.get<Character>();
	character.setName(name_);
	// TODO: Apply the complted builder properites to a new character.
	built_ = true;
	return character;
}

void CharacterBuilder::reset() {
	built_ = false;
	name_.clear();
	race_ = nullptr;
	culture_ = nullptr;
	culture_type_ = nullptr;
	profession_ = nullptr;
	magical_realms_.clear();
	race_category_everyman_choices_.clear();
	race_adolescent_language_choices_.clear();
	language_abilities_.clear();
	realm_progressions_.clear();
	stats_.clear();
	everyman_skills_.clear();
	restricted_skills_.clear();
	everyman_skill_categories_.clear();
	restricted_skill_categories_.clear();
	skill_bonuses_.clear();
}

void CharacterBuilder::setIntialChoices(rm::PersistentObjectManager& object_factory, const std::string& name, const std::string& race_id, const std::string& culture_id, const std::string& profession_id,
                                        const std::set<RealmType::Type> magical_realms) {

	// First we need to reset the builder to ensure that any previous choices are cleared and do not interfere with the new choices being set. This is important as the initial choices drive the rest of the character creation process and if
	// there are leftover choices from a previous setup it could lead to inconsistent state or unintended consequences.
	reset();

	name_ = name;
	race_ = &object_factory.get<RaceData>(race_id);
	applyRace();

	culture_ = &object_factory.get<CultureData>(culture_id);


	culture_type_ = &culture_->cultureType();


	profession_ = &object_factory.get<ProfessionData>(profession_id);
	magical_realms_ = magical_realms;
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
		stats_.at(stat_type).setRacialBonus(bonus);
	}

	// language abilities, both fixed and adolescentchoices
	for (const auto& [language_name, language_ability] : race_->startingLanguageAbilities()) {
		setBestLanguageAbility(language_ability);
	}
	for (const auto& [language_name, language_ability] : race_adolescent_language_choices_) {
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

} // namespace rm::game::character