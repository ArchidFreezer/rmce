#pragma once

/**
 * @class TableColumnMatcher
 * @brief Abstract class to identify the column index in a table
 */
template<class ColumnType>
class TableColumnMatcher {
public:
	/**
	 * @brief Virtual destructor to allow for polymorphism
	 */
	virtual ~TableColumnMatcher() = default;

	/**
	 * @brief Get the index of the table column
	 * @return int table column index
	 */
	virtual int getColumn(ColumnType match) const = 0;
};
