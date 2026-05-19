#include <format>
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
	max_power_points_ = std::max(0, skillBonus(*power_point_skill_));
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

std::string chaoticSpellListCost(const SkillCategoryData& category, int ranks) {
	std::string cost;
	if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS") {
		cost = "";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS") {
		if (ranks < 4) {
			cost = "16:16";
		} else if (ranks == 4) {
			cost = "16";
		} else if (ranks < 10) {
			cost = "18";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS") {
		if (ranks < 9) {
			cost = "11:11";
		} else if (ranks == 9) {
			cost = "11";
		} else if (ranks < 15) {
			cost = "12";
		} else if (ranks < 20) {
			cost = "18";
		} else {
			cost = "25";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS") {
		if (ranks < 5) {
			cost = "60";
		} else if (ranks < 10) {
			cost = "80";
		} else if (ranks < 15) {
			cost = "100";
		} else if (ranks < 20) {
			cost = "120";
		} else {
			cost = "140";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS") {
		if (ranks < 5) {
			cost = "12";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "12:12";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS") {
		cost = "";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS") {
		cost = "";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS") {
		cost = "6:6:6";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "6:6:6";
	}
	return cost;
}

std::string semiArcaneSpellListCost(const SkillCategoryData& category, int ranks) {
	std::string cost;
	if (category.name() == "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS") {
		cost = "";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS") {
		if (ranks < 4) {
			cost = "16:16";
		} else if (ranks == 4) {
			cost = "16";
		} else if (ranks < 10) {
			cost = "18";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS") {
		if (ranks < 9) {
			cost = "11:11";
		} else if (ranks == 9) {
			cost = "11";
		} else if (ranks < 15) {
			cost = "12";
		} else if (ranks < 20) {
			cost = "18";
		} else {
			cost = "25";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS") {
		if (ranks < 5) {
			cost = "60";
		} else if (ranks < 10) {
			cost = "80";
		} else if (ranks < 15) {
			cost = "100";
		} else if (ranks < 20) {
			cost = "120";
		} else {
			cost = "140";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS") {
		if (ranks < 5) {
			cost = "12";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "12:12";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS") {
		cost = "";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS") {
		cost = "";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS") {
		if (ranks < 3) {
			cost = "6:6:6";
		} else if (ranks == 3) {
			cost = "6:6:7";
		} else if (ranks == 4) {
			cost = "6:7:7";
		} else if (ranks == 18) {
			cost = "7:7:8";
		} else if (ranks < 18) {
			cost = "7:7:7";
		} else if (ranks == 18) {
			cost = "7:7:8";
		} else if (ranks == 19) {
			cost = "7:8:8";
		} else {
			cost = "8:8:8";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "6:6:6";
	}
	return cost;
}

std::string pureArcaneSpellListCost(const SkillCategoryData& category, int ranks) {
	std::string cost;
	if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS") {
		cost = "";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS") {
		if (ranks < 18) {
			cost = "4:4:4";
		} else if (ranks == 18) {
			cost = "4:4:8";
		} else if (ranks == 19) {
			cost = "4:8:8";
		} else {
			cost = "8:8";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS") {
		if (ranks < 18) {
			cost = "4:4:4";
		} else if (ranks == 18) {
			cost = "4:4:6";
		} else if (ranks == 19) {
			cost = "4:6:6";
		} else {
			cost = "6:6:6";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS") {
		if (ranks < 4) {
			cost = "10:10";
		} else if (ranks == 4) {
			cost = "10";
		} else if (ranks < 10) {
			cost = "12";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS") {
		if (ranks < 4) {
			cost = "6:6";
		} else if (ranks == 4) {
			cost = "6:8";
		} else if (ranks < 9) {
			cost = "8:8";
		} else if (ranks == 9) {
			cost = "8:10";
		} else if (ranks < 14) {
			cost = "10:10";
		} else if (ranks == 14) {
			cost = "10";
		} else if (ranks < 20) {
			cost = "12";
		} else {
			cost = "25";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS") {
		if (ranks < 4) {
			cost = "5:5";
		} else if (ranks == 4) {
			cost = "5:6";
		} else if (ranks < 9) {
			cost = "6:6";
		} else if (ranks == 9) {
			cost = "6:8";
		} else if (ranks < 14) {
			cost = "8:8";
		} else if (ranks == 14) {
			cost = "8:10";
		} else if (ranks < 19) {
			cost = "10:10";
		} else if (ranks == 19) {
			cost = "10";
		} else {
			cost = "12";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "8:8";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS") {
		cost = "";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS") {
		cost = "";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS") {
		if (ranks < 4) {
			cost = "12:12";
		} else if (ranks < 5) {
			cost = "12:16";
		} else if (ranks < 9) {
			cost = "16:16";
		} else if (ranks == 9) {
			cost = "16";
		} else if (ranks < 15) {
			cost = "18";
		} else if (ranks < 20) {
			cost = "25";
		} else {
			cost = "40";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS") {
		if (ranks < 18) {
			cost = "3:3:3";
		} else if (ranks == 18) {
			cost = "3:3:4";
		} else if (ranks == 19) {
			cost = "3:4:4";
		} else {
			cost = "4:4:4";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "4:4:4";
	}
	return cost;
}

std::string noneSpellListCost(const SkillCategoryData& category, int ranks, int baseCost) {
	std::string cost;
	if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "8:8:8";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "16:16";
	} else {
		if (ranks < 5) {
			cost = std::format("{}", baseCost);
		} else if (ranks < 10) {
			cost = std::format("{}", baseCost * 2);
		} else if (ranks < 15) {
			cost = std::format("{}", baseCost * 3);
		} else if (ranks < 20) {
			cost = std::format("{}", baseCost * 4);
		} else {
			cost = std::format("{}", baseCost * 5);
		}
	}

	return cost;
}

std::string semiSpellListCost(const SkillCategoryData& category, int ranks) {
	std::string cost;
	if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS") {
		if (ranks < 5) {
			cost = "40";
		} else if (ranks < 10) {
			cost = "60";
		} else if (ranks < 15) {
			cost = "80";
		} else if (ranks < 20) {
			cost = "100";
		} else {
			cost = "120";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS") {
		if (ranks < 5) {
			cost = "18";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS") {
		if (ranks < 5) {
			cost = "12";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS") {
		if (ranks < 5) {
			cost = "80";
		} else if (ranks < 10) {
			cost = "100";
		} else if (ranks < 15) {
			cost = "120";
		} else if (ranks < 20) {
			cost = "140";
		} else {
			cost = "160";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS") {
		if (ranks < 5) {
			cost = "45";
		} else if (ranks < 10) {
			cost = "60";
		} else if (ranks < 15) {
			cost = "80";
		} else if (ranks < 20) {
			cost = "100";
		} else {
			cost = "120";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS") {
		if (ranks < 5) {
			cost = "30";
		} else if (ranks < 10) {
			cost = "60";
		} else if (ranks < 15) {
			cost = "80";
		} else if (ranks < 20) {
			cost = "100";
		} else {
			cost = "120";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "12:12";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS") {
		if (ranks < 4) {
			cost = "10:10";
		} else if (ranks == 4) {
			cost = "10";
		} else if (ranks < 10) {
			cost = "12";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS") {
		if (ranks < 9) {
			cost = "8:8";
		} else if (ranks == 9) {
			cost = "8";
		} else if (ranks < 15) {
			cost = "12";
		} else if (ranks < 20) {
			cost = "18";
		} else {
			cost = "25";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS") {
		cost = "6:6:6";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "6:6:6";
	}
	return cost;
}

std::string hybridSpellListCost(const SkillCategoryData& category, int ranks) {
	std::string cost;
	if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS") {
		if (ranks < 5) {
			cost = "12";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS") {
		if (ranks < 4) {
			cost = "8:8";
		} else if (ranks == 4) {
			cost = "8:10";
		} else if (ranks < 9) {
			cost = "10:10";
		} else if (ranks == 9) {
			cost = "10";
		} else if (ranks < 15) {
			cost = "12";
		} else if (ranks < 20) {
			cost = "24";
		} else {
			cost = "40";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS") {
		if (ranks < 4) {
			cost = "5:5";
		} else if (ranks == 4) {
			cost = "5:6";
		} else if (ranks < 9) {
			cost = "6:6";
		} else if (ranks == 9) {
			cost = "6:8";
		} else if (ranks < 14) {
			cost = "8:8";
		} else if (ranks == 14) {
			cost = "8:10";
		} else if (ranks < 19) {
			cost = "10:10";
		} else if (ranks == 19) {
			cost = "10";
		} else {
			cost = "12";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS") {
		if (ranks < 5) {
			cost = "60";
		} else if (ranks < 10) {
			cost = "80";
		} else if (ranks < 15) {
			cost = "100";
		} else if (ranks < 20) {
			cost = "120";
		} else {
			cost = "140";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS") {
		if (ranks < 5) {
			cost = "12";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "8:8";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS") {
		if (ranks < 3) {
			cost = "4:4:4";
		} else if (ranks == 3) {
			cost = "4:4:6";
		} else if (ranks == 4) {
			cost = "4:6:6";
		} else if (ranks < 8) {
			cost = "6:6:6";
		} else if (ranks == 8) {
			cost = "6:6:8";
		} else if (ranks == 9) {
			cost = "6:8:8";
		} else if (ranks < 14) {
			cost = "8:8";
		} else if (ranks == 14) {
			cost = "8:10";
		} else if (ranks < 19) {
			cost = "10:10";
		} else if (ranks == 19) {
			cost = "10";
		} else {
			cost = "25";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS") {
		if (ranks < 8) {
			cost = "4:4:4";
		} else if (ranks == 8) {
			cost = "4:4:6";
		} else if (ranks == 9) {
			cost = "4:6:6";
		} else if (ranks < 13) {
			cost = "6:6:6";
		} else if (ranks == 13) {
			cost = "6:6:8";
		} else if (ranks == 14) {
			cost = "6:8";
		} else if (ranks < 19) {
			cost = "8:8";
		} else if (ranks == 19) {
			cost = "8";
		} else {
			cost = "12";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS") {
		if (ranks < 4) {
			cost = "10:10";
		} else if (ranks == 4) {
			cost = "10";
		} else if (ranks < 10) {
			cost = "12";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS") {
		cost = "3:3:3";
	} else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "4:4:4";
	}
	return cost;
}

std::string pureSpellListCost(const SkillCategoryData& category, int ranks) {
	std::string cost;
	if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_BASE_LISTS") {
		if (ranks < 5) {
			cost = "25";
		} else if (ranks < 10) {
			cost = "40";
		} else if (ranks < 15) {
			cost = "60";
		} else if (ranks < 20) {
			cost = "80";
		} else {
			cost = "100";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_CLOSED_LISTS") {
		if (ranks < 4) {
			cost = "10:10";
		} else if (ranks == 4) {
			cost = "10";
		} else if (ranks < 10) {
			cost = "12";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_ARCANE_OPEN_LISTS") {
		if (ranks < 4) {
			cost = "6:6";
		} else if (ranks == 4) {
			cost = "6:8";
		} else if (ranks < 9) {
			cost = "8:8";
		} else if (ranks == 9) {
			cost = "8:10";
		} else if (ranks < 14) {
			cost = "10:10";
		} else if (ranks == 14) {
			cost = "10";
		} else if (ranks < 20) {
			cost = "12";
		} else {
			cost = "25";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_BASE_LISTS") {
		if (ranks < 5) {
			cost = "50";
		} else if (ranks < 10) {
			cost = "70";
		} else if (ranks < 15) {
			cost = "90";
		} else if (ranks < 20) {
			cost = "110";
		} else {
			cost = "130";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_CLOSED_LISTS") {
		if (ranks < 5) {
			cost = "20";
		} else if (ranks < 10) {
			cost = "25";
		} else if (ranks < 15) {
			cost = "40";
		} else if (ranks < 20) {
			cost = "60";
		} else {
			cost = "80";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_OPEN_LISTS") {
		if (ranks < 4) {
			cost = "10:10";
		} else if (ranks == 4) {
			cost = "10";
		} else if (ranks < 10) {
			cost = "12";
		} else if (ranks < 15) {
			cost = "25";
		} else if (ranks < 20) {
			cost = "40";
		} else {
			cost = "60";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OTHER_REALM_TRAINING_PACKAGE") {
		cost = "8:8";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_CLOSED_LISTS") {
		if (ranks < 18) {
			cost = "4:4:4";
		} else if (ranks == 18) {
			cost = "4:4:8";
		} else if (ranks == 19) {
			cost = "4:8:8";
		} else {
			cost = "8:8";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OPEN_LISTS") {
		if (ranks < 18) {
			cost = "4:4:4";
		} else if (ranks == 18) {
			cost = "4:4:6";
		} else if (ranks == 19) {
			cost = "4:6:6";
		} else {
			cost = "6:6:6";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OTHER_BASE_LISTS") {
		if (ranks < 4) {
			cost = "8:8";
		} else if (ranks < 5) {
			cost = "8:10";
		} else if (ranks < 9) {
			cost = "10:10";
		} else if (ranks == 9) {
			cost = "10";
		} else if (ranks < 15) {
			cost = "12";
		} else if (ranks < 20) {
			cost = "25";
		} else {
			cost = "40";
		}
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_OWN_BASE_LISTS") {
		cost = "3:3:3";
	} 
	else if (category.id() == "SKILLCATEGORY_SPELLS_OWN_REALM_TRAINING_PACKAGE") {
		cost = "4:4:4";
	}
	return cost;
}

std::map<const SpellListData*, std::string> spellListsRankCosts(const Character& character) {
	std::map<const SpellListData*, std::string> costs;
	if (!character.profession()) {
		return costs;
	}

	for (auto& [spell_list_data, spell_list] : character.spellLists()) {
		switch (character.profession()->spellUserType()) {
		case SpellUserType::Type::kPure: {
			costs[spell_list_data] = pureSpellListCost(spell_list.category().categoryData(), spell_list.ranks());
			break;
		}
		case SpellUserType::Type::kHybrid: {
			costs[spell_list_data] = hybridSpellListCost(spell_list.category().categoryData(), spell_list.ranks());
			break;
		}
		case SpellUserType::Type::kSemi: {
			costs[spell_list_data] = semiSpellListCost(spell_list.category().categoryData(), spell_list.ranks());
			break;
		}
		case SpellUserType::Type::kNone: {
			const SkillCategoryData& category = spell_list.category().categoryData();
			std::optional<int> cost = character.profession()->skillCategoryDevelopmentCost(category).first();
			costs[spell_list_data] = noneSpellListCost(category, spell_list.ranks(), cost.value_or(500));
			break;
		}
		case SpellUserType::Type::kChaotic: {
			costs[spell_list_data] = chaoticSpellListCost(spell_list.category().categoryData(), spell_list.ranks());
			break;
		}
		default: {
			break;
		}
		}
	}
	return costs;
}

} // namespace rm::game::character