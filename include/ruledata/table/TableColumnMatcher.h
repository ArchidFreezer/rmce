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
	virtual int getColumn() const = 0;

	/**
	 * @brief Sets the value to identify the column index
	 *
	 * This function returns a reference to the matcher allowing for method chaining, an example of usage would be:
	 *
	 * @code
	 * // Use a named matcher
	 * TableColumnMatcherDerivative matcher = TableColumnMatcher();
	 * int col = matcher.setMatch(CreatureSizeType::kLarge).getColumn();
	 *
	 * // Use an anonymous matcher
	 * int col = TableColumnMatcherDerivative().setMatch(CreatureSizeType::kMedium).getColumn();
	 * @endcode
	 *
	 * @param match Value to match
	 * @return TableColumnMatcher& reference to this matcher
	 */
	TableColumnMatcher& setMatch(ColumnType match) { match_ = match; return *this; }

protected:
	ColumnType match_; /**< Stores the value used to match the column */
};
