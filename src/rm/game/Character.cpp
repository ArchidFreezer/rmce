#include <Character.h>

namespace rm::game::character {

using namespace rm::rule::enums;

void Character::updateStatDerivedData(StatType::Type stat_type) {
	switch (stat_type) {
	case StatType::kAgility:
		break;
	case StatType::kConstitution: {
		resistances_[ResistanceType::kPoison] = race_->resistanceBonus(ResistanceType::kPoison) + (getStat(StatType::kConstitution).bonus() * 3);
		resistances_[ResistanceType::kDisease] = race_->resistanceBonus(ResistanceType::kDisease) + (getStat(StatType::kConstitution).bonus() * 3);
		break;
	}
	case StatType::kEmpathy: {
		resistances_[ResistanceType::kEssence] = race_->resistanceBonus(ResistanceType::kEssence) + (getStat(StatType::kEmpathy).bonus() * 3);
		resistances_[ResistanceType::kEssenceMentalism] = (resistances_[ResistanceType::kEssence] + resistances_[ResistanceType::kMentalism]) / 2;
		resistances_[ResistanceType::kChannelingEssence] = (resistances_[ResistanceType::kChanneling] + resistances_[ResistanceType::kEssence]) / 2;
		break;
	}
	case StatType::kIntuition: {
		resistances_[ResistanceType::kChanneling] = race_->resistanceBonus(ResistanceType::kChanneling) + (getStat(StatType::kIntuition).bonus() * 3);
		resistances_[ResistanceType::kChannelingEssence] = (resistances_[ResistanceType::kChanneling] + resistances_[ResistanceType::kEssence]) / 2;
		resistances_[ResistanceType::kChannelingMentalism] = (resistances_[ResistanceType::kChanneling] + resistances_[ResistanceType::kMentalism]) / 2;
		break;
	}
	case StatType::kMemory:
		break;
	case StatType::kPresence: {
		resistances_[ResistanceType::kMentalism] = race_->resistanceBonus(ResistanceType::kMentalism) + (getStat(StatType::kPresence).bonus() * 3);
		resistances_[ResistanceType::kChannelingMentalism] = (resistances_[ResistanceType::kChanneling] + resistances_[ResistanceType::kMentalism]) / 2;
		resistances_[ResistanceType::kEssenceMentalism] = (resistances_[ResistanceType::kEssence] + resistances_[ResistanceType::kMentalism]) / 2;
		break;
	}
	case StatType::kQuickness:
		break;
	case StatType::kReasoning:
		break;
	case StatType::kSelfDiscipline: {
		resistances_[ResistanceType::kFear] = race_->resistanceBonus(ResistanceType::kFear) + (getStat(StatType::kSelfDiscipline).bonus() * 3);
		break;
	}
	case StatType::kStrength:
		break;
	default:
		break;
	}
}

int Character::skillBonus(const SubcategoriedSkillData& skill) const {
	int bonus{-25}; // The default bonus for no ranks in a skill with standard progression is -25, so we start with that and then update it if the character has any knowledge of the skill.

	auto it = skills_.find(&skill);
	if (it != skills_.end()) {
		bonus = it->second.bonus();
		// Now we need to add the stat bonus as that is chracter specific so not included in teh skill definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

void Character::updateMaxHits() {
	max_hits_ = skills_.at(body_devlopment_skill_).bonus();
}

void Character::updateMaxPowerPoints() {
	max_power_points_ = skills_.at(power_point_skill_).bonus();
}
void Character::updateAllDerivedData() {
	updateMaxHits();
	updateMaxPowerPoints();
	for (const auto& [stat_type, stat] : stats_) {
		updateStatDerivedData(stat_type);
	}
}

} // namespace rm::game::character