#include <gtest/gtest.h>

#include <CreatureSizeType.h>
#include <NumberRange.h>
#include <table/LookupTable.h>
#include <table/TableColumnCreatureSizeMatcher.h>

/**
 * @brief Dummy class to test LookupTable abstract class
 */
class DummyLookupTable : public LookupTable<NumberRange<int>, int, std::string, CreatureSizeType::Type> {
public:
	using LookupTable::cell;
	const std::string& cell() const override { return temp_; } // This produces a warning but we aren't calling it
	std::string temp_{ "Dummy cell value" };
};

TEST(LookupTable, General) {
	DummyLookupTable lut;
	std::unique_ptr<TableColumnCreatureSizeMatcher> matcher = std::make_unique<TableColumnCreatureSizeMatcher>(TableColumnCreatureSizeMatcher());
	lut.setColumnMatcher(std::move(matcher));
	TableRow<std::string> tr1 = TableRow<std::string>().addCell("Row 1, kTiny").addCell("Row 1, kSmall").addCell("Row 1, kMedium").addCell("Row 1, kLarge").addCell("Row 1, kSuperLarge");
	lut.addRow(std::make_shared<NumberRange<int>>(1, 3), tr1);
	TableRow<std::string> tr2 = TableRow<std::string>().addCell("Row 2, kTiny").addCell("Row 2, kSmall").addCell("Row 2, kMedium").addCell("Row 2, kLarge").addCell("Row 2, kSuperLarge");
	lut.addRow(std::make_shared<NumberRange<int>>(4, 6), tr2);
	int col1{ lut.columnMatcher().setMatch(CreatureSizeType::kLarge).getColumn() };
	// Test that the column matcher value is being correctly passed through
	EXPECT_EQ(col1, 3);

	// We are not testing the matchers here so we don't need to test too many combinations
	EXPECT_STREQ(lut.cell(2).c_str(), "Row 1, kLarge");
	EXPECT_STREQ(lut.cell(4).c_str(), "Row 2, kLarge");

	EXPECT_STREQ(lut.cell().c_str(), "Dummy cell value");
}