#include <gtest/gtest.h>
#include <RaceData.h>

namespace {
	TEST(RaceData, LanguageAbility) {
		RaceData race("RACE_ID");

		LanguageData l1("L1_ID");
		l1.setName("L1_NAME");
		LanguageAbility la1(l1, 6, 6, 2);
		race.setStartingLanguageAbility(la1);
		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").spoken(), 6);
		EXPECT_EQ(race.startingLanguageAbility("L1_NAME").somantic(), 2);

		LanguageData l2("L2_ID");
		l2.setName("L2_NAME");
		l2.setIsSomantic(false);
		l2.setIsSpoken(true);
		l2.setIsWritten(true);
		LanguageAbility la2(l2, 4, 4, 0);

		race.setStartingLanguageAbility(la2);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isSomantic(), false);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isSpoken(), true);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").isWritten(), true);
		EXPECT_EQ(race.startingLanguageAbility("L2_NAME").written(), 4);

	}
}