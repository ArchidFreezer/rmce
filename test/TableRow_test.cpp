#include <gtest/gtest.h>
#include <iostream>

#include <table/TableRow.h>

class Cell {
public:
	inline Cell(std::string val) : val_{ val } {}
	inline std::string getVal() const { return val_; }
	std::string val_;
};

namespace {
	TEST(TableRow, FindCell) {

		TableRow<Cell> tr;
		Cell c1("A");
		Cell c2("B");
		tr.addCell(c1);
		tr.addCell(c2);
		EXPECT_STREQ(tr.getCell(0).getVal().c_str(), "A");
		EXPECT_STREQ(tr.getCell(1).getVal().c_str(), "B");
	}

	TEST(TableRow, Exception) {
		TableRow<Cell> tr;
		Cell c1("A");
		Cell c2("B");
		tr.addCell(c1);
		tr.addCell(c2);
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