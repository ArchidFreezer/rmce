#include <gtest/gtest.h>
#include <iostream>

#include <table/TableRow.h>

using namespace rm;

class Cell {
public:
	Cell(std::string val) : val_{ val } {}
	std::string getVal() const { return val_; }
	std::string val_;
};

namespace {
	TEST(TableRow, FindCell) {
		rule::TableRow<Cell> tr = rule::TableRow<Cell>().addCell(Cell("A")).addCell(Cell("B"));
		EXPECT_STREQ(tr.cell(0).getVal().c_str(), "A");
		EXPECT_STREQ(tr.cell(1).getVal().c_str(), "B");
	}

	TEST(TableRow, Exception) {
		rule::TableRow<Cell> tr = rule::TableRow<Cell>().addCell(Cell("A")).addCell(Cell("B"));
		EXPECT_THROW(tr.cell(2), rule::ColNotFoundException);
	}
}