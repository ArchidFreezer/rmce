#include "SkillProgressionTypeData.h"

SkillProgressionTypeData::SkillProgressionTypeData(std::string_view id) : GameRuleData(id) {}

SkillProgressionTypeData::SkillProgressionTypeData(std::string_view id, std::string_view name, float zero, float ten, float twenty, float thirty, float remaining) :
	GameRuleData(id),
	name_{ name },
	zero_{ zero },
	ten_{ ten },
	twenty_{ twenty },
	thirty_{ thirty },
	remaining_{ remaining } {}

int SkillProgressionTypeData::getBonus(int ranks) const {
	return getBonus(ranks, zero_, ten_, twenty_, thirty_, remaining_);
}

int SkillProgressionTypeData::getBonus(int ranks, float zero, float ten, float twenty, float thirty, float remaining) const {
	// Short circuit if we have no ranks
	if (ranks == 0) return zero;

	float bonus{};
	if (ranks > 0) bonus = std::min(ranks, 10) * ten;
	if (ranks > 10) bonus += std::min((ranks - 10), 10) * twenty;
	if (ranks > 20) bonus += std::min((ranks - 20), 10) * thirty;
	if (ranks > 30) bonus += (ranks - 30) * remaining;
	return int(bonus); // There shouldn't be an overflow here as the bonus should never get into the thousands
}