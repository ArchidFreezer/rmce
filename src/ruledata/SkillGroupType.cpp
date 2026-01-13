#include "SkillGroupType.h"

const std::string SkillGroupType::toString(Type group) {
	using enum Type;

	switch (group) {
	case kArmour: return "Armour";
	case kArtistic: return "Artistic";
	case kAthletic: return "Athletic";
	case kAwareness: return "Awareness";
	case kBodyDevelopment: return "Body Development";
	case kCombatManoeuvres: return "Combat Manoeuvres";
	case kCommunications: return "Communications";
	case kCrafts: return "Crafts";
	case kDirectedSpells: return "Directed Spells";
	case kInfluence: return "Influence";
	case kLore: return "Lore";
	case kMartialArts: return "Martial Arts";
	case kOutdoor: return "Outdoor";
	case kPowerAwareness: return "Power Awareness";
	case kPowerManipulation: return "Power Manipulation";
	case kPowerPointDevelopment: return "Power Point Development";
	case kScienceAnalytic: return "Science/Analytic";
	case kSelfControl: return "Self Control";
	case kSpecialAttacks: return "Special Attacks";
	case kSpecialDefences: return "Special Defences";
	case kSpells: return "Spells";
	case kSubterfuge: return "Subterfuge";
	case kTechnicalTrade: return "Technical/Trade";
	case kUrban: return "Urban";
	case kWeapon: return "Weapon";
	default: return "";
	}
}

const std::optional<SkillGroupType::Type> SkillGroupType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcaseconcat(sv);
	if (val == "armour") return kArmour;
	if (val == "artistic") return kArtistic;
	if (val == "athletic") return kAthletic;
	if (val == "awareness") return kAwareness;
	if (val == "bodydevelopment") return kBodyDevelopment;
	if (val == "combatmanoeuvres") return kCombatManoeuvres;
	if (val == "communications") return kCommunications;
	if (val == "crafts") return kCrafts;
	if (val == "directedspells") return kDirectedSpells;
	if (val == "influence") return kInfluence;
	if (val == "lore") return kLore;
	if (val == "martialarts") return kMartialArts;
	if (val == "outdoor") return kOutdoor;
	if (val == "powerawareness") return kPowerAwareness;
	if (val == "powermanipulation") return kPowerManipulation;
	if (val == "powerpointdevelopment") return kPowerPointDevelopment;
	if (val == "scienceanalytic") return kScienceAnalytic;
	if (val == "selfcontrol") return kSelfControl;
	if (val == "specialattacks") return kSpecialAttacks;
	if (val == "specialdefences") return kSpecialDefences;
	if (val == "spells") return kSpells;
	if (val == "subterfuge") return kSubterfuge;
	if (val == "technicaltrade") return kTechnicalTrade;
	if (val == "urban") return kUrban;
	if (val == "weapon") return kWeapon;

	return {};
}