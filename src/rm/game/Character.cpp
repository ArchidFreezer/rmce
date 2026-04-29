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
		// Now we need to add the stat bonus as that is character specific so not included in the skill definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

int Character::categoryBonus(const SkillCategoryData& category) const {
	int bonus{-25}; // The default bonus for no ranks in a skill with standard progression is -25, so we start with that and then update it if the character has any knowledge of the category.
	auto it = categories_.find(&category);
	if (it != categories_.end()) {
		bonus = it->second.bonus();
		// Now we need to add the stat bonus as that is character specific so not included in the category definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

int Character::spellListBonus(const SpellListData& spell_list) const {
	int bonus{-30}; // The default bonus for no ranks in a spell list with limited progression is -30, so we start with that and then update it if the character has any knowledge of the spell list.
	auto it = spell_lists_.find(&spell_list);
	if (it != spell_lists_.end()) {
		bonus = it->second.bonus();
		// Now we need to add the stat bonus as that is character specific so not included in the spell list definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

int Character::languageSomaticBonus(const std::string& language_name) const {
	int bonus{0};
	auto it = languages_.find(language_name);
	if (it != languages_.end()) {
		bonus = it->second.somaticBonus();
		// Now we need to add the stat bonus as that is character specific so not included in the language definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

int Character::languageSpokenBonus(const std::string& language_name) const {
	int bonus{0};
	auto it = languages_.find(language_name);
	if (it != languages_.end()) {
		bonus = it->second.spokenBonus();
		// Now we need to add the stat bonus as that is character specific so not included in the language definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

int Character::languageWrittenBonus(const std::string& language_name) const {
	int bonus{0};
	auto it = languages_.find(language_name);
	if (it != languages_.end()) {
		bonus = it->second.writtenBonus();
		// Now we need to add the stat bonus as that is character specific so not included in the language definition.
		for (const auto& stat_type : it->second.stats()) {
			bonus += statBonus(stat_type);
		}
	}
	return bonus;
}

void Character::updateMaxHits() {
	max_hits_ = skillBonus(*body_devlopment_skill_);
}

void Character::updateMaxPowerPoints() {
	max_power_points_ = skillBonus(*power_point_skill_);
}

void Character::updateDevelopmentPoints() {
	development_points_ = 0;
	for (const auto& [stat_type, stat] : stats_) {
		if (StatType::isDevelopment(stat_type)) {
			development_points_ += stat.temporary();
		}
	}
	development_points_ = (development_points_ / 5);
}

void Character::updateAllDerivedData() {
	updateMaxHits();
	updateMaxPowerPoints();
	updateDevelopmentPoints();
	for (const auto& [stat_type, stat] : stats_) {
		updateStatDerivedData(stat_type);
	}
}

/* ------------------------------------------------------------------ */
/* Free functions                                                     */
/* ------------------------------------------------------------------ */

/* Helper function to find the category for a spell list. This is needed to apply the appropriate bonuses to the spell list based on the category it belongs to. */
const SkillCategoryData* spellListCategory(std::map<const SkillCategoryData*, std::set<const SpellListData*>> spell_list_categories, const SpellListData& spell_list) {
	for (const auto& [category, list_set] : spell_list_categories) {
		for (const SpellListData* spell_list_data : list_set) {
			if (spell_list_data == &spell_list) {
				return category;
			}
		}
	}
	return nullptr;
}


std::map<SpellListData*, std::string> spellListRankCosts(const Character& character) {
	std::map<SpellListData*, std::string> costs;
	if (!character.profession()) {
		return costs;
	}

	switch (character.profession()->spellUserType()) {
	case SpellUserType::Type::kPure: {
		//costs[character.profession()->spellList()] = "1 development point per rank";
		break;
	}
	case SpellUserType::Type::kHybrid: {
		//costs[character.profession()->spellList()] = "2 development points per rank";
		break;
	}
	case SpellUserType::Type::kSemi: {
		//costs[character.profession()->spellList()] = "3 development points per rank (1 for the spell list rank and 2 for the skill ranks)";
		break;
	}
	case SpellUserType::Type::kNone: {
		break;
	}
	case SpellUserType::Type::kChaotic: {
		//costs[character.profession()->spellList()] = "3 development points per rank";
		break;
	}
	default: {
		break;
	}
	}
	return costs;
}

} // namespace rm::game::character