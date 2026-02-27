#include <gtest/gtest.h>
#include <table/TableRowMatcher.h>
#include <NumberRange.h>

namespace {
	TEST(NumberRange, IntRange) {
		archid::NumberRange nr(1, 10);
		EXPECT_TRUE(nr.matches(1));
		EXPECT_TRUE(nr.matches(7));
		EXPECT_TRUE(nr.matches(8.9));
		EXPECT_TRUE(nr.matches(10));
		EXPECT_FALSE(nr.matches(-1));
		EXPECT_FALSE(nr.matches(0));
		EXPECT_FALSE(nr.matches(11));
	}

	TEST(NumberRange, FloatRange) {
		archid::NumberRange nr(1.0f, 10.8f);
		EXPECT_TRUE(nr.matches(1.0f));
		EXPECT_TRUE(nr.matches(7.3f));
		EXPECT_TRUE(nr.matches(10.8f));
		EXPECT_FALSE(nr.matches(-1));
		EXPECT_FALSE(nr.matches(0.99999f));
		EXPECT_FALSE(nr.matches(10.80001f));
		EXPECT_FALSE(nr.matches(11));
	}

	template<typename MatcherClass, typename MatcherDatatype>
		requires table_row_matcher<MatcherClass, MatcherDatatype>
	class NumberRangeTest {
	public:
		NumberRangeTest(MatcherClass matcher, MatcherDatatype val) {};
	};

	TEST(NumberRange, table_row_matcher) {
		archid::NumberRange nr(1, 10);
		// The line below will fail to compile if NumberRange does not adhere to table_row_matcher concept
		NumberRangeTest<archid::NumberRange<int>, int>(nr, 1);
	}

}