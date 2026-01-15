#include <gtest/gtest.h>
#include <Matcher.h>
#include <NumberRange.h>

namespace {
	TEST(NumberRange, IntRange) {
		NumberRange nr(1, 10);
		EXPECT_TRUE(nr.matches(1));
		EXPECT_TRUE(nr.matches(7));
		EXPECT_TRUE(nr.matches(8.9));
		EXPECT_TRUE(nr.matches(10));
		EXPECT_FALSE(nr.matches(-1));
		EXPECT_FALSE(nr.matches(0));
		EXPECT_FALSE(nr.matches(11));
	}

	TEST(NumberRange, FloatRange) {
		NumberRange nr(1.0f, 10.8f);
		EXPECT_TRUE(nr.matches(1.0f));
		EXPECT_TRUE(nr.matches(7.3f));
		EXPECT_TRUE(nr.matches(10.8f));
		EXPECT_FALSE(nr.matches(-1));
		EXPECT_FALSE(nr.matches(0.99999f));
		EXPECT_FALSE(nr.matches(10.80001f));
		EXPECT_FALSE(nr.matches(11));
	}

	template<typename T, typename U>
		requires is_matcher<T, U>
	class NumberRangeTest {
	public:
		NumberRangeTest(T matcher, U val) {};
	};

	TEST(NumberRange, is_matcher) {
		NumberRange nr(1, 10);
		// The line below will fail to compile if NumberRange does not adhere to is_matcher concept
		NumberRangeTest<NumberRange<int>, int>(nr, 1);
	}

}