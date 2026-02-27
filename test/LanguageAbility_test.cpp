#include <gtest/gtest.h>

#include <LanguageAbility.h>
#include <LanguageData.h>

using namespace rm;
using rm::game::character::LanguageAbility;
using rm::game::character::InvalidLanguageRank;


namespace {
	rule::LanguageData getLanguage(std::string id, std::string name, bool somantic, bool spoken, bool written) {
		rule::LanguageData lang(id);
		lang.setName(name);
		lang.setIsSomantic(somantic);
		lang.setIsSpoken(spoken);
		lang.setIsWritten(written);
		return lang;
	}

	TEST(LanguageAbility, construct) {
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", false, true, true);

		LanguageAbility laspw(lspw);
		EXPECT_EQ(laspw.isSomantic(), false);
		EXPECT_EQ(laspw.isSpoken(), true);
		EXPECT_EQ(laspw.isWritten(), true);
		EXPECT_EQ(laspw.somantic(), 0);
		EXPECT_EQ(laspw.spoken(), 0);
		EXPECT_EQ(laspw.written(), 0);
	}

	TEST(LanguageAbility, InvalidSomantic) {
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", false, true, true);

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
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, true);

		LanguageAbility laspw(lspw);
		EXPECT_NO_THROW(laspw.updateSomanticRanks(1));
		EXPECT_EQ(laspw.somantic(), 1);
		EXPECT_THROW(laspw.updateSomanticRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.somantic(), 1);
	}

	TEST(LanguageAbility, InvalidSpoken) {
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, false, true);

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
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, true);

		LanguageAbility laspw(lspw);
		EXPECT_NO_THROW(laspw.updateSpokenRanks(1));
		EXPECT_EQ(laspw.spoken(), 1);
		EXPECT_THROW(laspw.updateSpokenRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.spoken(), 1);
	}

	TEST(LanguageAbility, InvalidWritten) {
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, false);

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
		rule::LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, true);

		LanguageAbility laspw(lspw);
		EXPECT_NO_THROW(laspw.updateWrittenRanks(1));
		EXPECT_EQ(laspw.written(), 1);
		EXPECT_THROW(laspw.updateWrittenRanks(-2), InvalidLanguageRank);
		EXPECT_EQ(laspw.written(), 1);
	}

}