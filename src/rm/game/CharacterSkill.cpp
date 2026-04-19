#include <CharacterSkill.h>

int CharacterSkill::bonus() const {
	if (skill_data_ == nullptr || progression_type_ == nullptr) {
		return 0;
	}
	int category_bonus = category_ != nullptr ? category_->rankBonus() : 0;
	return progression_type_->bonus(ranks_) + profession_bonus_ + special_bonus_ + category_bonus;
}