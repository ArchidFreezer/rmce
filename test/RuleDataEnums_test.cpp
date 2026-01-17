#include <gtest/gtest.h>

#include <ArmourType.h>
#include <CreatureSizeType.h>
#include <CriticalTableType.h>
#include <CriticalType.h>
#include <RealmType.h>
#include <ResistanceType.h>
#include <SkillActionType.h>
#include <SkillDevelopmentType.h>
#include <SkillGroupType.h>
#include <SpellListType.h>
#include <StatLossType.h>
#include <StatType.h>

namespace {
	TEST(RuleDataEnums, StatType) {
		EXPECT_EQ(StatType::fromString(toString(StatType::kAgility)), StatType::kAgility);
		EXPECT_EQ(StatType::fromString(toString(StatType::kConstitution)), StatType::kConstitution);
		EXPECT_EQ(StatType::fromString(toString(StatType::kEmpathy)), StatType::kEmpathy);
		EXPECT_EQ(StatType::fromString(toString(StatType::kIntuition)), StatType::kIntuition);
		EXPECT_EQ(StatType::fromString(toString(StatType::kMemory)), StatType::kMemory);
		EXPECT_EQ(StatType::fromString(toString(StatType::kPresence)), StatType::kPresence);
		EXPECT_EQ(StatType::fromString(toString(StatType::kQuickness)), StatType::kQuickness);
		EXPECT_EQ(StatType::fromString(toString(StatType::kReasoning)), StatType::kReasoning);
		EXPECT_EQ(StatType::fromString(toString(StatType::kSelfDiscipline)), StatType::kSelfDiscipline);
		EXPECT_EQ(StatType::fromString(toString(StatType::kStrength)), StatType::kStrength);
	}

	TEST(RuleDataEnums, StatLossType) {
		EXPECT_EQ(StatLossType::fromString(toString(StatLossType::kSlow)), StatLossType::kSlow);
		EXPECT_EQ(StatLossType::fromString(toString(StatLossType::kNormal)), StatLossType::kNormal);
		EXPECT_EQ(StatLossType::fromString(toString(StatLossType::kFast)), StatLossType::kFast);
		EXPECT_EQ(StatLossType::fromString(toString(StatLossType::kVeryFast)), StatLossType::kVeryFast);
		EXPECT_EQ(StatLossType::fromString(toString(StatLossType::kExtreme)), StatLossType::kExtreme);
	}

	TEST(RuleDataEnums, SpellListType) {
		EXPECT_EQ(SpellListType::fromString(toString(SpellListType::kBase)), SpellListType::kBase);
		EXPECT_EQ(SpellListType::fromString(toString(SpellListType::kClosed)), SpellListType::kClosed);
		EXPECT_EQ(SpellListType::fromString(toString(SpellListType::kOpen)), SpellListType::kOpen);
		EXPECT_EQ(SpellListType::fromString(toString(SpellListType::kTrainingPackage)), SpellListType::kTrainingPackage);
		EXPECT_EQ(SpellListType::fromString(toString(SpellListType::kRacial)), SpellListType::kRacial);
	}

