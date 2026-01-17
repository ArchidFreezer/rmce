#pragma once

#include <CreatureSizeType.h>
#include <NumberRange.h>
#include <table/LookupTable.h>
#include <table/TableColumnCreatureSizeMatcher.h>
#include <table/TableColumnMatcher.h>

/**
 * @class  BoundIntRowLookupTable
 * @brief Lookup table class with inetegers for the rows and sets the minimum and maximum row value
 * 
 * This class is used for tables where the value used to identify the row is either an open ended dice roll or has
 * modifiers applied meaning that the actual value may be outside the range of values that the rows have matchers for in
 * these scenarios the smallest or largest row is the one that should be used. 
 * 
 * @tparam ColumnMatcherClass Class that identifies the table column to use
 * @tparam ColumnDataType Data type of the column, used as input to the ColumnMatcherClass
 * @tparam CellDatatype Data type stored in the table cells
 */
template<typename ColumnMatcherClass, typename ColumnDataType, typename CellDatatype>
	requires table_column_matcher< ColumnMatcherClass, ColumnDataType>
class BoundIntRowLookupTable : public LookupTable <NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype> {
public:
	/**
	 * @brief Constructor defining bounds
	 * @param id Unique identifier for the table
	 * @param min_row Minimum number for which a modified row value is defined
	 * @param max_row Maximum number for which a modified row value is defined
	 */
	BoundIntRowLookupTable(std::string_view id, int min_row, int max_row) : 
		LookupTable<NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype>(id), 
		max_row_{ max_row }, 
		min_row_{ min_row } {}

	/**
	 * @brief Gets the value of a cell in the table
	 * 
	 * Constrains the value of the \a row parameter to be within the bounds set on the table so that it will never be larger
	 * than the maximum bound and never smaller than the minimum bound.
	 *
	 * @param col Value to match against the column
	 * @param row Value to identify the row, must be of the same type as the matcher
	 *
	 * @return Contents of the cell from the row that matches \a row and column \a col
	 *
	 * @throw RowNotFoundException if \a row does not match any matchers
	 * @throw ColNotFoundException if \a col is an invalid column
	 */
	const CellDatatype& cell(ColumnDataType col, int row) const {
		row = std::min(row, max_row_);
		row = std::max(row, min_row_);
		return LookupTable<NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype>::cell(col, row);
	}

private:
	int max_row_{ INTMAX_MAX }; /**< Maximum number for which a modified row value is defined */
	int min_row_{ INTMAX_MIN }; /**< Minimum number for which a modified row value is defined */
}; 