#include <gtest/gtest.h>

#include <CreatureSizeType.h>
#include <NumberRange.h>
#include <table/LookupTable.h>
#include <table/TableColumnCreatureSizeMatcher.h>

/**
 * @brief Dummy class to test LookupTable abstract class
 */
class DummyLookupTable : public LookupTable<NumberRange<int>, int, TableColumnCreatureSizeMatcher, CreatureSizeType::Type, std::string> {
public:
	DummyLookupTable(std::string_view id) : LookupTable(id) {}
};

TEST(LookupTable, General) {
	DummyLookupTable lut("DummyTable");
	std::unique_ptr<TableColumnCreatureSizeMatcher> matcher = std::make_unique<TableColumnCreatureSizeMatcher>(TableColumnCreatureSizeMatcher());
	lut.setColumnMatcher(std::move(matcher));
	TableRow<std::string> tr1 = TableRow<std::string>().addCell("Row 1, kTiny").addCell("Row 1, kSmall").addCell("Row 1, kMedium").addCell("Row 1, kLarge").addCell("Row 1, kSuperLarge");
	lut.addRow(std::make_shared<NumberRange<int>>(1, 3), tr1);
	TableRow<std::string> tr2 = TableRow<std::string>().addCell("Row 2, kTiny").addCell("Row 2, kSmall").addCell("Row 2, kMedium").addCell("Row 2, kLarge").addCell("Row 2, kSuperLarge");
	lut.addRow(std::make_shared<NumberRange<int>>(4, 6), tr2);

	// We are not testing the matchers here so we don't need to test too many combinations
	EXPECT_STREQ(lut.cell(2, CreatureSizeType::kLarge).c_str(), "Row 1, kLarge");
	EXPECT_STREQ(lut.cell(4, CreatureSizeType::kSmall).c_str(), "Row 2, kSmall");
}