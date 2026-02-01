#include <ProfessionData.h>

int ProfessionData::addStat(StatType::Type stat) {
	if (stats_.size() < 4) {
		stats_.push_back(stat);
	} else {
		throw StatType::TooManyStatsException("You attempted to add more than 4 stat bonuses to a skill.");
	}

	return stats_.size();
}

bool ProfessionData::removeStat(StatType::Type stat) {
	std::vector<StatType::Type>::iterator it;
	it = std::find(stats_.begin(), stats_.end(), stat);
	if (it != stats_.end()) {
		stats_.erase(it);
		return true;
	}
	return false;
}
