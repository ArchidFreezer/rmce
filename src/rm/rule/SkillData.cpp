#include <SkillData.h>

namespace rm {

	int SkillData::addStat(StatType::Type stat) {
		if (category_->useRealmStats()) throw UsingCharacterRealmStatsException("Cannot add stat when also using realm stats");
		if (stats_.size() < 3) {
			stats_.push_back(stat);
		} else {
			throw StatType::TooManyStatsException("You attempted to add more than 3 stat bonuses to a skill.");
		}

		return stats_.size();
	}

	bool SkillData::removeStat(StatType::Type stat) {
		std::vector<StatType::Type>::iterator it;
		it = std::find(stats_.begin(), stats_.end(), stat);
		if (it != stats_.end()) {
			stats_.erase(it);
			return true;
		}
		return false;
	}

} // namespace rm