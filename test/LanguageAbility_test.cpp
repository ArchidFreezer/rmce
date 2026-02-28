#include <gtest/gtest.h>

#include <LanguageAbility.h>
#include <LanguageData.h>

using namespace rm;
using namespace rm::game::character;


namespace {
	TEST(LanguageAbility, construct) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(false);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageAbility laspw(lspw);
		EXPECT_EQ(laspw.isSomantic(), false);
		EXPECT_EQ(laspw.isSpoken(), true);
		EXPECT_EQ(laspw.isWritten(), true);
		EXPECT_EQ(laspw.somantic(), 0);
		EXPECT_EQ(laspw.spoken(), 0);
		EXPECT_EQ(laspw.written(), 0);
	}

	TEST(LanguageAbility, InvalidSomantic) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(false);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageAbility laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somantic(), 0);
		EXPECT_EQ(laspw.spoken(), 1);
		EXPECT_EQ(laspw.written(), 1);

		EXPECT_THROW(laspw.updateSomanticRanks(1), InvalidLanguageRank);
		EXPECT_EQ(laspw.somantic(), 0);

		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spoken(), 2);

		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.written(), 2);
	}

	TEST(LanguageAbility, NegativeSomantic) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageAbility laspw(lspw);
		EXPECT_NO_THROW(laspw.updateSomanticRanks(1));
		EXPECT_EQ(laspw.somantic(), 1);
		EXPECT_THROW(laspw.updateSomanticRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.somantic(), 1);
	}

	TEST(LanguageAbility, InvalidSpoken) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(true);
		lspw.setIsSpoken(false);
		lspw.setIsWritten(true);

		LanguageAbility laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somantic(), 1);
		EXPECT_EQ(laspw.spoken(), 0);
		EXPECT_EQ(laspw.written(), 1);

		EXPECT_NO_THROW(laspw.updateSomanticRanks(1));
		EXPECT_EQ(laspw.somantic(), 2);

		EXPECT_THROW(laspw.updateSpokenRanks(1), InvalidLanguageRank);
		EXPECT_EQ(laspw.spoken(), 0);

		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.written(), 2);
	}

	TEST(LanguageAbility, NegativeSpoken) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageAbility laspw(lspw);
		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spoken(), 1);
		EXPECT_THROW(laspw.updateSpokenRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.spoken(), 1);
	}

	TEST(LanguageAbility, InvalidWritten) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(false);

		LanguageAbility laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somantic(), 1);
		EXPECT_EQ(laspw.spoken(), 1);
		EXPECT_EQ(laspw.written(), 0);

		EXPECT_NO_THROW(laspw.updateSomanticRanks(1));
		EXPECT_EQ(laspw.somantic(), 2);

		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spoken(), 2);

		EXPECT_THROW(laspw.updateWrittenRanks(1), InvalidLanguageRank);
		EXPECT_EQ(laspw.written(), 0);
	}

	TEST(LanguageAbility, NegativeWritten) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomantic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageAbility laspw(lspw);
		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.written(), 1);
		EXPECT_THROW(laspw.updateWrittenRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.written(), 1);
	}

}