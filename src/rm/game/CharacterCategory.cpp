#include <CharacterCategory.h>

namespace rm::game::character {

int Category::rankBonus() const {
	if (category_data_ == nullptr || progression_type_ == nullptr) {
		return 0;
	}

	return progression_type_->bonus(ranks_) + profession_bonus_ + special_bonus_;
}

const std::vector<StatType::Type>& Category::stats() const {
	return stats_;
}

} // namespace rm::game::character