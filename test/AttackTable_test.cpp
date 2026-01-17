#include <gtest/gtest.h>

#include <table/AttackTable.h>

namespace {

	TableRow<std::string> buildATRow(std::string prefix, int row_num) {
		TableRow<std::string> tr{};
		for (int i{ 1 }; i < 21; i++) {
			tr.addCell(prefix + std::to_string(row_num) + ", kAT" + std::to_string(i));
		}
		return tr;
	}

	TEST(AttackTable, General) {
		// Populate a table with 6 rows
		AttackTable at{"TestGeneral"};
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(std::make_shared<NumberRange<int>>(i, i), buildATRow("Row ", i));
		}
		at.addUnmodifiedRow(std::make_shared<NumberRange<int>>(1, 1), buildATRow("URow ", 1));
		at.addUnmodifiedRow(std::make_shared<NumberRange<int>>(6, 6), buildATRow("URow ", 6));

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
		// Populate a table with 6 rows, but only 10 columns
		AttackTable at{"TestException"};
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(std::make_shared<NumberRange<int>>(i, i), buildBrokenATRow("Row ", i));
		}

		try {
			at.cell(ArmourType::kAT12, 0);
			FAIL();
		} catch (RowNotFoundException err) {
			SUCCEED();
		} catch (...) {
			FAIL();
		}
		
		try {
			at.cell(ArmourType::kAT12, 1);
			FAIL();
		} catch (ColNotFoundException err) {
			SUCCEED();
		} catch (...) {
			FAIL();
		}
		
	}


}