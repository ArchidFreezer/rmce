#include <CharacterSkill.h>

namespace rm::game::character {

int Skill::bonus() const {
	if (skill_data_ == nullptr || progression_type_ == nullptr) {
		return 0;
	}
	int category_bonus = category_ != nullptr ? category_->rankBonus() : 0;
	return progression_type_->bonus(ranks_) + profession_bonus_ + special_bonus_ + stat_bonus_ + category_bonus;
}

const std::vector<StatType::Type>& Skill::stats() const {
	static const std::vector<StatType::Type> empty_stats{};
	return category_ != nullptr ? category_->stats() : empty_stats;
}

} // namespace rm::game::character