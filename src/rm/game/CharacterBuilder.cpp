#include <CharacterBuilder.h>

namespace rm::game::character {
using namespace rm::rule::enums;

Character& CharacterBuilder::build(rm::PersistentObjectManager& object_factory) {

	// Do some sanity checking first to make sure that the builder has been properly set up before we start building the character.
	if (race_ == nullptr) {
		throw std::runtime_error("CharacterBuilder: Race must be set before building a character.");
	}

	Character& character = object_factory.get<Character>();
	
	return character;
}

} // namespace rm::game::character