#include <gtest/gtest.h>

#include "SkillCategoryData.h"
#include "SkillGroupType.h"
#include "StatType.h"

namespace {

	TEST(SkillCategoryData, Constructor) {
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic);
		EXPECT_EQ(0, strcmp(scd.id().c_str(), "SKILLCATEGORY_ARTISTIC_TESTCATEGORY"));
		EXPECT_EQ(0, strcmp(scd.name().c_str(), "TestCategory"));
		EXPECT_EQ(scd.getGroup(), SkillGroupType::Type::kArtistic);
		EXPECT_EQ(0, strcmp(scd.getFullName().c_str(), "Artistic - TestCategory"));
		EXPECT_EQ(scd.getDefaultSkillProgression(), SkillProgressionType::kStandard);
		EXPECT_EQ(scd.getSkillCategoryProgression(), SkillProgressionType::kStandard);

		SkillCategoryData scd2("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic, SkillProgressionType::kLimited, SkillProgressionType::kNone);
		EXPECT_EQ(scd2.getDefaultSkillProgression(), SkillProgressionType::kLimited);
		EXPECT_EQ(scd2.getSkillCategoryProgression(), SkillProgressionType::kNone);
	}

	TEST(SkillCategoryData, Stats) {
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic);
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

	TEST(SkillCategoryData, SkillProgression) {
		SkillCategoryData scd("SKILLCATEGORY_ARTISTIC_TESTCATEGORY", "TestCategory", SkillGroupType::Type::kArtistic);

		// Invalid category type
		try {
			scd.setSkillProgressions(SkillProgressionType::kStandard, SkillProgressionType::kCombined);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard or kNone."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(SkillProgressionType::kStandard, SkillProgressionType::kLimited);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard or kNone."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(SkillProgressionType::kStandard, SkillProgressionType::kSpecial);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard or kNone."));
		} catch (...) {
			FAIL();
		}

		// Invalid combinations
		try {
			scd.setSkillProgressions(SkillProgressionType::kCombined, SkillProgressionType::kStandard);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard if the skill progression is also set to kStandard."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(SkillProgressionType::kLimited, SkillProgressionType::kStandard);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard if the skill progression is also set to kStandard."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(SkillProgressionType::kNone, SkillProgressionType::kStandard);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard if the skill progression is also set to kStandard."));
		} catch (...) {
			FAIL();
		}
		try {
			scd.setSkillProgressions(SkillProgressionType::kSpecial, SkillProgressionType::kStandard);
			FAIL();
		} catch (InvalidSkillProgression err) {
			EXPECT_EQ(err.what(), std::string("Category progression may only be kStandard if the skill progression is also set to kStandard."));
		} catch (...) {
			FAIL();
		}

		scd.setSkillProgressions(SkillProgressionType::kStandard, SkillProgressionType::kStandard);
		EXPECT_EQ(scd.getDefaultSkillProgression(), SkillProgressionType::kStandard);
		EXPECT_EQ(scd.getSkillCategoryProgression(), SkillProgressionType::kStandard);

		scd.setSkillProgressions(SkillProgressionType::kCombined, SkillProgressionType::kNone);
		EXPECT_EQ(scd.getDefaultSkillProgression(), SkillProgressionType::kCombined);
		EXPECT_EQ(scd.getSkillCategoryProgression(), SkillProgressionType::kNone);

		scd.setSkillProgressions(SkillProgressionType::kLimited, SkillProgressionType::kNone);
		EXPECT_EQ(scd.getDefaultSkillProgression(), SkillProgressionType::kLimited);
		EXPECT_EQ(scd.getSkillCategoryProgression(), SkillProgressionType::kNone);

		scd.setSkillProgressions(SkillProgressionType::kNone, SkillProgressionType::kNone);
		EXPECT_EQ(scd.getDefaultSkillProgression(), SkillProgressionType::kNone);
		EXPECT_EQ(scd.getSkillCategoryProgression(), SkillProgressionType::kNone);

		scd.setSkillProgressions(SkillProgressionType::kSpecial, SkillProgressionType::kNone);
		EXPECT_EQ(scd.getDefaultSkillProgression(), SkillProgressionType::kSpecial);
		EXPECT_EQ(scd.getSkillCategoryProgression(), SkillProgressionType::kNone);
	}
}