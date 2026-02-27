#include <gtest/gtest.h>
#include <CreatureSizeType.h>
#include <NumberRange.h>
#include <table/BoundIntRowLookupTable.h>
#include <table/TableColumnCreatureSizeMatcher.h>
#include <NumberMatcherFactory.h>

using namespace rm;

namespace {

	/**
	 * @brief Dummy class to test LookupTable abstract class
	 */
	class DummyBoundIntTable : public rule::BoundIntRowLookupTable<rule::TableColumnCreatureSizeMatcher, rule::enums::CreatureSizeType::Type, std::string> {
	public:
		DummyBoundIntTable() : BoundIntRowLookupTable("DummyTable", 1, 6) {}
	};

	TEST(BoundIntRowLookupTable, General) {
		DummyBoundIntTable lut;
		std::unique_ptr<rule::TableColumnCreatureSizeMatcher> matcher = std::make_unique<rule::TableColumnCreatureSizeMatcher>(rule::TableColumnCreatureSizeMatcher());
		lut.setColumnMatcher(std::move(matcher));

		NumberMatcherFactory matchers;

		rule::TableRow<std::string> tr1 = rule::TableRow<std::string>().addCell("Row 1, kTiny").addCell("Row 1, kSmall").addCell("Row 1, kMedium").addCell("Row 1, kLarge").addCell("Row 1, kHuge");
		lut.addRow(matchers.matcher(1, 3), tr1);
		rule::TableRow<std::string> tr2 = rule::TableRow<std::string>().addCell("Row 2, kTiny").addCell("Row 2, kSmall").addCell("Row 2, kMedium").addCell("Row 2, kLarge").addCell("Row 2, kHuge");
		lut.addRow(matchers.matcher(4, 6), tr2);

		// We add unmodified rows that overlap the normal rows
		rule::TableRow<std::string> utr1 = rule::TableRow<std::string>().addCell("URow 1, kTiny").addCell("URow 1, kSmall").addCell("URow 1, kMedium").addCell("URow 1, kLarge").addCell("URow 1, kHuge");
		lut.addUnmodifiedRow(matchers.matcher(1, 1), utr1);
		rule::TableRow<std::string> utr2 = rule::TableRow<std::string>().addCell("URow 2, kTiny").addCell("URow 2, kSmall").addCell("URow 2, kMedium").addCell("URow 2, kLarge").addCell("URow 2, kHuge");
		lut.addUnmodifiedRow(matchers.matcher(6, 6), utr2);

		// We are not testing the matchers here so we don't need to test too many combinations
		// Test basic operation without unmodifiers
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kLarge, 2).c_str(), "Row 1, kLarge");
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kSmall, 4).c_str(), "Row 2, kSmall");

		// Test that the unmodified row is never selected if the method call without the unmodified value is used
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kLarge, 1).c_str(), "Row 1, kLarge");
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kSmall, 6).c_str(), "Row 2, kSmall");

		// Test that the unmodified row is always selected when it matches
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kLarge, 1, 2).c_str(), "Row 1, kLarge"); // No match for unmodified, use modified
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kSmall, 6, 4).c_str(), "Row 2, kSmall"); // No match for unmodified, use modified
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kLarge, 1, 1).c_str(), "URow 1, kLarge"); // Match both, use unmodified
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kSmall, 6, 6).c_str(), "URow 2, kSmall"); // Match both, use unmodified
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kLarge, 2, 1).c_str(), "URow 1, kLarge"); // Match both, use unmodified
		EXPECT_STREQ(lut.cell(rule::enums::CreatureSizeType::kSmall, 5, 6).c_str(), "URow 2, kSmall"); // Match both, use unmodified
	}

}