	TEST(RuleDataEnums, SkillGroupType) {
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kArmour)), SkillGroupType::kArmour);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kArtistic)), SkillGroupType::kArtistic);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kAthletic)), SkillGroupType::kAthletic);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kAwareness)), SkillGroupType::kAwareness);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kBodyDevelopment)), SkillGroupType::kBodyDevelopment);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kCombatManoeuvres)), SkillGroupType::kCombatManoeuvres);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kCommunications)), SkillGroupType::kCommunications);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kCrafts)), SkillGroupType::kCrafts);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kDirectedSpells)), SkillGroupType::kDirectedSpells);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kInfluence)), SkillGroupType::kInfluence);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kLore)), SkillGroupType::kLore);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kMartialArts)), SkillGroupType::kMartialArts);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kOutdoor)), SkillGroupType::kOutdoor);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kPowerAwareness)), SkillGroupType::kPowerAwareness);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kPowerManipulation)), SkillGroupType::kPowerManipulation);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kPowerPointDevelopment)), SkillGroupType::kPowerPointDevelopment);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kScienceAnalytic)), SkillGroupType::kScienceAnalytic);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kSelfControl)), SkillGroupType::kSelfControl);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kSpecialAttacks)), SkillGroupType::kSpecialAttacks);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kSpecialDefences)), SkillGroupType::kSpecialDefences);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kSpells)), SkillGroupType::kSpells);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kSubterfuge)), SkillGroupType::kSubterfuge);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kTechnicalTrade)), SkillGroupType::kTechnicalTrade);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kUrban)), SkillGroupType::kUrban);
		EXPECT_EQ(SkillGroupType::fromString(toString(SkillGroupType::kWeapon)), SkillGroupType::kWeapon);
	}

	TEST(RuleDataEnums, SkillDevelopmentType) {
		EXPECT_EQ(SkillDevelopmentType::fromString(toString(SkillDevelopmentType::kEveryman)), SkillDevelopmentType::kEveryman);
		EXPECT_EQ(SkillDevelopmentType::fromString(toString(SkillDevelopmentType::kOccupational)), SkillDevelopmentType::kOccupational);
		EXPECT_EQ(SkillDevelopmentType::fromString(toString(SkillDevelopmentType::kRestricted)), SkillDevelopmentType::kRestricted);
		EXPECT_EQ(SkillDevelopmentType::fromString(toString(SkillDevelopmentType::kStandard)), SkillDevelopmentType::kStandard);
	}

	TEST(RuleDataEnums, SkillActionType) {
		EXPECT_EQ(SkillActionType::fromString(toString(SkillActionType::kMoving)), SkillActionType::kMoving);
		EXPECT_EQ(SkillActionType::fromString(toString(SkillActionType::kOB)), SkillActionType::kOB);
		EXPECT_EQ(SkillActionType::fromString(toString(SkillActionType::kSpecial)), SkillActionType::kSpecial);
		EXPECT_EQ(SkillActionType::fromString(toString(SkillActionType::kStatic)), SkillActionType::kStatic);
	}

	TEST(RuleDataEnums, ResistanceType) {
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kArcane)), ResistanceType::kArcane);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kChanneling)), ResistanceType::kChanneling);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kChannelingEssence)), ResistanceType::kChannelingEssence);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kChannelingMentalism)), ResistanceType::kChannelingMentalism);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kCold)), ResistanceType::kCold);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kDisease)), ResistanceType::kDisease);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kEssence)), ResistanceType::kEssence);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kEssenceMentalism)), ResistanceType::kEssenceMentalism);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kFear)), ResistanceType::kFear);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kHeat)), ResistanceType::kHeat);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kMentalism)), ResistanceType::kMentalism);
		EXPECT_EQ(ResistanceType::fromString(toString(ResistanceType::kPoison)), ResistanceType::kPoison);
	}

	TEST(RuleDataEnums, RealmType) {
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kArcane)), RealmType::kArcane);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kArms)), RealmType::kArms);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kChanneling)), RealmType::kChanneling);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kChannelingEssence)), RealmType::kChannelingEssence);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kChannelingMentalism)), RealmType::kChannelingMentalism);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kEssence)), RealmType::kEssence);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kEssenceMentalism)), RealmType::kEssenceMentalism);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kMentalism)), RealmType::kMentalism);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kMundane)), RealmType::kMundane);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kNeutral)), RealmType::kNeutral);
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kSubterfuge)), RealmType::kSubterfuge);
	}

	TEST(RuleDataEnums, CreatureSizeType) {
		EXPECT_EQ(CreatureSizeType::fromString(toString(CreatureSizeType::kLarge)), CreatureSizeType::kLarge);
		EXPECT_EQ(CreatureSizeType::fromString(toString(CreatureSizeType::kMedium)), CreatureSizeType::kMedium);
		EXPECT_EQ(CreatureSizeType::fromString(toString(CreatureSizeType::kSmall)), CreatureSizeType::kSmall);
		EXPECT_EQ(CreatureSizeType::fromString(toString(CreatureSizeType::kHuge)), CreatureSizeType::kHuge);
		EXPECT_EQ(CreatureSizeType::fromString(toString(CreatureSizeType::kTiny)), CreatureSizeType::kTiny);
	}

	TEST(RuleDataEnums, CriticalTableType) {
		EXPECT_EQ(CriticalTableType::fromString(toString(CriticalTableType::kNormal)), CriticalTableType::kNormal);
		EXPECT_EQ(CriticalTableType::fromString(toString(CriticalTableType::kLargeCreaturePhysical)), CriticalTableType::kLargeCreaturePhysical);
		EXPECT_EQ(CriticalTableType::fromString(toString(CriticalTableType::kHugeCreaturePhysical)), CriticalTableType::kHugeCreaturePhysical);
		EXPECT_EQ(CriticalTableType::fromString(toString(CriticalTableType::kLargeCreatureSpell)), CriticalTableType::kLargeCreatureSpell);
		EXPECT_EQ(CriticalTableType::fromString(toString(CriticalTableType::kHugeCreatureSpell)), CriticalTableType::kHugeCreatureSpell);
	}

	TEST(RuleDataEnums, CriticalType) {
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kBrawling)), CriticalType::kBrawling);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kGrapple)), CriticalType::kGrapple);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kImpact)), CriticalType::kImpact);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kKrush)), CriticalType::kKrush);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kMartialArtsStrikes)), CriticalType::kMartialArtsStrikes);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kMartialArtsSweeps)), CriticalType::kMartialArtsSweeps);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kPuncture)), CriticalType::kPuncture);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kSlash)), CriticalType::kSlash);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kSubdual)), CriticalType::kSubdual);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kTiny)), CriticalType::kTiny);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kUnbalance)), CriticalType::kUnbalance);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kAether)), CriticalType::kAether);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kCold)), CriticalType::kCold);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kElectrical)), CriticalType::kElectrical);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kHeat)), CriticalType::kHeat);
		EXPECT_EQ(CriticalType::fromString(toString(CriticalType::kNether)), CriticalType::kNether);
	}

	TEST(RuleDataEnums, ArmourType) {
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT1)), ArmourType::kAT1);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT2)), ArmourType::kAT2);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT3)), ArmourType::kAT3);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT4)), ArmourType::kAT4);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT5)), ArmourType::kAT5);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT6)), ArmourType::kAT6);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT7)), ArmourType::kAT7);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT8)), ArmourType::kAT8);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT9)), ArmourType::kAT9);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT10)), ArmourType::kAT10);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT11)), ArmourType::kAT11);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT12)), ArmourType::kAT12);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT13)), ArmourType::kAT13);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT14)), ArmourType::kAT14);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT15)), ArmourType::kAT15);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT16)), ArmourType::kAT16);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT17)), ArmourType::kAT17);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT18)), ArmourType::kAT18);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT19)), ArmourType::kAT19);
		EXPECT_EQ(ArmourType::fromString(toString(ArmourType::kAT20)), ArmourType::kAT20);
	}

}