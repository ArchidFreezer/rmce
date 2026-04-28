#include <CharacterLanguage.h>

namespace rm::game::character {

int Language::sharedBonus() const {
	int bonus{0};
	bonus += profession_bonus_ + special_bonus_;
	bonus += category_ != nullptr ? category_->bonus() : 0;
	return bonus;
}

int Language::somaticBonus() const {
	int bonus{0};
	bonus += progression_type_ != nullptr ? progression_type_->bonus(somaticRanks()) : 0;
	bonus += sharedBonus();
	return bonus;
}

int Language::spokenBonus() const {
	int bonus{0};
	bonus += progression_type_ != nullptr ? progression_type_->bonus(spokenRanks()) : 0;
	bonus += sharedBonus();
	return bonus;
}

int Language::writtenBonus() const {
	int bonus{0};
	bonus += progression_type_ != nullptr ? progression_type_->bonus(writtenRanks()) : 0;
	bonus += sharedBonus();
	return bonus;
}

const std::vector<StatType::Type>& Language::stats() const {
	static const std::vector<StatType::Type> empty_stats{};
	return category_ != nullptr ? category_->stats() : empty_stats;
}

} // namespace rm::game::character	