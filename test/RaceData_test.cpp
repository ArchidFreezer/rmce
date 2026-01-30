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

		SkillData sk1("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		std::unique_ptr<SubcategoriedSkillData> s1 = std::make_unique<SubcategoriedSkillData>(sk1, "Sub");
		std::unique_ptr<SubcategoriedSkillData> s2 = std::make_unique<SubcategoriedSkillData>(sk2, "Sub");
		EXPECT_STREQ(s1->id().c_str(), "SKILL1_ID_Sub");

		race.addEverymanSkill(std::move(s1));
		race.addEverymanSkill(std::move(s2));

		const std::set<SubcategoriedSkillData> eset(race.everymanSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<SubcategoriedSkillData> eset2(race.everymanSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.everymanSkills()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		std::unique_ptr<SubcategoriedSkillData> s2b = std::make_unique<SubcategoriedSkillData>(sk2, "Sub");
		EXPECT_FALSE(race.isEverymanSkill(sk2));
		EXPECT_TRUE(race.isEverymanSkill(sk2, "Sub"));
		EXPECT_THROW(race.addEverymanSkill(std::move(s2b)), InvalidSkillDevelopment);

		SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(race.isEverymanSkill(sk3));

		SkillData sk4("SKILL4_ID");
		std::unique_ptr<SubcategoriedSkillData> s4 = std::make_unique<SubcategoriedSkillData>(sk4, "");
		race.addRestrictedSkill(std::move(s4));
		std::unique_ptr<SubcategoriedSkillData> s5 = std::make_unique<SubcategoriedSkillData>(sk4, "");
		EXPECT_THROW(race.addEverymanSkill(std::move(s5)), InvalidSkillDevelopment);
	}

	TEST(RaceData, RestrictedSkills) {
		RaceData race("RACE_ID");

		SkillData sk1("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		std::unique_ptr<SubcategoriedSkillData> s1 = std::make_unique<SubcategoriedSkillData>(sk1, "Sub");
		std::unique_ptr<SubcategoriedSkillData> s2 = std::make_unique<SubcategoriedSkillData>(sk2, "Sub");

		race.addRestrictedSkill(std::move(s1));
		race.addRestrictedSkill(std::move(s2));

		const std::set<SubcategoriedSkillData> eset(race.restrictedSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<SubcategoriedSkillData> eset2(race.restrictedSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.restrictedSkills()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillData sk2b("SKILL2_ID");
		std::unique_ptr<SubcategoriedSkillData> s2b = std::make_unique<SubcategoriedSkillData>(sk2b, "Sub");
		EXPECT_FALSE(race.isRestrictedSkill(sk2b));
		EXPECT_TRUE(race.isRestrictedSkill(sk2b, "Sub"));
		EXPECT_THROW(race.addRestrictedSkill(std::move(s2b)), InvalidSkillDevelopment);

		SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(race.isRestrictedSkill(sk3));

		SkillData sk4("SKILL4_ID");
		std::unique_ptr<SubcategoriedSkillData> s4 = std::make_unique<SubcategoriedSkillData>(sk4, "");
		race.addEverymanSkill(std::move(s4));
		std::unique_ptr<SubcategoriedSkillData> s5 = std::make_unique<SubcategoriedSkillData>(sk4, "");
		EXPECT_THROW(race.addRestrictedSkill(std::move(s5)), InvalidSkillDevelopment);
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

		SkillData sk1("SKILL1_ID");
		SkillData sk1a("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		std::unique_ptr<SubcategoriedSkillData> s1 = std::make_unique<SubcategoriedSkillData>(sk1, "Sub");
		std::unique_ptr<SubcategoriedSkillData> s1a = std::make_unique<SubcategoriedSkillData>(sk1a, "Sub");
		std::unique_ptr<SubcategoriedSkillData> s2 = std::make_unique<SubcategoriedSkillData>(sk2, "Sub");
		std::unique_ptr<SubcategoriedSkillData> s2a = std::make_unique<SubcategoriedSkillData>(sk2, "Sub");

		race.setSkillBonus(std::move(s1), 5);
		race.setSkillBonus(std::move(s2), 10);

		EXPECT_TRUE(race.isBonusSkill(sk1a, "Sub"));

		EXPECT_EQ(race.skillBonus(*s1a), 5);
		EXPECT_FALSE(race.isBonusSkill(sk1));
		EXPECT_TRUE(race.isBonusSkill(sk1, "Sub"));

		EXPECT_THROW(race.setSkillBonus(std::move(s1a), 15), InvalidSkillBonus);
		EXPECT_EQ(race.skillBonus(*s2a), 10);

		EXPECT_EQ(race.skillBonus(sk2, "Sub"), 10);
		EXPECT_EQ(race.skillBonus(sk2), 0);

		int count{ 0 };
		for (auto& skill : race.skillsWithBonus()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

	}

	TEST(RaceData, Book) {
		RaceData race("RACE_ID");
		BookData book("TEST_BOOK");

		EXPECT_FALSE(race.book());
		race.setBook(book);
		EXPECT_TRUE(race.book());
		EXPECT_STREQ(race.book().value()->id().c_str(), "TEST_BOOK");
	}
}