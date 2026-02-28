#include <gtest/gtest.h>

#include <SkillCategoryData.h>
#include <SkillGroupData.h>
#include <StatType.h>

using namespace rm;

namespace {
	TEST(SkillCategoryData, Constructor) {
		rule::SkillProgressionTypeData spd_sk_std("SKILLPROGRESSIONTYPE_STANDARD");
		spd_sk_std.setName("Standard");

		rule::SkillProgressionTypeData spd_c_std("SKILLPROGRESSIONTYPE_CATEGORY_STANDARD");
		spd_c_std.setName("Category Standard");

		rule::SkillGroupData sg("GROUP1_ID");
		sg.setName("Artistic");

		rule::SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		scd.setUseRealmStats(false);

		scd.setGroup(sg);
		EXPECT_STREQ(scd.id().c_str(), "SKILLCATEGORY_ARTISTIC_TESTCATEGORY");

		scd.setName("TestCategory");
		EXPECT_STREQ(scd.name().c_str(), "TestCategory");

		scd.setGroup(sg);
		EXPECT_EQ(scd.group().name(), "Artistic");
		EXPECT_STREQ(scd.fullName().c_str(), "Artistic - TestCategory");

		scd.setSkillProgressions(spd_sk_std, spd_c_std);
		EXPECT_STREQ(scd.defaultSkillProgression().name().c_str(), spd_sk_std.name().c_str());
		EXPECT_STREQ(scd.skillCategoryProgression().name().c_str(), spd_c_std.name().c_str());


		rule::SkillProgressionTypeData spd_ltd("SKILLPROGRESSIONTYPE_LIMITED");
		spd_ltd.setName("Limited");
		rule::SkillProgressionTypeData spd_none("SKILLPROGRESSIONTYPE_NONE");
		spd_none.setName("None");
		rule::SkillCategoryData scd2("SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		scd2.setSkillProgressions(spd_ltd, spd_none);
		EXPECT_STREQ(scd2.defaultSkillProgression().name().c_str(), spd_ltd.name().c_str());
		EXPECT_STREQ(scd2.skillCategoryProgression().name().c_str(), spd_none.name().c_str());
	}

	TEST(SkillCategoryData, Stats) {
		rule::SkillProgressionTypeData spd_std("SKILLPROGRESSIONTYPE_STANDARD");
		rule::SkillGroupData sg("Artistic");
		rule::SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		int i{};
		i = scd.addStat(rule::enums::StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 1);
		EXPECT_EQ(i, 1);

		i = scd.addStat(rule::enums::StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 2);
		EXPECT_EQ(i, 2);
		
		i = scd.addStat(rule::enums::StatType::Type::kStrength);
		EXPECT_EQ(scd.numberOfStats(), 3);
		EXPECT_EQ(i, 3);

		EXPECT_THROW(scd.addStat(rule::enums::StatType::Type::kStrength), rule::enums::StatType::TooManyStatsException);

		EXPECT_EQ(scd.numberOfStats(), 3);

		bool removed{};
		removed = scd.removeStat(rule::enums::StatType::Type::kAgility);
		EXPECT_EQ(removed, true);
		EXPECT_EQ(scd.numberOfStats(), 2);

		removed = scd.removeStat(rule::enums::StatType::Type::kMemory);
		EXPECT_EQ(removed, false);
		EXPECT_EQ(scd.numberOfStats(), 2);

		scd.clearStats();
		EXPECT_EQ(scd.numberOfStats(), 0);
	}
	
	TEST(SkillCategoryData, CharStats) {
		rule::SkillProgressionTypeData spd_std("SKILLPROGRESSIONTYPE_STANDARD");
		rule::SkillGroupData sg("Artistic");
		rule::SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		int i{};
		i = scd.addStat(rule::enums::StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 1);
		EXPECT_EQ(i, 1);

		i = scd.addStat(rule::enums::StatType::Type::kAgility);
		EXPECT_EQ(scd.numberOfStats(), 2);
		EXPECT_EQ(i, 2);

		i = scd.addStat(rule::enums::StatType::Type::kStrength);
		EXPECT_EQ(scd.numberOfStats(), 3);
		EXPECT_EQ(i, 3);

		scd.setUseRealmStats(true);
		EXPECT_EQ(scd.numberOfStats(), 0);

		EXPECT_THROW(scd.addStat(rule::enums::StatType::Type::kStrength), rule::SkillCategoryData::UsingCharacterRealmStatsException);

		scd.setUseRealmStats(false);
		EXPECT_NO_THROW(scd.addStat(rule::enums::StatType::Type::kStrength));
		EXPECT_EQ(scd.numberOfStats(), 1);
	}

	TEST(SkillCategoryData, SkillProgression) {
		rule::SkillProgressionTypeData spd_cmb("SKILLPROGRESSIONTYPE_COMBINED");
		spd_cmb.setName("Combined");
		rule::SkillProgressionTypeData spd_ltd("SKILLPROGRESSIONTYPE_LIMITED");
		spd_ltd.setName("Limited");
		rule::SkillProgressionTypeData spd_none("SKILLPROGRESSIONTYPE_NONE");
		spd_none.setName("None");
		rule::SkillProgressionTypeData spd_spc("SKILLPROGRESSIONTYPE_SPECIAL");
		spd_spc.setName("Special");
		rule::SkillProgressionTypeData spd_std("SKILLPROGRESSIONTYPE_STANDARD");
		spd_std.setName("Standard");
		rule::SkillProgressionTypeData spd_cstd("SKILLPROGRESSIONTYPE_CATEGORY_STANDARD");
		spd_cstd.setName("Category Standard");
		rule::SkillGroupData sg("Artistic");
		rule::SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		scd.setName("TestCategory");
		scd.setGroup(sg);

		// Invalid category type
		EXPECT_THROW(scd.setSkillProgressions(spd_std, spd_cmb), rule::InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_std, spd_ltd), rule::InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_std, spd_spc), rule::InvalidSkillProgression);

		// Invalid combinations
		EXPECT_THROW(scd.setSkillProgressions(spd_cmb, spd_cstd), rule::InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_ltd, spd_cstd), rule::InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_none, spd_cstd), rule::InvalidSkillProgression);
		EXPECT_THROW(scd.setSkillProgressions(spd_spc, spd_cstd), rule::InvalidSkillProgression);

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