#include "SkillProgressionData.h"

SkillProgressionData::SkillProgressionData(SkillProgressionType type) : type_{ type }, id_{toString(type)} {}

int SkillProgressionData::getBonus(int ranks) {
	using enum SkillProgressionType;

	switch (type_) {
	case kCombined:
		return getBonus(ranks, -30.0f, 5.0f, 3.0f, 1.5f, 0.5f);
	case kLimited:
		return getBonus(ranks, 0.0f, 1.0f, 1.0f, 0.5f, 0.0f);
	case kNone:
		return 0;
	case kSpecial:
		return getBonus(ranks, 0.0f, 6.0f, 5.0f, 4.0f, 3.0f);
	case kStandard:
		return getBonus(ranks, -15.0f, 3.0f, 2.0f, 1.0f, 0.5f);
	default: return 0;
	}
}

int SkillProgressionData::getBonus(int ranks, float zero, float ten, float twenty, float thirty, float remaining) {
	// Short circuit if we have no ranks
	if (ranks == 0) return zero;

	float bonus{};
	if (ranks > 0) bonus = std::min(ranks, 10) * ten;
	if (ranks > 10) bonus += std::min((ranks - 10), 10) * twenty;
	if (ranks > 20) bonus += std::min((ranks - 20), 10) * thirty;
	if (ranks > 30) bonus += (ranks - 30) * remaining;
	return int(bonus); // There shouldn't be an overflow here as the bonus should never get into the thousands
}