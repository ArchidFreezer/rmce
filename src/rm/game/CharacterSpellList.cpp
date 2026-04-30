#include <CharacterSpellList.h>

namespace rm::game::character {

int SpellList::bonus() const {
	int bonus{0};
	bonus += progression_type_ != nullptr ? progression_type_->bonus(ranks_) : 0;
	bonus += category_ != nullptr ? category_->bonus() : 0;
	bonus += profession_bonus_ + special_bonus_;
	return bonus;
}

const std::vector<StatType::Type>& SpellList::stats() const {
	static const std::vector<StatType::Type> empty_stats{};
	return category_ != nullptr ? category_->stats() : empty_stats;
}

} // namespace rm::game::character