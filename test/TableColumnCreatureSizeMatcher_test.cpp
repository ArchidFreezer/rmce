#include <gtest/gtest.h>

#include <table/TableColumnCreatureSizeMatcher.h>

namespace {
	TEST(TableColumnCreatureSizeMatcher, All) {
		TableColumnCreatureSizeMatcher matcher = TableColumnCreatureSizeMatcher();
		// Check the default
		EXPECT_EQ(matcher.getColumn(CreatureSizeType::kTiny), 0);
		EXPECT_EQ(matcher.getColumn(CreatureSizeType::kSmall), 1);
		EXPECT_EQ(matcher.getColumn(CreatureSizeType::kMedium), 2);
		EXPECT_EQ(matcher.getColumn(CreatureSizeType::kLarge), 3);
		EXPECT_EQ(matcher.getColumn(CreatureSizeType::kSuperLarge), 4);
	}
}