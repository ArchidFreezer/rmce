#include <gtest/gtest.h>

#include <StringUtils.h>

namespace {
	TEST(StringUtils, Lcase) {
		EXPECT_STREQ(archid::lcase("This > IS a str12NG").c_str(), "this > is a str12ng");
	}

	TEST(StringUtils, LcaseConcat) {
		EXPECT_STREQ(archid::lcaseconcat("This > IS a str-12NG").c_str(), "thisisastr12ng");
	}

}