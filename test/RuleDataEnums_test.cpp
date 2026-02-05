#include <gtest/gtest.h>

#include <ArmourType.h>
#include <CreatureSizeType.h>
#include <CriticalTableType.h>
#include <CriticalType.h>
#include <EnvironmentType.h>
#include <HabitatType.h>
#include <RealmType.h>
#include <ResistanceType.h>
#include <SkillActionType.h>
#include <SkillDevelopmentType.h>
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
		EXPECT_EQ(RealmType::fromString(toString(RealmType::kEssence)), RealmType::kEssence);
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

		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT1)), ArmourType::kAT1);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT2)), ArmourType::kAT2);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT3)), ArmourType::kAT3);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT4)), ArmourType::kAT4);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT5)), ArmourType::kAT5);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT6)), ArmourType::kAT6);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT7)), ArmourType::kAT7);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT8)), ArmourType::kAT8);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT9)), ArmourType::kAT9);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT10)), ArmourType::kAT10);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT11)), ArmourType::kAT11);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT12)), ArmourType::kAT12);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT13)), ArmourType::kAT13);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT14)), ArmourType::kAT14);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT15)), ArmourType::kAT15);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT16)), ArmourType::kAT16);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT17)), ArmourType::kAT17);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT18)), ArmourType::kAT18);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT19)), ArmourType::kAT19);
		EXPECT_EQ(ArmourType::fromDescription(description(ArmourType::kAT20)), ArmourType::kAT20);
	}

	TEST(RuleDataEnums, HabitatTemperature) {
		EXPECT_EQ(HabitatType::temperature(HabitatType::toString(HabitatType::Temperature::kHot)), HabitatType::Temperature::kHot);
		EXPECT_EQ(HabitatType::temperature(HabitatType::toString(HabitatType::Temperature::kWarm)), HabitatType::Temperature::kWarm);
		EXPECT_EQ(HabitatType::temperature(HabitatType::toString(HabitatType::Temperature::kTemperate)), HabitatType::Temperature::kTemperate);
		EXPECT_EQ(HabitatType::temperature(HabitatType::toString(HabitatType::Temperature::kCool)), HabitatType::Temperature::kCool);
		EXPECT_EQ(HabitatType::temperature(HabitatType::toString(HabitatType::Temperature::kCold)), HabitatType::Temperature::kCold);

		EXPECT_NE(HabitatType::temperature(HabitatType::toString(HabitatType::Temperature::kHot)), HabitatType::Temperature::kWarm);
	}

	TEST(RuleDataEnums, HabitatPrecipitation) {
		EXPECT_EQ(HabitatType::precipitation(HabitatType::toString(HabitatType::Precipitation::kRainy)), HabitatType::Precipitation::kRainy);
		EXPECT_EQ(HabitatType::precipitation(HabitatType::toString(HabitatType::Precipitation::kHumid)), HabitatType::Precipitation::kHumid);
		EXPECT_EQ(HabitatType::precipitation(HabitatType::toString(HabitatType::Precipitation::kTemperate)), HabitatType::Precipitation::kTemperate);
		EXPECT_EQ(HabitatType::precipitation(HabitatType::toString(HabitatType::Precipitation::kDry)), HabitatType::Precipitation::kDry);
		EXPECT_EQ(HabitatType::precipitation(HabitatType::toString(HabitatType::Precipitation::kArid)), HabitatType::Precipitation::kArid);

		EXPECT_NE(HabitatType::precipitation(HabitatType::toString(HabitatType::Precipitation::kRainy)), HabitatType::Precipitation::kDry);
	}

	TEST(RuleDataEnums, EnvironmentFeature) {
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kBattlefield)), EnvironmentType::Feature::kBattlefield);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kBurial)), EnvironmentType::Feature::kBurial);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kCave)), EnvironmentType::Feature::kCave);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kCavern)), EnvironmentType::Feature::kCavern);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kDimention)), EnvironmentType::Feature::kDimention);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kEnchanted)), EnvironmentType::Feature::kEnchanted);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kHabitation)), EnvironmentType::Feature::kHabitation);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kRuins)), EnvironmentType::Feature::kRuins);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kRural)), EnvironmentType::Feature::kRural);
		EXPECT_EQ(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kVolcanic)), EnvironmentType::Feature::kVolcanic);

		EXPECT_NE(EnvironmentType::feature(EnvironmentType::toString(EnvironmentType::Feature::kVolcanic)), EnvironmentType::Feature::kEnchanted);
	}

	TEST(RuleDataEnums, EnvironmentTerrain) {
		EXPECT_EQ(EnvironmentType::terrain(EnvironmentType::toString(EnvironmentType::Terrain::kAlpine)), EnvironmentType::Terrain::kAlpine);
		EXPECT_EQ(EnvironmentType::terrain(EnvironmentType::toString(EnvironmentType::Terrain::kRough)), EnvironmentType::Terrain::kRough);
		EXPECT_EQ(EnvironmentType::terrain(EnvironmentType::toString(EnvironmentType::Terrain::kUnderground)), EnvironmentType::Terrain::kUnderground);
		EXPECT_EQ(EnvironmentType::terrain(EnvironmentType::toString(EnvironmentType::Terrain::kWaste)), EnvironmentType::Terrain::kWaste);

		EXPECT_NE(EnvironmentType::terrain(EnvironmentType::toString(EnvironmentType::Terrain::kAlpine)), EnvironmentType::Terrain::kUnderground);
	}

	TEST(RuleDataEnums, EnvironmentVegetation) {
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kBarren)), EnvironmentType::Vegetation::kBarren);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kConiferous)), EnvironmentType::Vegetation::kConiferous);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kDeciduous)), EnvironmentType::Vegetation::kDeciduous);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kGrasslands)), EnvironmentType::Vegetation::kGrasslands);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kHeath)), EnvironmentType::Vegetation::kHeath);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kJungle)), EnvironmentType::Vegetation::kJungle);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kPlains)), EnvironmentType::Vegetation::kPlains);
		EXPECT_EQ(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kTundra)), EnvironmentType::Vegetation::kTundra);

		EXPECT_NE(EnvironmentType::vegetation(EnvironmentType::toString(EnvironmentType::Vegetation::kBarren)), EnvironmentType::Vegetation::kGrasslands);
	}

	TEST(RuleDataEnums, EnvironmentWater) {
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kBreaks)), EnvironmentType::Water::kBreaks);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kDesert)), EnvironmentType::Water::kDesert);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kFreshCoast)), EnvironmentType::Water::kFreshCoast);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kGlacier)), EnvironmentType::Water::kGlacier);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kIslet)), EnvironmentType::Water::kIslet);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kLake)), EnvironmentType::Water::kLake);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kMarsh)), EnvironmentType::Water::kMarsh);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kOasis)), EnvironmentType::Water::kOasis);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kOcean)), EnvironmentType::Water::kOcean);
		EXPECT_EQ(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kSaltShore)), EnvironmentType::Water::kSaltShore);

		EXPECT_NE(EnvironmentType::water(EnvironmentType::toString(EnvironmentType::Water::kSaltShore)), EnvironmentType::Water::kLake);
	}

}