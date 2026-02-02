#include <gtest/gtest.h>

#include <ProfessionData.h>
#include <GameRuleDataChoice.h>
#include <RealmType.h>
#include <SpellListData.h>
#include <StatType.h>

namespace {
	TEST(ProfessionData, Realms) {
		ProfessionData prof("PROF_ID");

		prof.addRealm(RealmType::kArms);
		prof.addRealm(RealmType::kChanneling);

		EXPECT_TRUE(prof.isRealm(RealmType::kArms));
		EXPECT_TRUE(prof.isRealm(RealmType::kChanneling));
		EXPECT_FALSE(prof.isRealm(RealmType::kArcane));
		EXPECT_FALSE(prof.isRealm(RealmType::kEssence));
		EXPECT_FALSE(prof.isRealm(RealmType::kMentalism));

		EXPECT_EQ(prof.realms().size(), 2);

	}

	TEST(ProfessionData, Stats) {
		ProfessionData prof("PROF_ID");

		prof.addStat(StatType::kAgility);
		prof.addStat(StatType::kReasoning);

		EXPECT_EQ(prof.stats().size(), 2);
		prof.clearStats();
		EXPECT_EQ(prof.stats().size(), 0);

		prof.addStat(StatType::kAgility);
		prof.addStat(StatType::kReasoning);

		for (StatType::Type stat : prof.stats()) {
			if (stat != StatType::kAgility && stat != StatType::kReasoning) FAIL();
		}

	}

	TEST(ProfessionData, BaseLists) {
		ProfessionData prof("PROF_ID");

		// Putting the choice in a block means that we are testing an object has gone out of scope
		{
			GameRuleDataChoice<SpellListData> choice1{};
			SpellListData sl1("List1");
			SpellListData sl2("List2");
			SpellListData sl3("List3");
			SpellListData sl4("List4");
			choice1.setNumChoices(1);
			choice1.addOption(sl1);
			choice1.addOption(sl2);
			choice1.addOption(sl3);
			choice1.addOption(sl4);
			prof.addBaseSpellListChoice(std::move(choice1));
		}

		// choice1 is out of scope here
		for (auto& choice : prof.baseSpellListChoices()) {
			EXPECT_EQ(choice.numChoices(), 1);
			EXPECT_EQ(choice.numOptions(), 4);
		}

		GameRuleDataChoice<SpellListData> choice2{};
		choice2.setNumChoices(2);
		SpellListData sl1a("List1a");
		SpellListData sl2a("List2a");
		choice2.addOption(sl1a);
		choice2.addOption(sl2a);
		prof.addBaseSpellListChoice(std::move(choice2));

		EXPECT_EQ(prof.baseSpellListChoices().size(), 2);

		int count{ 0 };
		for (auto& choice : prof.baseSpellListChoices()) {
			count++;
			if (choice.numChoices() == 1) {
				EXPECT_EQ(choice.numOptions(), 4);
			} else if (choice.numChoices() == 2) {
				EXPECT_EQ(choice.numOptions(), 2);
			} else {
				FAIL();
			}
		}
		EXPECT_EQ(count, 2);
	}

