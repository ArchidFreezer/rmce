#include <ProfessionData.h>

namespace rm {

	int ProfessionData::addStat(rule::enums::StatType::Type stat) {
		if (stats_.size() < 4) {
			stats_.push_back(stat);
		} else {
			throw rule::enums::StatType::TooManyStatsException("You attempted to add more than 4 stat bonuses to a skill.");
		}

		return stats_.size();
	}

	bool ProfessionData::removeStat(rule::enums::StatType::Type stat) {
		std::vector<rule::enums::StatType::Type>::iterator it;
		it = std::find(stats_.begin(), stats_.end(), stat);
		if (it != stats_.end()) {
			stats_.erase(it);
			return true;
		}
		return false;
	}

} // namespace rm