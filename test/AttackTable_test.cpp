#include <gtest/gtest.h>

#include <table/AttackTable.h>
#include <NumberMatcherFactory.h>

using namespace rm::rule::enums;
using namespace rm::rule::table;

namespace {

	TableRow<std::string> buildATRow(std::string prefix, int row_num) {
		TableRow<std::string> tr{};
		for (int i{ 1 }; i < 21; i++) {
			tr.addCell(prefix + std::to_string(row_num) + ", kAT" + std::to_string(i));
		}
		return tr;
	}

	TEST(AttackTable, General) {

		rm::rule::table::NumberMatcherFactory matchers;

		// Populate a table with 6 rows
		AttackTable at{"TestGeneral"};
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(matchers.matcher(i, i), buildATRow("Row ", i));
		}
		at.addUnmodifiedRow(matchers.matcher(1, 1), buildATRow("URow ", 1));
		at.addUnmodifiedRow(matchers.matcher(6, 6), buildATRow("URow ", 6));

		EXPECT_STREQ(at.cell(ArmourType::kAT12, 3).c_str(), "Row 3, kAT12");     // No unmodifier
		EXPECT_STREQ(at.cell(ArmourType::kAT12, 3, 1).c_str(), "URow 1, kAT12"); // Unmodified match
		EXPECT_STREQ(at.cell(ArmourType::kAT12, 3, 2).c_str(), "Row 3, kAT12");  // No unmodified match
	}

	/*
	* This only populates the first 10 columns 
	*/
	TableRow<std::string> buildBrokenATRow(std::string prefix, int row_num) {
		TableRow<std::string> tr{};
		for (int i{ 1 }; i < 11; i++) {
			tr.addCell(prefix + std::to_string(row_num) + ", kAT" + std::to_string(i));
		}
		return tr;
	}

	TEST(AttackTable, Exception) {

		rm::rule::table::NumberMatcherFactory matchers;

		// Populate a table with 6 rows, but only 10 columns
		AttackTable at{"TestException"};
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(matchers.matcher(i, i), buildBrokenATRow("Row ", i));
		}

		EXPECT_THROW(at.cell(ArmourType::kAT12, 0), RowNotFoundException);
		EXPECT_THROW(at.cell(ArmourType::kAT12, 1), ColNotFoundException);
	}


}