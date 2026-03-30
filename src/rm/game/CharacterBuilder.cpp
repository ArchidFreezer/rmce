#include <CharacterBuilder.h>

namespace rm::game::character {

Character& CharacterBuilder::build(rm::PersistentObjectManager& object_factory) {
	// Do some sanity checking first to make sure that the builder has been properly set up before we start building the character.
	if (race_ == nullptr) {
		throw std::runtime_error("CharacterBuilder: Race must be set before building a character.");
	}

	Character& character = object_factory.get<Character>();
	character_.setName(name_);
	applyRace(*race_);
	built_ = true;
	return character_;
}

void CharacterBuilder::setName(const std::string& name) {
	name_ = name;
}

void CharacterBuilder::setRace(const RaceData& race) {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot set race after character has been built.");
	}
	race_ = &race;
	race_adolescent_language_choices_.clear();
}

void CharacterBuilder::applyRace(const RaceData& race) {
	if (built_) {
		throw std::runtime_error("CharacterBuilder: Cannot apply race after character has been built.");
	}

	character_.setRace(race);
	// Stat bonuses - we need to reset any existing racial bonuses to ensure that if the race was previously set the character does not end up with the bonuses from both

	for (const auto& [stat_type, bonus] : race.statBonuses()) {
		character_.stats_.at(stat_type).setRacialBonus(bonus);
	}

	// language abilities, both fixed and adolescentchoices
	for (const auto& [language_name, language_ability] : race.startingLanguageAbilities()) {
		setBestLanguageAbility(language_ability);
	}
	for (const auto& [language_name, language_ability] : race_adolescent_language_choices_) {
		setBestLanguageAbility(language_ability);
	}
}

void CharacterBuilder::setBestLanguageAbility(const LanguageAbility& ability) {
	LanguageAbility best_ability = ability;
	if (character_.hasLanguageAbility(ability.language())) {
		LanguageAbility existing_ability = character_.languageAbilities().at(ability.language());
		best_ability.updateSpokenRanks(std::max(existing_ability.spoken(), ability.spoken()));
		best_ability.updateWrittenRanks(std::max(existing_ability.written(), ability.written()));
		best_ability.updateSomanticRanks(std::max(existing_ability.somatic(), ability.somatic()));
	}
	character_.setLanguageAbility(best_ability);
}

} // namespace rm::game::character