#include <gtest/gtest.h>
#include <iostream>

#include <table/TableRow.h>

class Cell {
public:
	Cell(std::string val) : val_{ val } {}
	std::string getVal() const { return val_; }
	std::string val_;
};

namespace {
	TEST(TableRow, FindCell) {
		TableRow<Cell> tr = TableRow<Cell>().addCell(Cell("A")).addCell(Cell("B"));
		EXPECT_STREQ(tr.getCell(0).getVal().c_str(), "A");
		EXPECT_STREQ(tr.getCell(1).getVal().c_str(), "B");
	}

	TEST(TableRow, Exception) {
		TableRow<Cell> tr = TableRow<Cell>().addCell(Cell("A")).addCell(Cell("B"));
		try {
			tr.getCell(2);
			FAIL();
		} catch (ColNotFoundException err) {
			SUCCEED();
		} catch (...) {
			FAIL();
		}

	}
}