#include <gtest/gtest.h>

#include <TrainingPackageData.h>

using namespace rm;

namespace {
	TEST(TrainingPackageData, General) {
		std::string id{ "TP1_ID" };
		rule::TrainingPackageData tp1(id);
		EXPECT_STREQ(tp1.id().c_str(), id.c_str());

		std::string name{ "TP1 Name" };
		tp1.setName(name);
		EXPECT_STREQ(tp1.name().c_str(), name.c_str());

		rule::BookData book("BOOK_ID");
		tp1.setBook(book);
		EXPECT_STREQ(tp1.book().id().c_str(), book.id().c_str());

		// Races
		rule::RaceData r1("RACE1_ID");
		rule::RaceData r2("RACE2_ID");
		EXPECT_TRUE(tp1.available(r1));
		EXPECT_TRUE(tp1.available(r2));
		tp1.addRace(r1);
		EXPECT_TRUE(tp1.available(r1));
		EXPECT_FALSE(tp1.available(r2));
		EXPECT_EQ(tp1.races().size(), 1);
		tp1.addRace(r2);
		EXPECT_EQ(tp1.races().size(), 2);

		// Qualifiers
		std::string q10{ "q10" };
		tp1.addQualifier(q10, 10);
		std::string q15{ "q15" };
		tp1.addQualifier(q15, 15);

		EXPECT_EQ(tp1.qualifiers().size(), 2);
		for (const auto& key : tp1.qualifiers()) {
			if (key.first == "q10") {
				EXPECT_EQ(key.second, 10);
			} else if (key.first == "q15") {
				EXPECT_EQ(key.second, 15);
			} else {
				FAIL();
			}
		}

		tp1.setLifestyle(false);
		EXPECT_FALSE(tp1.lifestyle());
		tp1.setLifestyle(true);
		EXPECT_TRUE(tp1.lifestyle());

		tp1.setTimeToAcquire(15);
		EXPECT_EQ(tp1.timeToAcquire(), 15);

		// Starting money positive
		tp1.setStaringMoneyModifierDice("2d10");
		int max{ 0 };
		for (int x{ 0 }; x < 25; x++) {
			int change = tp1.startingMoneyChange();
			max = std::max(max, change);
			EXPECT_GT(change, 0);
		}
		EXPECT_GT(max, 10);

		// Starting money negative
		tp1.setStaringMoneyModifierDice("-d10");
		int min{ 0 };
		for (int x{ 0 }; x < 25; x++) {
			int change = tp1.startingMoneyChange();
			min = std::min(min, change);
			EXPECT_LT(change, 0);
		}
		EXPECT_LT(min, -10);

		// Stat gains
		EXPECT_TRUE(tp1.statGains().empty());
		tp1.addStatGain(rule::enums::StatType::kAgility);
		EXPECT_EQ(tp1.statGains().size(), 1);
		tp1.addStatGain(rule::enums::StatType::kConstitution);
		EXPECT_EQ(tp1.statGains().size(), 2);
		for (const auto& stat : tp1.statGains()) {
			if (stat == rule::enums::StatType::kAgility) SUCCEED();
			else if (stat == rule::enums::StatType::kConstitution) SUCCEED();
			else FAIL();
		}
	}

	TEST(TrainingPackageData, SkillRanks) {
		rule::TrainingPackageData tp("TP_ID");

		rule::SkillData s1("SKILL1_ID");
		rule::SubcategoriedSkillData sd1(s1);
		std::string sd1_id = sd1.id();

		EXPECT_EQ(tp.skillRank(sd1), 0);
		tp.addSkillRank(sd1, 10);
		EXPECT_EQ(tp.skillRank(s1), 10);
		EXPECT_EQ(tp.skillRank(sd1), 10);

		rule::SkillData s2("SKILL2_ID");
		rule::SubcategoriedSkillData sd2(s2, "Sub2");
		std::string sd2_id = sd2.id();
		EXPECT_FALSE(tp.isRankSkill(s2));
		tp.addSkillRank(sd2, 15);
		EXPECT_EQ(tp.skillRank(s2), 0);
		EXPECT_EQ(tp.skillRank(s2, "Sub2"), 15);
		EXPECT_EQ(tp.skillRank(sd2), 15);
		EXPECT_FALSE(tp.isRankSkill(s2));
		EXPECT_TRUE(tp.isRankSkill(s2, "Sub2"));

		EXPECT_EQ(tp.skillsWithRanks().size(), 2);

		for (const auto& sc : tp.skillsWithRanks()) {
			if (sc->id() == sd1_id) SUCCEED();
			else if (sc->id() == sd2_id) SUCCEED();
			else FAIL();
		}
	}
}