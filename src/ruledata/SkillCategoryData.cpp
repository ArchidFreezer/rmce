#include "SkillCategoryData.h"

int SkillCategoryData::addStat(StatType::Type stat) {
	if (stats_.size() < 3) {
		stats_.push_back(stat);
	} else {
		throw TooManyStatsException("You attempted to add more than 3 stat bonuses to a skill category.");
	}

	return stats_.size();
}

bool SkillCategoryData::removeStat(StatType::Type stat) {
	std::vector<StatType::Type>::iterator it;
	it = std::find(stats_.begin(), stats_.end(), stat);
	if (it != stats_.end()) {
		stats_.erase(it);
		return true;
	}
	return false;
}

void SkillCategoryData::setSkillProgressions(SkillProgressionType::Type skillProgression, SkillProgressionType::Type categoryProgression) {
	// Check for invlaid combinations first
	if (categoryProgression != SkillProgressionType::kStandard && categoryProgression != SkillProgressionType::kNone) {
		throw InvalidSkillProgression("Category progression may only be kStandard or kNone.");
	}
	if (categoryProgression == SkillProgressionType::kStandard && skillProgression != SkillProgressionType::kStandard) {
		throw InvalidSkillProgression("Category progression may only be kStandard if the skill progression is also set to kStandard.");
	}
	// Combination is valid
	skillProgression_ = skillProgression;
	skillCategoryProgression_ = categoryProgression;
}
