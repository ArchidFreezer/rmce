#include "SkillProgressionTypeData.h"

int SkillProgressionTypeData::bonus(int ranks) const {
	return bonus(ranks, zero_, ten_, twenty_, thirty_, remaining_);
}

int SkillProgressionTypeData::bonus(int ranks, float zero, float ten, float twenty, float thirty, float remaining) const {
	// Short circuit if we have no ranks
	if (ranks == 0) return zero;

	float bonus{};
	if (ranks > 0) bonus = std::min(ranks, 10) * ten;
	if (ranks > 10) bonus += std::min((ranks - 10), 10) * twenty;
	if (ranks > 20) bonus += std::min((ranks - 20), 10) * thirty;
	if (ranks > 30) bonus += (ranks - 30) * remaining;
	return int(bonus); // There shouldn't be an overflow here as the bonus should never get into the thousands
}