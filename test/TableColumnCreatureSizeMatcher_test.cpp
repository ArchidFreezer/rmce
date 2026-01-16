#include <gtest/gtest.h>

#include <table/TableColumnCreatureSizeMatcher.h>

namespace {
	TEST(TableColumnCreatureSizeMatcher, All) {
		TableColumnCreatureSizeMatcher matcher = TableColumnCreatureSizeMatcher();
		// Check the default
		EXPECT_EQ(matcher.getColumn(), 0);
		EXPECT_EQ(matcher.setMatch(CreatureSizeType::kTiny).getColumn(), 0);
		EXPECT_EQ(matcher.setMatch(CreatureSizeType::kSmall).getColumn(), 1);
		EXPECT_EQ(matcher.setMatch(CreatureSizeType::kMedium).getColumn(), 2);
		EXPECT_EQ(matcher.setMatch(CreatureSizeType::kLarge).getColumn(), 3);
		EXPECT_EQ(matcher.setMatch(CreatureSizeType::kSuperLarge).getColumn(), 4);
	}
}