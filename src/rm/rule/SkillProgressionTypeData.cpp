#include <numeric>
#include <SkillProgressionTypeData.h>

namespace rm::rule {

int SkillProgressionTypeData::bonus(int ranks) const {
	return bonus(ranks, zero_, ten_, twenty_, thirty_, remaining_);
}

int SkillProgressionTypeData::bonus(int ranks, float zero, float ten, float twenty, float thirty, float remaining) const {
	// Short circuit if we have no ranks
	if (ranks == 0)
		return zero;

	float bonus{};
	if (ranks > 0)
		bonus = std::min(ranks, 10) * ten;
	if (ranks > 10)
		bonus += std::min((ranks - 10), 10) * twenty;
	if (ranks > 20)
		bonus += std::min((ranks - 20), 10) * thirty;
	if (ranks > 30)
		bonus += (ranks - 30) * remaining;
	return int(bonus); // There shouldn't be an overflow here as the bonus should never get into the thousands
}

void computeCombinedProgression(const std::set<const SkillProgressionTypeData*>& progression_types, SkillProgressionTypeData& combined_progression) {
	int num_progressions = progression_types.size();
	// We can compute the combined progression by averaging the bonuses provided by each progression type for the given number of ranks.
	combined_progression.setZero(std::accumulate(progression_types.begin(), progression_types.end(), 0.f, [](float sum, const SkillProgressionTypeData* progression_type) { return sum + progression_type->zero(); }) / num_progressions);
	combined_progression.setTen(std::accumulate(progression_types.begin(), progression_types.end(), 0.f, [](float sum, const SkillProgressionTypeData* progression_type) { return sum + progression_type->ten(); }) / num_progressions);
	combined_progression.setTwenty(std::accumulate(progression_types.begin(), progression_types.end(), 0.f, [](float sum, const SkillProgressionTypeData* progression_type) { return sum + progression_type->twenty(); }) / num_progressions);
	combined_progression.setThirty(std::accumulate(progression_types.begin(), progression_types.end(), 0.f, [](float sum, const SkillProgressionTypeData* progression_type) { return sum + progression_type->thirty(); }) / num_progressions);
	combined_progression.setRemaining(std::accumulate(progression_types.begin(), progression_types.end(), 0.f, [](float sum, const SkillProgressionTypeData* progression_type) { return sum + progression_type->remaining(); }) / num_progressions);
}

} // namespace rm::rule