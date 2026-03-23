#include <gtest/gtest.h>
#include <RaceData.h>

using namespace rm;
using namespace rm::game::character;

namespace {
	TEST(RaceData, AdolescentLanguageAbility) {
		rule::RaceData race("RACE_ID");

		rule::LanguageData l1("L1_ID");
		l1.setName("L1_NAME");
		l1.setIsSomatic(true);
		LanguageAbility la1(l1, 6, 4, 2);
		race.setAdolescentLanguageAbility(la1);

		rule::LanguageData l2("L2_ID");
		l2.setName("L2_NAME");
		LanguageAbility la2(l2, 5, 5, 0);
		race.setAdolescentLanguageAbility(la2);

		// Test the language form is passed through to the ability
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").isSomatic(), false);
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").isSpoken(), true);
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").isWritten(), true);

		EXPECT_EQ(race.adolescentLanguageAbility("L1_NAME").spoken(), 6);
		EXPECT_EQ(race.adolescentLanguageAbility("L1_NAME").written(), 4);
		EXPECT_EQ(race.adolescentLanguageAbility("L1_NAME").somatic(), 2);

		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").written(), 5);
		EXPECT_EQ(race.adolescentLanguageAbility("L2_NAME").spoken(), 5);
	}

	TEST(RaceData, StartingLanguageAbility) {
		rule::RaceData race("RACE_ID");

		rule::LanguageData l1("L1_ID");
		l1.setName("L1_NAME");
		l1.setIsSomatic(true);
		LanguageAbility la1(l1, 6, 4, 2);
		race.setStartingLanguageAbility(la1);

		rule::LanguageData l2("L2_ID");
		l2.setName("L2_NAME");
		LanguageAbility la2(l2, 5, 5, 0);
		race.setStartingLanguageAbility(la2);

		// Test the language form is passed through to the ability
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isSomatic(), false);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isSpoken(), true);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isWritten(), true);

		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").spoken(), 6);
		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").written(), 4);
		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").somatic(), 2);

		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").written(), 5);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").spoken(), 5);
	}

	TEST(RaceData, StatBonus) {
		rule::RaceData race("RACE_ID");

		race.setStatBonus(rule::enums::StatType::kAgility, 5);
		race.setStatBonus(rule::enums::StatType::kStrength, 10);
		EXPECT_EQ(race.statBonus(rule::enums::StatType::kAgility), 5);
		EXPECT_EQ(race.statBonus(rule::enums::StatType::kStrength), 10);
		EXPECT_EQ(race.statBonus(rule::enums::StatType::kConstitution), 0);

		EXPECT_EQ(race.statsWithBonus().size(), 2);

		for (auto& stat : race.statsWithBonus()) {
			if (stat == rule::enums::StatType::kAgility || stat == rule::enums::StatType::kStrength) continue;
			FAIL();
		}
	}

	TEST(RaceData, EverymanSkills) {
		rule::RaceData race("RACE_ID");

		rule::SkillData sk1("SKILL1_ID");
		rule::SkillData sk2("SKILL2_ID");
		rule::SubcategoriedSkillData s1(sk1, "Sub");
		rule::SubcategoriedSkillData s2(sk2, "Sub");
		EXPECT_STREQ(s1.id().c_str(), "SKILL1_ID_Sub");

		race.addEverymanSkill(s1);
		race.addEverymanSkill(s2);

		const std::set<const rule::SubcategoriedSkillData*> eset(race.everymanSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<const rule::SubcategoriedSkillData*> eset2(race.everymanSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.everymanSkills()) {
			count++;
			if (skill->skillData().id() == "SKILL1_ID" || skill->skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		rule::SubcategoriedSkillData s2b(sk2, "Sub");
		EXPECT_FALSE(race.isEverymanSkill(sk2));
		EXPECT_TRUE(race.isEverymanSkill(sk2, "Sub"));
		EXPECT_THROW(race.addEverymanSkill(s2b), rule::InvalidSkillDevelopment);

		rule::SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(race.isEverymanSkill(sk3));

		rule::SkillData sk4("SKILL4_ID");
		rule::SubcategoriedSkillData s4(sk4, "");
		race.addRestrictedSkill(s4);
		rule::SubcategoriedSkillData s5(sk4, "");
		EXPECT_THROW(race.addEverymanSkill(s5), rule::InvalidSkillDevelopment);
	}

	TEST(RaceData, RestrictedSkills) {
		rule::RaceData race("RACE_ID");

		rule::SkillData sk1("SKILL1_ID");
		rule::SkillData sk2("SKILL2_ID");
		rule::SubcategoriedSkillData s1(sk1, "Sub");
		rule::SubcategoriedSkillData s2(sk2, "Sub");

		race.addRestrictedSkill(s1);
		race.addRestrictedSkill(s2);

		const std::set<const rule::SubcategoriedSkillData*> eset(race.restrictedSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<const rule::SubcategoriedSkillData*> eset2(race.restrictedSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.restrictedSkills()) {
			count++;
			if (skill->skillData().id() == "SKILL1_ID" || skill->skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		rule::SkillData sk2b("SKILL2_ID");
		rule::SubcategoriedSkillData s2b(sk2b, "Sub");
		EXPECT_FALSE(race.isRestrictedSkill(sk2b));
		EXPECT_TRUE(race.isRestrictedSkill(sk2b, "Sub"));
		EXPECT_THROW(race.addRestrictedSkill(s2b), rule::InvalidSkillDevelopment);

		rule::SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(race.isRestrictedSkill(sk3));

		rule::SkillData sk4("SKILL4_ID");
		rule::SubcategoriedSkillData s4(sk4, "");
		race.addEverymanSkill(s4);
		rule::SubcategoriedSkillData s5(sk4, "");
		EXPECT_THROW(race.addRestrictedSkill(s5), rule::InvalidSkillDevelopment);
	}

	TEST(RaceData, EverymanSkillCategories) {
		rule::RaceData race("RACE_ID");

		rule::SkillCategoryData s1("SKILL1_ID");
		rule::SkillCategoryData s2("SKILL2_ID");

		race.addEverymanSkillCategory(s1);
		race.addEverymanSkillCategory(s2);

		std::set<const rule::SkillCategoryData*> eset(race.everymanSkillCategories());
		EXPECT_EQ(eset.size(), 2);
		eset.clear();
		EXPECT_EQ(eset.size(), 0);

		std::set<const rule::SkillCategoryData*> eset2(race.everymanSkillCategories());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.everymanSkillCategories()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		rule::SkillCategoryData s2b("SKILL2_ID");
		EXPECT_TRUE(race.isEverymanSkillCategory(s2b));
		EXPECT_THROW(race.addEverymanSkillCategory(s2b), rule::InvalidSkillDevelopment);

		rule::SkillCategoryData s3("SKILL3_ID");
		EXPECT_FALSE(race.isEverymanSkillCategory(s3));

		rule::SkillCategoryData s4("SKILL4_ID");
		race.addRestrictedSkillCategory(s4);
		EXPECT_THROW(race.addEverymanSkillCategory(s4), rule::InvalidSkillDevelopment);
	}

	TEST(RaceData, RestrictedSkillCategories) {
		rule::RaceData race("RACE_ID");

		rule::SkillCategoryData s1("SKILL1_ID");
		rule::SkillCategoryData s2("SKILL2_ID");

		race.addRestrictedSkillCategory(s1);
		race.addRestrictedSkillCategory(s2);

		std::set<const rule::SkillCategoryData*> eset(race.restrictedSkillCategories());
		EXPECT_EQ(eset.size(), 2);
		eset.clear();
		EXPECT_EQ(eset.size(), 0);

		std::set<const rule::SkillCategoryData*> eset2(race.restrictedSkillCategories());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : race.restrictedSkillCategories()) {
			count++;
			if (skill->id() == "SKILL1_ID" || skill->id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		rule::SkillCategoryData s2b("SKILL2_ID");
		EXPECT_TRUE(race.isRestrictedSkillCategory(s2b));
		EXPECT_THROW(race.addRestrictedSkillCategory(s2b), rule::InvalidSkillDevelopment);

		rule::SkillCategoryData s3("SKILL3_ID");
		EXPECT_FALSE(race.isRestrictedSkillCategory(s3));

		rule::SkillCategoryData s4("SKILL4_ID");
		race.addEverymanSkillCategory(s4);
		EXPECT_THROW(race.addRestrictedSkillCategory(s4), rule::InvalidSkillDevelopment);
	}

	TEST(RaceData, SkillBonus) {
		rule::RaceData race("RACE_ID");

		rule::SkillData sk1("SKILL1_ID");
		rule::SkillData sk1a("SKILL1_ID");
		rule::SkillData sk2("SKILL2_ID");
		rule::SubcategoriedSkillData s1(sk1, "Sub");
		rule::SubcategoriedSkillData s1a(sk1a, "Sub");
		rule::SubcategoriedSkillData s2(sk2, "Sub");
		rule::SubcategoriedSkillData s2a(sk2, "Sub");

		race.setSkillBonus(s1, 5);
		race.setSkillBonus(s2, 10);

		EXPECT_TRUE(race.isBonusSkill(sk1a, "Sub"));

		EXPECT_EQ(race.skillBonus(s1a), 5);
		EXPECT_FALSE(race.isBonusSkill(sk1));
		EXPECT_FALSE(race.isBonusSkill(sk1, "Invalid"));
		EXPECT_TRUE(race.isBonusSkill(sk1, "Sub"));

		EXPECT_THROW(race.setSkillBonus(s1a, 15), rule::InvalidSkillBonus);
		EXPECT_EQ(race.skillBonus(s2a), 10);

		EXPECT_EQ(race.skillBonus(sk2, "Sub"), 10);
		EXPECT_EQ(race.skillBonus(sk2), 0);

		int count{ 0 };
		for (auto& skill : race.skillsWithBonus()) {
			count++;
			if (skill->skillData().id() == "SKILL1_ID" || skill->skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

	}

	TEST(RaceData, Book) {
		rule::RaceData race("RACE_ID");
		rule::BookData book("TEST_BOOK");

		EXPECT_FALSE(race.book());
		race.setBook(book);
		EXPECT_TRUE(race.book());
		EXPECT_STREQ(race.book().value()->id().c_str(), "TEST_BOOK");
	}

	TEST(ProfessionData, EverymanSkillCategoryChoices) {
		rule::RaceData race("RACE_ID");

		// Putting the choice in a block means that we are testing an object has gone out of scope
		{
			rule::GameRuleDataChoice<rule::SkillCategoryData> choice1{};
			choice1.setNumChoices(1);

			rule::SkillCategoryData s1("SKILL1_ID");
			rule::SkillCategoryData s2("SKILL2_ID");
			rule::SkillCategoryData s3("SKILL3_ID");
			choice1.addOption(s1);
			choice1.addOption(s2);
			choice1.addOption(s3);

			race.addCategoryEverymanSkillChoice(choice1);
		}

		// choice1 is out of scope here
		for (auto& choice : race.categoryEverymanSkillChoices()) {
			EXPECT_EQ(choice.numChoices(), 1);
			EXPECT_EQ(choice.numOptions(), 3);
		}

		rule::GameRuleDataChoice<rule::SkillCategoryData> choice2{};
		choice2.setNumChoices(2);

		rule::SkillCategoryData s1a("SKILL1a_ID");
		rule::SkillCategoryData s2a("SKILL2a_ID");
		choice2.addOption(s1a);
		choice2.addOption(s2a);
		race.addCategoryEverymanSkillChoice(choice2);

		EXPECT_EQ(race.categoryEverymanSkillChoices().size(), 2);

		int count{ 0 };
		for (auto& choice : race.categoryEverymanSkillChoices()) {
			count++;
			if (choice.numChoices() == 1) {
				EXPECT_EQ(choice.numOptions(), 3);
			} else if (choice.numChoices() == 2) {
				EXPECT_EQ(choice.numOptions(), 2);
			} else {
				FAIL();
			}
		}
		EXPECT_EQ(count, 2);
	}
}