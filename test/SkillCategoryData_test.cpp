#include <gtest/gtest.h>

#include <SkillCategoryData.h>
#include <SkillGroupData.h>
#include <StatType.h>

namespace {
	TEST(SkillCategoryData, Constructor) {
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillGroupData sg("GROUP1_ID", "Artistic");
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", sg, spd_std, spd_std, false);
		EXPECT_STREQ(scd.id().c_str(), "SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		EXPECT_STREQ(scd.name().c_str(), "TestCategory");
		EXPECT_EQ(scd.group().name(), "Artistic");
		EXPECT_STREQ(scd.fullName().c_str(), "Artistic - TestCategory");
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), spd_std.name().c_str());
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), spd_std.name().c_str());


		SkillProgressionTypeData spd_ltd("LIMITED_ID", "Limited", 0.0, 1.0, 1.0, 0.5, 0.0);
		SkillProgressionTypeData spd_none("NONE_ID", "None", 0.0, 0.0, 0.0, 0.0, 0.0);
		SkillCategoryData scd2("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", sg, spd_ltd, spd_none, false);
		EXPECT_STREQ(scd2.defaultSkillProgression().name().c_str(), spd_ltd.name().c_str());
		EXPECT_STREQ(scd2.skillCategoryProgression().name().c_str(), spd_none.name().c_str());
	}

	TEST(SkillCategoryData, Stats) {
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillGroupData sg("Artistic");
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", sg, spd_std, spd_std, false);
		int i{};
		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 1);
		EXPECT_EQ(i, 1);

		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 2);
		EXPECT_EQ(i, 2);
		
		i = scd.addStat(StatType::Type::kStrength);
		EXPECT_EQ(scd.numberOfStats(), 3);
		EXPECT_EQ(i, 3);

		EXPECT_THROW(scd.addStat(StatType::Type::kStrength), StatType::TooManyStatsException);

		EXPECT_EQ(scd.numberOfStats(), 3);

		bool removed{};
		removed = scd.removeStat(StatType::Type::kAgility);
		EXPECT_EQ(removed, true);
		EXPECT_EQ(scd.numberOfStats(), 2);

		removed = scd.removeStat(StatType::Type::kMemory);
		EXPECT_EQ(removed, false);
		EXPECT_EQ(scd.numberOfStats(), 2);

		scd.clearStats();
		EXPECT_EQ(scd.numberOfStats(), 0);
	}
	
	TEST(SkillCategoryData, CharStats) {
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillGroupData sg("Artistic");
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", sg, spd_std, spd_std, false);
		int i{};
		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 1);
		EXPECT_EQ(i, 1);

		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 2);
		EXPECT_EQ(i, 2);

		i = scd.addStat(StatType::Type::kStrength);
		EXPECT_EQ(scd.numberOfStats(), 3);
		EXPECT_EQ(i, 3);

		scd.setUseRealmStats(true);
		EXPECT_EQ(scd.numberOfStats(), 0);

		EXPECT_THROW(scd.addStat(StatType::Type::kStrength), SkillCategoryData::UsingCharacterRealmStatsException);

		scd.setUseRealmStats(false);
		EXPECT_NO_THROW(scd.addStat(StatType::Type::kStrength));
		EXPECT_EQ(scd.numberOfStats(), 1);
	}

	TEST(SkillCategoryData, SkillProgression) {
		SkillProgressionTypeData spd_cmb("COMBINED_ID", "Combined", -30.0, 5.0, 3.0, 1.5, 0.5);
		SkillProgressionTypeData spd_ltd("LIMITED_ID", "Limited", 0.0, 1.0, 1.0, 0.5, 0.0);
		SkillProgressionTypeData spd_none("NONE_ID", "None", 0.0, 0.0, 0.0, 0.0, 0.0);
		SkillProgressionTypeData spd_spc("SPECIAL_ID", "Special", 0.0, 6.0, 5.0, 4.0, 3.0);
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillProgressionTypeData spd_cstd("CATEGORY_STANDARD_ID", "Category Standard", -15.0, 2.0, 1.0, 0.5, 0.0);
		SkillGroupData sg("Artistic");
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", sg, spd_std, spd_std, false);

		// Invalid category type
		EXPECT_THROW(scd.setSkillProgressions(spd_std, spd_cmb), InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_std, spd_ltd), InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_std, spd_spc), InvalidSkillProgression);

		// Invalid combinations
		EXPECT_THROW(scd.setSkillProgressions(spd_cmb, spd_cstd), InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_ltd, spd_cstd), InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_none, spd_cstd), InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_spc, spd_cstd), InvalidSkillProgression);

		scd.setSkillProgressions(spd_std, spd_cstd);
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), "Standard");
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), "Category Standard");

		scd.setSkillProgressions(spd_cmb, spd_none);
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), "Combined");
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), "None");

		scd.setSkillProgressions(spd_ltd, spd_none);
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), "Limited");
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), "None");

		scd.setSkillProgressions(spd_none, spd_none);
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), "None");
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), "None");

		scd.setSkillProgressions(spd_spc, spd_none);
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), "Special");
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), "None");
	}
}