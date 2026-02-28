#include <gtest/gtest.h>

#include <table/SpecialAttackTable.h>
#include <NumberMatcherFactory.h>

using namespace rm::rule::enums;
using namespace rm::rule::table;

using namespace rm;

namespace {

	TableRow<std::string> buildATRow(std::string prefix, int row_num) {
		TableRow<std::string> tr{};
		for (int i{ 1 }; i < 21; i++) {
			tr.addCell(prefix + std::to_string(row_num) + ", kAT" + std::to_string(i));
		}
		return tr;
	}

	TEST(SpecialAttackTable, GeneralRef) {
		// Populate a table with 6 rows
		std::map<AttackSizeType::Type, int> limits{};
		limits.emplace(AttackSizeType::kSmall, 2);
		limits.emplace(AttackSizeType::kMedium, 3);
		limits.emplace(AttackSizeType::kLarge, 4);
		limits.emplace(AttackSizeType::kHuge, 5);

		NumberMatcherFactory matchers;

		SpecialAttackTable at{ "TestGeneral" , limits};
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(matchers.matcher(i, i), buildATRow("Row ", i));
		}
		at.addUnmodifiedRow(matchers.matcher(1, 1), buildATRow("URow ", 1));
		at.addUnmodifiedRow(matchers.matcher(6, 6), buildATRow("URow ", 6));

		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kSmall, 3).c_str(), "Row 2, kAT12");     // Capped by type
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3).c_str(), "Row 3, kAT12");     // Uncapped
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3, 1).c_str(), "URow 1, kAT12"); // Unmodified, uncapped match
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3, 6).c_str(), "Row 3, kAT12");  // Capped so can't find UM at 6
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3, 2).c_str(), "Row 3, kAT12");  // No unmodified match
	}

	TEST(SpecialAttackTable, GeneralExplicit) {

		NumberMatcherFactory matchers;

		// Populate a table with 6 rows
		SpecialAttackTable at{ "TestGeneral", 2, 3, 4, 5 };
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(matchers.matcher(i, i), buildATRow("Row ", i));
		}
		at.addUnmodifiedRow(matchers.matcher(1, 1), buildATRow("URow ", 1));
		at.addUnmodifiedRow(matchers.matcher(6, 6), buildATRow("URow ", 6));

		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kSmall, 3).c_str(), "Row 2, kAT12");     // Capped by type
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3).c_str(), "Row 3, kAT12");     // Uncapped
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3, 1).c_str(), "URow 1, kAT12"); // Unmodified, uncapped match
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3, 6).c_str(), "Row 3, kAT12");  // Capped so can't find UM at 6
		EXPECT_STREQ(at.cell(ArmourType::kAT12, AttackSizeType::kLarge, 3, 2).c_str(), "Row 3, kAT12");  // No unmodified match
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

	TEST(SpecialAttackTable, Exception) {
		NumberMatcherFactory matchers;
		// Populate a table with 6 rows, but only 10 columns
		SpecialAttackTable at{ "TestException", 2, 3, 4, 5 };
		for (int i{ 1 }; i < 6; i++) {
			at.addRow(matchers.matcher(i, i), buildBrokenATRow("Row ", i));
		}

		EXPECT_THROW(at.cell(ArmourType::kAT12, AttackSizeType::kSmall, 0), RowNotFoundException);
		EXPECT_THROW(at.cell(ArmourType::kAT12, AttackSizeType::kSmall, 1), ColNotFoundException);

	}


}