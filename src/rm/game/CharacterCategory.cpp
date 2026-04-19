#include <CharacterCategory.h>

namespace rm::game::character {

int Category::rankBonus() const {
	if (category_data_ == nullptr || progression_type_ == nullptr) {
		return 0;
	}

	return progression_type_->bonus(ranks_) + profession_bonus_ + special_bonus_;
}

}