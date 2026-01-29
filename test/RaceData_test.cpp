#include <gtest/gtest.h>
#include <RaceData.h>

namespace {
	TEST(RaceData, AdolescentLanguageAbility) {
		RaceData race("RACE_ID");

		LanguageData l1("L1_ID");
		l1.setName("L1_NAME");
		l1.setIsSomantic(true);
		LanguageAbility la1(l1, 6, 4, 2);
		race.setAdolescentLanguageAbility(la1);

		LanguageData l2("L2_ID");
		l2.setName("L2_NAME");
		LanguageAbility la2(l2, 5, 5, 0);
		race.setAdolescentLanguageAbility(la2);

		// Test the language form is passed through to the ability
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").isSomantic(), false);
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").isSpoken(), true);
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").isWritten(), true);

		EXPECT_EQ(race.adolescentLanguageAbility("L1_NAME").spoken(), 6);
		EXPECT_EQ(race.adolescentLanguageAbility("L1_NAME").written(), 4);
		EXPECT_EQ(race.adolescentLanguageAbility("L1_NAME").somantic(), 2);

		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").written(), 5);
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").spoken(), 5);
	}

	TEST(RaceData, StartingLanguageAbility) {
		RaceData race("RACE_ID");

		LanguageData l1("L1_ID");
		l1.setName("L1_NAME");
		l1.setIsSomantic(true);
		LanguageAbility la1(l1, 6, 4, 2);
		race.setStartingLanguageAbility(la1);

		LanguageData l2("L2_ID");
		l2.setName("L2_NAME");
		LanguageAbility la2(l2, 5, 5, 0);
		race.setStartingLanguageAbility(la2);

		// Test the language form is passed through to the ability
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isSomantic(), false);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isSpoken(), true);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isWritten(), true);

		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").spoken(), 6);
		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").written(), 4);
		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").somantic(), 2);

		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").written(), 5);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").spoken(), 5);
	}

	TEST(RaceData, StatBonus) {
		RaceData race("RACE_ID");

		race.setStatBonus(StatType::kAgility, 5);
		race.setStatBonus(StatType::kStrength, 10);
		EXPECT_EQ(race.statBonus(StatType::kAgility), 5);
		EXPECT_EQ(race.statBonus(StatType::kStrength), 10);
		EXPECT_EQ(race.statBonus(StatType::kConstitution), 0);

		EXPECT_EQ(race.statsWithBonus().size(), 2);

		for (auto& stat : race.statsWithBonus()) {
			if (stat == StatType::kAgility || stat == StatType::kStrength) continue;
			FAIL();
		}
	}

	TEST(RaceData, EverymanSkills) {
		RaceData race("RACE_ID");

		SkillData s1("SKILL1_ID");
		SkillData s2("SKILL2_ID");

		race.addEverymanSkill(s1);
		race.addEverymanSkill(s2);

		std::set<const SkillData*> eset(race.everymanSkills());
		EXPECT_EQ(eset.size(), 2);
		eset.clear();
		EXPECT_EQ(eset.size(), 0);

		std::set<const SkillData*> eset2(race.everymanSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.everymanSkills()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillData s2b("SKILL2_ID");
		EXPECT_TRUE(race.isEverymanSkill(s2b));
		EXPECT_THROW(race.addEverymanSkill(s2b), InvalidSkillDevelopment);

		SkillData s3("SKILL3_ID");
		EXPECT_FALSE(race.isEverymanSkill(s3));

		SkillData s4("SKILL4_ID");
		race.addRestrictedSkill(s4);
		EXPECT_THROW(race.addEverymanSkill(s4), InvalidSkillDevelopment);
	}

	TEST(RaceData, RestrictedSkills) {
		RaceData race("RACE_ID");

		SkillData s1("SKILL1_ID");
		SkillData s2("SKILL2_ID");

		race.addRestrictedSkill(s1);
		race.addRestrictedSkill(s2);

		std::set<const SkillData*> eset(race.restrictedSkills());
		EXPECT_EQ(eset.size(), 2);
		eset.clear();
		EXPECT_EQ(eset.size(), 0);

		std::set<const SkillData*> eset2(race.restrictedSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.restrictedSkills()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillData s2b("SKILL2_ID");
		EXPECT_TRUE(race.isRestrictedSkill(s2b));
		EXPECT_THROW(race.addRestrictedSkill(s2b), InvalidSkillDevelopment);

		SkillData s3("SKILL3_ID");
		EXPECT_FALSE(race.isRestrictedSkill(s3));

		SkillData s4("SKILL4_ID");
		race.addEverymanSkill(s4);
		EXPECT_THROW(race.addRestrictedSkill(s4), InvalidSkillDevelopment);
	}

	TEST(RaceData, EverymanSkillCategories) {
		RaceData race("RACE_ID");

		SkillCategoryData s1("SKILL1_ID");
		SkillCategoryData s2("SKILL2_ID");

		race.addEverymanSkillCategory(s1);
		race.addEverymanSkillCategory(s2);

		std::set<const SkillCategoryData*> eset(race.everymanSkillCategories());
		EXPECT_EQ(eset.size(), 2);
		eset.clear();
		EXPECT_EQ(eset.size(), 0);

		std::set<const SkillCategoryData*> eset2(race.everymanSkillCategories());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.everymanSkillCategories()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillCategoryData s2b("SKILL2_ID");
		EXPECT_TRUE(race.isEverymanSkillCategory(s2b));
		EXPECT_THROW(race.addEverymanSkillCategory(s2b), InvalidSkillDevelopment);

		SkillCategoryData s3("SKILL3_ID");
		EXPECT_FALSE(race.isEverymanSkillCategory(s3));

		SkillCategoryData s4("SKILL4_ID");
		race.addRestrictedSkillCategory(s4);
		EXPECT_THROW(race.addEverymanSkillCategory(s4), InvalidSkillDevelopment);
	}

	TEST(RaceData, RestrictedSkillCategories) {
		RaceData race("RACE_ID");

		SkillCategoryData s1("SKILL1_ID");
		SkillCategoryData s2("SKILL2_ID");

		race.addRestrictedSkillCategory(s1);
		race.addRestrictedSkillCategory(s2);

		std::set<const SkillCategoryData*> eset(race.restrictedSkillCategories());
		EXPECT_EQ(eset.size(), 2);
		eset.clear();
		EXPECT_EQ(eset.size(), 0);

		std::set<const SkillCategoryData*> eset2(race.restrictedSkillCategories());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.restrictedSkillCategories()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillCategoryData s2b("SKILL2_ID");
		EXPECT_TRUE(race.isRestrictedSkillCategory(s2b));
		EXPECT_THROW(race.addRestrictedSkillCategory(s2b), InvalidSkillDevelopment);

		SkillCategoryData s3("SKILL3_ID");
		EXPECT_FALSE(race.isRestrictedSkillCategory(s3));

		SkillCategoryData s4("SKILL4_ID");
		race.addEverymanSkillCategory(s4);
		EXPECT_THROW(race.addRestrictedSkillCategory(s4), InvalidSkillDevelopment);
	}

	TEST(RaceData, SkillBonus) {
		RaceData race("RACE_ID");

		SkillData s1("SKILL1_ID");
		SkillData s1a("SKILL1_ID");
		SkillData s2("SKILL2_ID");

		race.setSkillBonus(s1, 5);
		race.setSkillBonus(s2, 10);

		EXPECT_TRUE(race.isBonusSkill(s1));

		EXPECT_EQ(race.skillBonus(s1a), 5);
		EXPECT_TRUE(race.isBonusSkill(s1a));

		EXPECT_THROW(race.setSkillBonus(s1a, 15), InvalidSkillBonus);
		EXPECT_EQ(race.skillBonus(s2), 10);

		int count{ 0 };
		for (auto& skill : race.skillsWithBonus()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

	}

	TEST(RaceData, Book) {
		RaceData race("RACE_ID");
		BookData book("TEST_BOOK");

		race.setBook(book);
		EXPECT_STREQ(race.book().id().c_str(), "TEST_BOOK");

	}
}