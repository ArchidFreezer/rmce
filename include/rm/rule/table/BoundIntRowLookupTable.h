#pragma once

#include <NumberRange.h>
#include <table/LookupTable.h>

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
class BoundIntRowLookupTable : public LookupTable <archid::NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype> {
public:

	/**
	 * @brief Default constructor deleted
	 */
	BoundIntRowLookupTable() = delete;

	/**
	 * @brief Constructor defining bounds
	 * @param id Unique identifier for the table
	 * @param min_row Minimum number for which a modified row value is defined
	 * @param max_row Maximum number for which a modified row value is defined
	 */
	BoundIntRowLookupTable(std::string_view id, int min_row, int max_row) : 
		LookupTable<archid::NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype>(id),
		max_row_{ max_row }, 
		min_row_{ min_row } {}

	/**
	 * @brief Constructor defining bounds
	 * @param id Unique identifier for the table
	 */
	BoundIntRowLookupTable(std::string_view id) :
		BoundIntRowLookupTable(id, INTMAX_MIN, INTMAX_MAX) {}

	/**
	 * @brief Set the maximum number for which a modified row value is defined
	 * @param max_row maximum row value
	 */
	void setMaxRow(int max_row) { max_row_ = max_row; }

	/**
	 * @brief Set the minimum number for which a modified row value is defined
	 * @param min_row minimum row value
	 */
	void setMinRow(int min_row) { min_row_ = min_row; }
	/**
	 * @brief Gets the value of a cell in the table
	 * 
	 * Constrains the value of the \a row_index parameter to be within the bounds set on the table so that it will never be larger
	 * than the maximum bound and never smaller than the minimum bound.
	 *
	 * @param col_index Value to match against the column
	 * @param row_index Value to identify the row, must be of the same type as the matcher
	 *
	 * @return Contents of the cell from the row that matches \a row_index and column \a col_index
	 *
	 * @throw RowNotFoundException if \a row_index does not match any matchers
	 * @throw ColNotFoundException if \a col_index is an invalid column
	 */
	const CellDatatype& cell(ColumnDataType col_index, int row_index) const {
		row_index = std::min(row_index, max_row_);
		row_index = std::max(row_index, min_row_);
		return LookupTable<archid::NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype>::cell(col_index, row_index);
	}

	/**
 * @brief Gets the value of a cell in the table
 *
 * Constrains the value of the \a row_index parameter to be within the bounds set on the table so that it will never be larger
 * than the maximum bound and never smaller than the minimum bound.
 *
 * @param col_index Value to match against the column
 * @param row_index Value to match against the row
 * @param unmodified_row_index Value to match against the rows that are for unmodified values
 *
 * @return Contents of the cell from the row that matches the indexes
 *
 * @throw RowNotFoundException if neither \a row_index nor \a unmodified_row_index are matched
 * @throw ColNotFoundException if \a col_index is an invalid column
 */
	const CellDatatype& cell(ColumnDataType col_index, int row_index, int unmodified_row_index) const {
		row_index = std::min(row_index, max_row_);
		row_index = std::max(row_index, min_row_);
		return LookupTable<archid::NumberRange<int>, int, ColumnMatcherClass, ColumnDataType, CellDatatype>::cell(col_index, row_index, unmodified_row_index);
	}

private:
	int max_row_{ INTMAX_MAX }; /**< Maximum number for which a modified row value is defined */
	int min_row_{ INTMAX_MIN }; /**< Minimum number for which a modified row value is defined */
}; 