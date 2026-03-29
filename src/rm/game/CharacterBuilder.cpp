#include <CharacterBuilder.h>
#include <Dice.h>

namespace rm::game::character {
using namespace rm::rule::enums;

void CharacterBuilder::rollStats(Character& character) {
	// Roll the stats for the character using the standard method of rolling 2d10 for each stat
	for (int i = 0; i < 10; ++i) {
		int temp_value = archid::Dice(100).roll().result();
		character.stats_[static_cast<StatType::Type>(i)].setTemporary(temp_value);
	}
}

} // namespace rm::game::character