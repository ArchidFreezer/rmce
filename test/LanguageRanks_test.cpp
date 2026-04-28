#include <gtest/gtest.h>

#include <LanguageRanks.h>
#include <LanguageData.h>

using namespace rm;
using namespace rm::game::character;


namespace {
	TEST(LanguageRanks, construct) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(false);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageRanks laspw(lspw);
		EXPECT_EQ(laspw.isSomatic(), false);
		EXPECT_EQ(laspw.isSpoken(), true);
		EXPECT_EQ(laspw.isWritten(), true);
		EXPECT_EQ(laspw.somaticRanks(), 0);
		EXPECT_EQ(laspw.spokenRanks(), 0);
		EXPECT_EQ(laspw.writtenRanks(), 0);
	}

	TEST(LanguageRanks, InvalidSomantic) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(false);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageRanks laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somaticRanks(), 0);
		EXPECT_EQ(laspw.spokenRanks(), 1);
		EXPECT_EQ(laspw.writtenRanks(), 1);

		EXPECT_THROW(laspw.updateSomaticRanks(1), InvalidLanguageRank);
		EXPECT_EQ(laspw.somaticRanks(), 0);

		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spokenRanks(), 2);

		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.writtenRanks(), 2);
	}

	TEST(LanguageRanks, NegativeSomantic) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageRanks laspw(lspw);
		EXPECT_NO_THROW(laspw.updateSomaticRanks(1));
		EXPECT_EQ(laspw.somaticRanks(), 1);
		EXPECT_THROW(laspw.updateSomaticRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.somaticRanks(), 1);
	}

	TEST(LanguageRanks, InvalidSpoken) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(true);
		lspw.setIsSpoken(false);
		lspw.setIsWritten(true);

		LanguageRanks laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somaticRanks(), 1);
		EXPECT_EQ(laspw.spokenRanks(), 0);
		EXPECT_EQ(laspw.writtenRanks(), 1);

		EXPECT_NO_THROW(laspw.updateSomaticRanks(1));
		EXPECT_EQ(laspw.somaticRanks(), 2);

		EXPECT_THROW(laspw.updateSpokenRanks(1), InvalidLanguageRank);
		EXPECT_EQ(laspw.spokenRanks(), 0);

		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.writtenRanks(), 2);
	}

	TEST(LanguageRanks, NegativeSpoken) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageRanks laspw(lspw);
		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spokenRanks(), 1);
		EXPECT_THROW(laspw.updateSpokenRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.spokenRanks(), 1);
	}

	TEST(LanguageRanks, InvalidWritten) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(false);

		LanguageRanks laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somaticRanks(), 1);
		EXPECT_EQ(laspw.spokenRanks(), 1);
		EXPECT_EQ(laspw.writtenRanks(), 0);

		EXPECT_NO_THROW(laspw.updateSomaticRanks(1));
		EXPECT_EQ(laspw.somaticRanks(), 2);

		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spokenRanks(), 2);

		EXPECT_THROW(laspw.updateWrittenRanks(1), InvalidLanguageRank);
		EXPECT_EQ(laspw.writtenRanks(), 0);
	}

	TEST(LanguageRanks, NegativeWritten) {
		rule::LanguageData lspw("LSPW_ID");
		lspw.setName("LSPW");
		lspw.setIsSomatic(true);
		lspw.setIsSpoken(true);
		lspw.setIsWritten(true);

		LanguageRanks laspw(lspw);
		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.writtenRanks(), 1);
		EXPECT_THROW(laspw.updateWrittenRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.writtenRanks(), 1);
	}

}