	TEST(ProfessionData, EverymanSkills) {
		ProfessionData prof("PROF_ID");

		SkillData sk1("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		SubcategoriedSkillData s1(sk1, "Sub");
		SubcategoriedSkillData s2(sk2, "Sub");
		EXPECT_STREQ(s1.id().c_str(), "SKILL1_ID_Sub");

		prof.addEverymanSkill(std::move(s1));
		prof.addEverymanSkill(std::move(s2));

		const std::set<SubcategoriedSkillData> eset(prof.everymanSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<SubcategoriedSkillData> eset2(prof.everymanSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : prof.everymanSkills()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SubcategoriedSkillData s2b(sk2, "Sub");
		EXPECT_FALSE(prof.isEverymanSkill(sk2));
		EXPECT_TRUE(prof.isEverymanSkill(sk2, "Sub"));
		EXPECT_THROW(prof.addEverymanSkill(std::move(s2b)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(prof.isEverymanSkill(sk3));

		SkillData sk4("SKILL4_ID");
		SubcategoriedSkillData s4(sk4, "");
		prof.addOccupationalSkill(std::move(s4));
		SubcategoriedSkillData s5(sk4, "");
		EXPECT_THROW(prof.addEverymanSkill(std::move(s5)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk5("SKILL5_ID");
		SubcategoriedSkillData s6(sk5, "");
		prof.addRestrictedSkill(std::move(s6));
		SubcategoriedSkillData s7(sk5, "");
		EXPECT_THROW(prof.addEverymanSkill(std::move(s7)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk6("SKILL6_ID");
		SubcategoriedSkillData s8(sk6, "");
		prof.addStandardSkill(std::move(s8));
		SubcategoriedSkillData s9(sk6, "");
		EXPECT_THROW(prof.addEverymanSkill(std::move(s9)), ProfessionData::InvalidSkillDevelopment);
	}

	TEST(ProfessionData, OccupationalSkills) {
		ProfessionData prof("PROF_ID");

		SkillData sk1("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		SubcategoriedSkillData s1(sk1, "Sub");
		SubcategoriedSkillData s2(sk2, "Sub");
		EXPECT_STREQ(s1.id().c_str(), "SKILL1_ID_Sub");

		prof.addOccupationalSkill(std::move(s1));
		prof.addOccupationalSkill(std::move(s2));

		const std::set<SubcategoriedSkillData> eset(prof.occupationalSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<SubcategoriedSkillData> eset2(prof.occupationalSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : prof.occupationalSkills()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SubcategoriedSkillData s2b(sk2, "Sub");
		EXPECT_FALSE(prof.isOccupationalSkill(sk2));
		EXPECT_TRUE(prof.isOccupationalSkill(sk2, "Sub"));
		EXPECT_THROW(prof.addOccupationalSkill(std::move(s2b)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(prof.isOccupationalSkill(sk3));

		SkillData sk4("SKILL4_ID");
		SubcategoriedSkillData s4(sk4, "");
		prof.addRestrictedSkill(std::move(s4));
		SubcategoriedSkillData s5(sk4, "");
		EXPECT_THROW(prof.addOccupationalSkill(std::move(s5)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk5("SKILL5_ID");
		SubcategoriedSkillData s6(sk5, "");
		prof.addEverymanSkill(std::move(s6));
		SubcategoriedSkillData s7(sk5, "");
		EXPECT_THROW(prof.addOccupationalSkill(std::move(s7)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk6("SKILL6_ID");
		SubcategoriedSkillData s8(sk6, "");
		prof.addStandardSkill(std::move(s8));
		SubcategoriedSkillData s9(sk6, "");
		EXPECT_THROW(prof.addOccupationalSkill(std::move(s9)), ProfessionData::InvalidSkillDevelopment);
	}

	TEST(ProfessionData, RestrictedSkills) {
		ProfessionData prof("PROF_ID");

		SkillData sk1("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		SubcategoriedSkillData s1(sk1, "Sub");
		SubcategoriedSkillData s2(sk2, "Sub");

		prof.addRestrictedSkill(std::move(s1));
		prof.addRestrictedSkill(std::move(s2));

		const std::set<SubcategoriedSkillData> eset(prof.restrictedSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<SubcategoriedSkillData> eset2(prof.restrictedSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : prof.restrictedSkills()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillData sk2b("SKILL2_ID");
		SubcategoriedSkillData s2b(sk2b, "Sub");
		EXPECT_FALSE(prof.isRestrictedSkill(sk2b));
		EXPECT_TRUE(prof.isRestrictedSkill(sk2b, "Sub"));
		EXPECT_THROW(prof.addRestrictedSkill(std::move(s2b)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(prof.isRestrictedSkill(sk3));

		SkillData sk4("SKILL4_ID");
		SubcategoriedSkillData s4(sk4, "");
		prof.addEverymanSkill(std::move(s4));
		SubcategoriedSkillData s5(sk4, "");
		EXPECT_THROW(prof.addRestrictedSkill(std::move(s5)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk5("SKILL5_ID");
		SubcategoriedSkillData s6(sk5, "");
		prof.addOccupationalSkill(std::move(s6));
		SubcategoriedSkillData s7(sk5, "");
		EXPECT_THROW(prof.addRestrictedSkill(std::move(s7)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk6("SKILL6_ID");
		SubcategoriedSkillData s8(sk6, "");
		prof.addStandardSkill(std::move(s8));
		SubcategoriedSkillData s9(sk6, "");
		EXPECT_THROW(prof.addRestrictedSkill(std::move(s9)), ProfessionData::InvalidSkillDevelopment);
	}


	TEST(ProfessionData, StandardSkills) {
		ProfessionData prof("PROF_ID");

		SkillData sk1("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		SubcategoriedSkillData s1(sk1, "Sub");
		SubcategoriedSkillData s2(sk2, "Sub");

		prof.addStandardSkill(std::move(s1));
		prof.addStandardSkill(std::move(s2));

		const std::set<SubcategoriedSkillData> eset(prof.standardSkills());
		EXPECT_EQ(eset.size(), 2);

		const std::set<SubcategoriedSkillData> eset2(prof.standardSkills());
		EXPECT_EQ(eset2.size(), 2);

		int count{ 0 };
		for (auto& skill : prof.standardSkills()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);

		SkillData sk2b("SKILL2_ID");
		SubcategoriedSkillData s2b(sk2b, "Sub");
		EXPECT_FALSE(prof.isStandardSkill(sk2b));
		EXPECT_TRUE(prof.isStandardSkill(sk2b, "Sub"));
		EXPECT_THROW(prof.addStandardSkill(std::move(s2b)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk3("SKILL3_ID");
		EXPECT_FALSE(prof.isStandardSkill(sk3));

		SkillData sk4("SKILL4_ID");
		SubcategoriedSkillData s4(sk4, "");
		prof.addEverymanSkill(std::move(s4));
		SubcategoriedSkillData s5(sk4, "");
		EXPECT_THROW(prof.addStandardSkill(std::move(s5)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk5("SKILL5_ID");
		SubcategoriedSkillData s6(sk5, "");
		prof.addOccupationalSkill(std::move(s6));
		SubcategoriedSkillData s7(sk5, "");
		EXPECT_THROW(prof.addStandardSkill(std::move(s7)), ProfessionData::InvalidSkillDevelopment);

		SkillData sk6("SKILL6_ID");
		SubcategoriedSkillData s8(sk6, "");
		prof.addRestrictedSkill(std::move(s8));
		SubcategoriedSkillData s9(sk6, "");
		EXPECT_THROW(prof.addStandardSkill(std::move(s9)), ProfessionData::InvalidSkillDevelopment);
	}

	TEST(ProfessionData, EverymanSkillChoices) {
		ProfessionData prof("PROF_ID");

		// Putting the choice in a block means that we are testing an object has gone out of scope
		{
			GameRuleDataChoice<SkillData> choice1{};
			choice1.setNumChoices(1);

			SkillData s1("SKILL1_ID");
			SkillData s2("SKILL2_ID");
			SkillData s3("SKILL3_ID");
			choice1.addOption(s1);
			choice1.addOption(s2);
			choice1.addOption(s3);

			prof.addEverymanSkillChoice(choice1);
		}

		// choice1 is out of scope here
		for (auto& choice : prof.everymanSkillChoices()) {
			EXPECT_EQ(choice.numChoices(), 1);
			EXPECT_EQ(choice.numOptions(), 3);
		}

		GameRuleDataChoice<SkillData> choice2{};
		choice2.setNumChoices(2);

		SkillData s1a("SKILL1a_ID");
		SkillData s2a("SKILL2a_ID");
		choice2.addOption(s1a);
		choice2.addOption(s2a);
		prof.addEverymanSkillChoice(choice2);

		EXPECT_EQ(prof.everymanSkillChoices().size(), 2);

		int count{ 0 };
		for (auto& choice : prof.everymanSkillChoices()) {
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

	TEST(ProfessionData, EverymanSkillCategoryChoices) {
		ProfessionData prof("PROF_ID");

		// Putting the choice in a block means that we are testing an object has gone out of scope
		{
			GameRuleDataChoice<SkillCategoryData> choice1{};
			choice1.setNumChoices(1);

			SkillCategoryData s1("SKILL1_ID");
			SkillCategoryData s2("SKILL2_ID");
			SkillCategoryData s3("SKILL3_ID");
			choice1.addOption(s1);
			choice1.addOption(s2);
			choice1.addOption(s3);

			prof.addCategoryEverymanSkillChoice(choice1);
		}

		// choice1 is out of scope here
		for (auto& choice : prof.categoryEverymanSkillChoices()) {
			EXPECT_EQ(choice.numChoices(), 1);
			EXPECT_EQ(choice.numOptions(), 3);
		}

		GameRuleDataChoice<SkillCategoryData> choice2{};
		choice2.setNumChoices(2);

		SkillCategoryData s1a("SKILL1a_ID");
		SkillCategoryData s2a("SKILL2a_ID");
		choice2.addOption(s1a);
		choice2.addOption(s2a);
		prof.addCategoryEverymanSkillChoice(choice2);

		EXPECT_EQ(prof.categoryEverymanSkillChoices().size(), 2);

		int count{ 0 };
		for (auto& choice : prof.categoryEverymanSkillChoices()) {
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

	TEST(ProfessionData, GroupBonus) {
		ProfessionData prof("PROF_ID");

		EXPECT_EQ(prof.skillGroupsWithBonus().size(), 0);
		EXPECT_FALSE(prof.isBonusSkillGroup("Group"));
		EXPECT_EQ(prof.skillGroupBonus("Group"), 0);

		prof.addSkillGroupBonus("Group1", 10);
		EXPECT_EQ(prof.skillGroupsWithBonus().size(), 1);
		EXPECT_FALSE(prof.isBonusSkillGroup("Group"));
		EXPECT_TRUE(prof.isBonusSkillGroup("Group1"));
		EXPECT_EQ(prof.skillGroupBonus("Group1"), 10);

		prof.addSkillGroupBonus("Group2", 5);
		EXPECT_EQ(prof.skillGroupsWithBonus().size(), 2);
		EXPECT_TRUE(prof.isBonusSkillGroup("Group1"));
		EXPECT_EQ(prof.skillGroupBonus("Group1"), 10);
		EXPECT_TRUE(prof.isBonusSkillGroup("Group2"));
		EXPECT_EQ(prof.skillGroupBonus("Group2"), 5);
	}

	TEST(ProfessionData, GroupSkillDevelopmentType) {
		ProfessionData prof("PROF_ID");

		EXPECT_EQ(prof.skillGroupsWithSkillDevelopmentType().size(), 0);
		EXPECT_FALSE(prof.isSkillDevelopmentTypeSkillGroup("Group"));
		EXPECT_EQ(prof.skillGroupSkillDevelopmentType("Group"), SkillDevelopmentType::kStandard);

		prof.addSkillGroupSkillDevelopmentType("Group1", SkillDevelopmentType::kEveryman);
		EXPECT_EQ(prof.skillGroupsWithSkillDevelopmentType().size(), 1);
		EXPECT_FALSE(prof.isSkillDevelopmentTypeSkillGroup("Group"));
		EXPECT_TRUE(prof.isSkillDevelopmentTypeSkillGroup("Group1"));
		EXPECT_EQ(prof.skillGroupSkillDevelopmentType("Group1"), SkillDevelopmentType::kEveryman);

		prof.addSkillGroupSkillDevelopmentType("Group2", SkillDevelopmentType::kOccupational);
		EXPECT_EQ(prof.skillGroupsWithSkillDevelopmentType().size(), 2);
		EXPECT_TRUE(prof.isSkillDevelopmentTypeSkillGroup("Group1"));
		EXPECT_EQ(prof.skillGroupSkillDevelopmentType("Group1"), SkillDevelopmentType::kEveryman);
		EXPECT_TRUE(prof.isSkillDevelopmentTypeSkillGroup("Group2"));
		EXPECT_EQ(prof.skillGroupSkillDevelopmentType("Group2"), SkillDevelopmentType::kOccupational);
	}

	TEST(ProfessionData, CategoryBonus) {
		ProfessionData prof("PROF_ID");

		SkillCategoryData c1("CAT1_ID");
		EXPECT_EQ(prof.skillCategoriesWithBonus().size(), 0);
		EXPECT_FALSE(prof.isBonusSkillCategory(c1));
		EXPECT_EQ(prof.skillCategoryBonus(c1), 0);

		prof.addSkillCategoryBonus(c1, 10);
		EXPECT_TRUE(prof.isBonusSkillCategory(c1));
		
		SkillCategoryData c1a("CAT1_ID");
		EXPECT_TRUE(prof.isBonusSkillCategory(c1a));

	}

	TEST(ProfessionData, SkillBonus) {
		ProfessionData prof("PROF_ID");

		SkillData sk1("SKILL1_ID");
		SkillData sk1a("SKILL1_ID");
		SkillData sk2("SKILL2_ID");
		SubcategoriedSkillData s1(sk1, "Sub");
		SubcategoriedSkillData s1a(sk1a, "Sub");
		SubcategoriedSkillData s2(sk2, "Sub");
		SubcategoriedSkillData s2a(sk2, "Sub");

		prof.setSkillBonus(std::move(s1), 5);
		prof.setSkillBonus(std::move(s2), 10);

		EXPECT_TRUE(prof.isBonusSkill(sk1a, "Sub"));

		EXPECT_EQ(prof.skillBonus(s1a), 5);
		EXPECT_FALSE(prof.isBonusSkill(sk1));
		EXPECT_FALSE(prof.isBonusSkill(sk1, "Invalid"));
		EXPECT_TRUE(prof.isBonusSkill(sk1, "Sub"));

		EXPECT_THROW(prof.setSkillBonus(std::move(s1a), 15), ProfessionData::InvalidSkillBonus);
		EXPECT_EQ(prof.skillBonus(s2a), 10);

		EXPECT_EQ(prof.skillBonus(sk2, "Sub"), 10);
		EXPECT_EQ(prof.skillBonus(sk2), 0);

		int count{ 0 };
		for (auto& skill : prof.skillsWithBonus()) {
			count++;
			if (skill.skillData().id() == "SKILL1_ID" || skill.skillData().id() == "SKILL2_ID") continue;
			FAIL();
		}
		EXPECT_EQ(count, 2);
	}

	TEST(ProfessionData, CategoryCosts) {
		ProfessionData prof("PROF_ID");
		SkillCategoryData c1("CAT1_ID");
		EXPECT_THROW(prof.categoryDevelopmentCost(c1), ProfessionData::InvalidCategoryDevelopmentCost);

		prof.addCategoryDevelopmentCost(c1, SkillDevelopmentCost("3:4"));
		EXPECT_EQ(prof.categoryDevelopmentCost(c1).first(), 3);
		EXPECT_EQ(prof.categoryDevelopmentCost(c1).second(), 4);
		EXPECT_EQ(prof.categoryDevelopmentCost(c1).third(), std::nullopt);

		SkillCategoryData c2("CAT2_ID");
		{
			SkillDevelopmentCost dc1("12");
			prof.addCategoryDevelopmentCost(c2, dc1);
			EXPECT_EQ(prof.categoryDevelopmentCost(c1).first(), 3);
			EXPECT_EQ(prof.categoryDevelopmentCost(c2).first(), 12);
			EXPECT_EQ(prof.categoryDevelopmentCost(c2).second(), std::nullopt);
		}
		EXPECT_EQ(prof.categoryDevelopmentCost(c2).first(), 12);
	}
}