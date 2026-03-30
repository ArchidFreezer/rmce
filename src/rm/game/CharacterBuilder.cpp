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

void CharacterBuilder::setIntialChoices(rm::PersistentObjectManager& object_factory, const std::string& name, const std::string& race_id, const std::string& culture_id, const std::string& profession_id,
                                        const std::set<RealmType::Type> magical_realms) {
	name_ = name;
	race_ = &object_factory.get < RaceData >(race_id);
	culture_ = &object_factory.get < CultureData >(culture_id);
	culture_type_ = &culture_->cultureType();
	profession_ = &object_factory.get < ProfessionData >(profession_id);
	magical_realms_ = magical_realms;
}

/*
 * @brief Apply the effects of the given race data to the character being built.
 *
 * This includes setting the race for the character and applying any racial innate abilities and choices associated with the race.
 *
 * @param race The race data to apply to the character.
 */
void CharacterBuilder::applyRace(Character& character) {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply race after character has been built.");
	}

	// Stat bonuses - we need to reset any bonuses not applied by the race to ensure that if the race was previously set the character does not end up with the bonuses from both
	for (auto stat_type : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
		int bonus = 0;
		if (race_->statBonuses().find(stat_type) != race_->statBonuses().end())
			bonus = race_->statBonuses().at(stat_type);
		character.stats_.at(stat_type).setRacialBonus(bonus);
	}

	// language abilities, both fixed and adolescentchoices
	for (const auto& [language_name, language_ability] : race_->startingLanguageAbilities()) {
		setBestLanguageAbility(character, language_ability);
	}
	for (const auto& [language_name, language_ability] : race_adolescent_language_choices_) {
		setBestLanguageAbility(character, language_ability);
	}

	// realm progressions
	for (const auto& [realm_type, progression] : race_realm_progressions_) {
		character.setRealmProgression(realm_type, *progression);
	}
}

/*
 * @brief Set the best language ability for a given language by comparing the existing ability with the new ability and keeping the highest ranks in each category (spoken, written, somantic).
 */
void CharacterBuilder::setBestLanguageAbility(Character& character, const LanguageAbility& ability) {
	LanguageAbility best_ability = ability;
	if (character.hasLanguageAbility(ability.language())) {
		LanguageAbility existing_ability = character.languageAbilities().at(ability.language());
		best_ability.updateSpokenRanks(std::max(existing_ability.spoken(), ability.spoken()));
		best_ability.updateWrittenRanks(std::max(existing_ability.written(), ability.written()));
		best_ability.updateSomanticRanks(std::max(existing_ability.somatic(), ability.somatic()));
	}
	character.setLanguageAbility(best_ability);
}

} // namespace rm::game::character