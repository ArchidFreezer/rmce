#include <gtest/gtest.h>
#include <NumberRange.h>

namespace {
	TEST(NumberTest, IntRange) {
		NumberRange nr(1, 10);
		EXPECT_TRUE(nr.matches(1));
		EXPECT_TRUE(nr.matches(7));
		EXPECT_TRUE(nr.matches(8.9));
		EXPECT_TRUE(nr.matches(10));
		EXPECT_FALSE(nr.matches(-1));
		EXPECT_FALSE(nr.matches(0));
		EXPECT_FALSE(nr.matches(11));
	}

	TEST(NumberTest, FloatRange) {
		NumberRange nr(1.0f, 10.8f);
		EXPECT_TRUE(nr.matches(1.0f));
		EXPECT_TRUE(nr.matches(7.3f));
		EXPECT_TRUE(nr.matches(10.8f));
		EXPECT_FALSE(nr.matches(-1));
		EXPECT_FALSE(nr.matches(0.99999f));
		EXPECT_FALSE(nr.matches(10.80001f));
		EXPECT_FALSE(nr.matches(11));
	}

}