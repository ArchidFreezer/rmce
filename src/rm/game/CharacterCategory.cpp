#include <CharacterCategory.h>

namespace rm::game::character {

int Category::bonus() const {
	int bonus{0};
	bonus += progression_type_ != nullptr ? progression_type_->bonus(ranks_) : 0;
	bonus += profession_bonus_ + special_bonus_;
	return bonus;
}

const std::vector<StatType::Type>& Category::stats() const {
	if (category_data_ != nullptr && !category_data_->useRealmStats()) {
		return category_data_->stats();
	} else {
		return stats_;
	}
}

} // namespace rm::game::character