#include <gtest/gtest.h>

#include <LanguageAbility.h>
#include <LanguageData.h>

namespace {
	LanguageData getLanguage(std::string id, std::string name, bool somantic, bool spoken, bool written) {
		LanguageData lang(id);
		lang.setName(name);
		lang.setIsSomantic(somantic);
		lang.setIsSpoken(spoken);
		lang.setIsWritten(written);
		return lang;
	}

	TEST(LanguageAbility, construct) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", false, true, true);

		LanguageAbility laspw(lspw);
		EXPECT_EQ(laspw.isSomantic(), false);
		EXPECT_EQ(laspw.isSpoken(), true);
		EXPECT_EQ(laspw.isWritten(), true);
		EXPECT_EQ(laspw.somantic(), 0);
		EXPECT_EQ(laspw.spoken(), 0);
		EXPECT_EQ(laspw.written(), 0);
	}

	TEST(LanguageAbility, InvalidSomantic) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", false, true, true);

		LanguageAbility laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somantic(), 0);
		EXPECT_EQ(laspw.spoken(), 1);
		EXPECT_EQ(laspw.written(), 1);

		try {
			laspw.updateSomanticRanks(1);
			FAIL();
		} catch (InvalidLanguageRank e) {
			SUCCEED();
		}

		try {
			laspw.updateSpokenRanks(1);
			SUCCEED();
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.spoken(), 2);

		try {
			laspw.updateWrittenRanks(1);
			SUCCEED();
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.written(), 2);
	}

	TEST(LanguageAbility, NegativeSomantic) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, true);

		LanguageAbility laspw(lspw);
		try {
			laspw.updateSomanticRanks(1);
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.somantic(), 1);
		try {
			laspw.updateSomanticRanks(-2);
			FAIL();
		} catch (InvalidLanguageRank e) {
			SUCCEED();
		}
	}

	TEST(LanguageAbility, InvalidSpoken) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, false, true);

		LanguageAbility laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somantic(), 1);
		EXPECT_EQ(laspw.spoken(), 0);
		EXPECT_EQ(laspw.written(), 1);

		try {
			laspw.updateSomanticRanks(1);
			SUCCEED();
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.somantic(), 2);

		try {
			laspw.updateSpokenRanks(1);
			FAIL();
		} catch (InvalidLanguageRank e) {
			SUCCEED();
		}

		try {
			laspw.updateWrittenRanks(1);
			SUCCEED();
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.written(), 2);
	}

	TEST(LanguageAbility, NegativeSpoken) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, true);

		LanguageAbility laspw(lspw);
		try {
			laspw.updateSpokenRanks(1);
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.spoken(), 1);
		try {
			laspw.updateSpokenRanks(-2);
			FAIL();
		} catch (InvalidLanguageRank e) {
			SUCCEED();
		}
	}

	TEST(LanguageAbility, InvalidWritten) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, false);

		LanguageAbility laspw(lspw, 1, 1, 1);
		EXPECT_EQ(laspw.somantic(), 1);
		EXPECT_EQ(laspw.spoken(), 1);
		EXPECT_EQ(laspw.written(), 0);

		try {
			laspw.updateSomanticRanks(1);
			SUCCEED();
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.somantic(), 2);

		try {
			laspw.updateSpokenRanks(1);
			SUCCEED();
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.spoken(), 2);

		try {
			laspw.updateWrittenRanks(1);
			FAIL();
		} catch (InvalidLanguageRank e) {
			SUCCEED();
		}
	}

	TEST(LanguageAbility, NegativeWritten) {
		LanguageData lspw = getLanguage("LSPW_ID", "LSPW", true, true, true);

		LanguageAbility laspw(lspw);
		try {
			laspw.updateWrittenRanks(1);
		} catch (InvalidLanguageRank e) {
			FAIL();
		}
		EXPECT_EQ(laspw.written(), 1);
		try {
			laspw.updateWrittenRanks(-2);
			FAIL();
		} catch (InvalidLanguageRank e) {
			SUCCEED();
		}
	}

}