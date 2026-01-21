#include <gtest/gtest.h>

#include <SkillCategoryData.h>
#include <SkillGroupType.h>
#include <StatType.h>

namespace {
	TEST(SkillCategoryData, Constructor) {
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic, spd_std, spd_std, false);
		EXPECT_STREQ(scd.id().c_str(), "SKILLCATEGORY_ARTISTIC_TESTCATEGORY");
		EXPECT_STREQ(scd.name().c_str(), "TestCategory");
		EXPECT_EQ(scd.getGroup(), SkillGroupType::Type::kArtistic);
		EXPECT_STREQ(scd.getFullName().c_str(), "Artistic - TestCategory");
		EXPECT_STREQ(scd.getDefaultSkillProgression().name().c_str(), spd_std.name().c_str());
		EXPECT_STREQ(scd.getSkillCategoryProgression().name().c_str(), spd_std.name().c_str());


		SkillProgressionTypeData spd_ltd("LIMITED_ID", "Limited", 0.0, 1.0, 1.0, 0.5, 0.0);
		SkillProgressionTypeData spd_none("NONE_ID", "None", 0.0, 0.0, 0.0, 0.0, 0.0);
		SkillCategoryData scd2("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic, spd_ltd, spd_none, false);
		EXPECT_STREQ(scd2.getDefaultSkillProgression().name().c_str(), spd_ltd.name().c_str());
		EXPECT_STREQ(scd2.getSkillCategoryProgression().name().c_str(), spd_none.name().c_str());
	}

	TEST(SkillCategoryData, Stats) {
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic, spd_std, spd_std, false);
		int i{};
		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.getNumberOfStats(), 1);
		EXPECT_EQ(i, 1);

		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.getNumberOfStats(), 2);
		EXPECT_EQ(i, 2);
		
		i = scd.addStat(StatType::Type::kStrength);
		EXPECT_EQ(scd.getNumberOfStats(), 3);
		EXPECT_EQ(i, 3);

		try {
			scd.addStat(StatType::Type::kStrength);
			FAIL();
		} catch (TooManyStatsException err) {
			// SUCCEED(); Could have used this if we don't care about the actual exception text
			EXPECT_EQ(err.what(),std::string("You attempted to add more than 3 stat bonuses to a skill category."));
		} catch (...) {
			FAIL();
		}

		EXPECT_EQ(scd.getNumberOfStats(), 3);

		bool removed{};
		removed = scd.removeStat(StatType::Type::kAgility);
		EXPECT_EQ(removed, true);
		EXPECT_EQ(scd.getNumberOfStats(), 2);

		removed = scd.removeStat(StatType::Type::kMemory);
		EXPECT_EQ(removed, false);
		EXPECT_EQ(scd.getNumberOfStats(), 2);

		scd.clearStats();
		EXPECT_EQ(scd.getNumberOfStats(), 0);
	}
	
	TEST(SkillCategoryData, CharStats) {
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic, spd_std, spd_std, false);
		int i{};
		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.getNumberOfStats(), 1);
		EXPECT_EQ(i, 1);

		i = scd.addStat(StatType::Type::kAgility);
		EXPECT_EQ(scd.getNumberOfStats(), 2);
		EXPECT_EQ(i, 2);

		i = scd.addStat(StatType::Type::kStrength);
		EXPECT_EQ(scd.getNumberOfStats(), 3);
		EXPECT_EQ(i, 3);

		scd.setUseRealmStats(true);
		EXPECT_EQ(scd.getNumberOfStats(), 0);

		try {
			scd.addStat(StatType::Type::kStrength);
			FAIL();
		} catch (UsingCharacterRealmStatsException err) {
			SUCCEED();
		} catch (...) {
			FAIL();
		}

		scd.setUseRealmStats(false);

		try {
			scd.addStat(StatType::Type::kStrength);
			SUCCEED();
		} catch (...) {
			FAIL();
		}
		EXPECT_EQ(scd.getNumberOfStats(), 1);
	}

	TEST(SkillCategoryData, SkillProgression) {
		SkillProgressionTypeData spd_cmb("COMBINED_ID", "Combined", -30.0, 5.0, 3.0, 1.5, 0.5);
		SkillProgressionTypeData spd_ltd("LIMITED_ID", "Limited", 0.0, 1.0, 1.0, 0.5, 0.0);
		SkillProgressionTypeData spd_none("NONE_ID", "None", 0.0, 0.0, 0.0, 0.0, 0.0);
		SkillProgressionTypeData spd_spc("SPECIAL_ID", "Special", 0.0, 6.0, 5.0, 4.0, 3.0);
		SkillProgressionTypeData spd_std("STANDARD_ID", "Standard", -15.0, 3.0, 2.0, 1.0, 0.5);
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic, spd_std, spd_std, false);

		// Invalid category type
		try {
			scd.setSkillProgressions(spd_std, spd_cmb);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard or None."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(spd_std, spd_ltd);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard or None."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(spd_std, spd_spc);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard or None."));
		} catch (...) {
			FAIL();
		}

		// Invalid combinations
		try {
			scd.setSkillProgressions(spd_cmb, spd_std);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard if the skill progression is also set to Standard."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(spd_ltd, spd_std);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard if the skill progression is also set to Standard."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(spd_none, spd_std);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard if the skill progression is also set to Standard."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(spd_spc, spd_std);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be Standard if the skill progression is also set to Standard."));
		} catch (...) {
			FAIL();
		}

		scd.setSkillProgressions(spd_std, spd_std);
		EXPECT_STREQ(scd.getDefaultSkillProgression().name().c_str(), "Standard");
		EXPECT_STREQ(scd.getSkillCategoryProgression().name().c_str(), "Standard");

		scd.setSkillProgressions(spd_cmb, spd_none);
		EXPECT_STREQ(scd.getDefaultSkillProgression().name().c_str(), "Combined");
		EXPECT_STREQ(scd.getSkillCategoryProgression().name().c_str(), "None");

		scd.setSkillProgressions(spd_ltd, spd_none);
		EXPECT_STREQ(scd.getDefaultSkillProgression().name().c_str(), "Limited");
		EXPECT_STREQ(scd.getSkillCategoryProgression().name().c_str(), "None");

		scd.setSkillProgressions(spd_none, spd_none);
		EXPECT_STREQ(scd.getDefaultSkillProgression().name().c_str(), "None");
		EXPECT_STREQ(scd.getSkillCategoryProgression().name().c_str(), "None");

		scd.setSkillProgressions(spd_spc, spd_none);
		EXPECT_STREQ(scd.getDefaultSkillProgression().name().c_str(), "Special");
		EXPECT_STREQ(scd.getSkillCategoryProgression().name().c_str(), "None");
	}
}