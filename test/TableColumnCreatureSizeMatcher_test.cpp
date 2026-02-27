#include <gtest/gtest.h>

#include <table/TableColumnCreatureSizeMatcher.h>

using namespace rm;

namespace {
	TEST(TableColumnCreatureSizeMatcher, All) {
		TableColumnCreatureSizeMatcher matcher = TableColumnCreatureSizeMatcher();
		// Check the default
		EXPECT_EQ(matcher.column(CreatureSizeType::kTiny), 0);
		EXPECT_EQ(matcher.column(CreatureSizeType::kSmall), 1);
		EXPECT_EQ(matcher.column(CreatureSizeType::kMedium), 2);
		EXPECT_EQ(matcher.column(CreatureSizeType::kLarge), 3);
		EXPECT_EQ(matcher.column(CreatureSizeType::kHuge), 4);
	}
}