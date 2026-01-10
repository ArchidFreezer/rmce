#include "SkillGroup.h"

const std::string toString(SkillGroup group) {
	using enum SkillGroup;

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

const std::optional<SkillGroup> fromString(std::string_view sv) {
	using enum SkillGroup;

	const std::string& val = lcase(sv);
	if (val == "armour") return kArmour;
	if (val == "artistic") return kArtistic;
	if (val == "athletic") return kAthletic;
	if (val == "awareness") return kAwareness;
	if (val == "bodydevelopment") return kBodyDevelopment;
	if (val == "body_development") return kBodyDevelopment;
	if (val == "body_development") return kBodyDevelopment;
	if (val == "combatmanoeuvres") return kCombatManoeuvres;
	if (val == "combat_manoeuvres") return kCombatManoeuvres;
	if (val == "combat manoeuvres") return kCombatManoeuvres;
	if (val == "communications") return kCommunications;
	if (val == "crafts") return kCrafts;
	if (val == "directedspells") return kDirectedSpells;
	if (val == "directed_spells") return kDirectedSpells;
	if (val == "directed spells") return kDirectedSpells;
	if (val == "influence") return kInfluence;
	if (val == "lore") return kLore;
	if (val == "martialarts") return kMartialArts;
	if (val == "martial_arts") return kMartialArts;
	if (val == "martial arts") return kMartialArts;
	if (val == "outdoor") return kOutdoor;
	if (val == "powerawareness") return kPowerAwareness;
	if (val == "power_awareness") return kPowerAwareness;
	if (val == "power awareness") return kPowerAwareness;
	if (val == "powermanipulation") return kPowerManipulation;
	if (val == "power_manipulation") return kPowerManipulation;
	if (val == "power manipulation") return kPowerManipulation;
	if (val == "powerpointdevelopment") return kPowerPointDevelopment;
	if (val == "power_point_development") return kPowerPointDevelopment;
	if (val == "power point development") return kPowerPointDevelopment;
	if (val == "scienceanalytic") return kScienceAnalytic;
	if (val == "science_analytic") return kScienceAnalytic;
	if (val == "science analytic") return kScienceAnalytic;
	if (val == "science/analytic") return kScienceAnalytic;
	if (val == "selfcontrol") return kSelfControl;
	if (val == "self_control") return kSelfControl;
	if (val == "self control") return kSelfControl;
	if (val == "specialattacks") return kSpecialAttacks;
	if (val == "special_attacks") return kSpecialAttacks;
	if (val == "special attacks") return kSpecialAttacks;
	if (val == "specialdefences") return kSpecialDefences;
	if (val == "special_defences") return kSpecialDefences;
	if (val == "special defences") return kSpecialDefences;
	if (val == "spells") return kSpells;
	if (val == "subterfuge") return kSubterfuge;
	if (val == "technicaltrade") return kTechnicalTrade;
	if (val == "technical_trade") return kTechnicalTrade;
	if (val == "technical trade") return kTechnicalTrade;
	if (val == "technical/trade") return kTechnicalTrade;
	if (val == "urban") return kUrban;
	if (val == "weapon") return kWeapon;

	return {};
}