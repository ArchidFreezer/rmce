#include <SkillCategoryData.h>
	
int SkillCategoryData::addStat(StatType::Type stat) {
	if (useRealmStats()) throw UsingCharacterRealmStatsException("Cannot add category stat when also using character realm stats");
	if (stats_.size() < 3) {
		stats_.push_back(stat);
	} else {
		throw StatType::TooManyStatsException("You attempted to add more than 3 stat bonuses to a skill category.");
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

void SkillCategoryData::setSkillProgressions(const SkillProgressionTypeData& skill_progression, const SkillProgressionTypeData& skill_category_progression) {
	// Check for invalid combinations first
	if (skill_category_progression.name() != "Category Standard" && skill_category_progression.name() != "None") {
		throw InvalidSkillProgression("Category progression may only be Standard or None.");
	}
	if (skill_category_progression.name() == "Category Standard" && skill_progression.name() != "Standard") {
		throw InvalidSkillProgression("Category progression may only be Standard if the skill progression is also set to Standard.");
	}
	// Combination is valid
	skill_progression_ = &skill_progression;
	skill_category_progression_ = &skill_category_progression;
}

void SkillCategoryData::setUseRealmStats(bool use_realm_stats) {
	use_realm_stats_ = use_realm_stats;

	// If we are using the character realm stats then any set on the category should be removed
	if (use_realm_stats_) clearStats();
}