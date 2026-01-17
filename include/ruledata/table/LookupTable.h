#pragma once

#include <string>
#include <map>
#include <GameRuleData.h>
#include <table/TableColumnMatcher.h>
#include <table/TableRowMatcher.h>
#include <table/TableRow.h>

/**
 * @class RowNotFoundException
 * @brief Exception throw if no row was found during table lookup
 */
class RowNotFoundException : public std::runtime_error {
public:
	/**
	 * @brief Constructor
	 * @param error Error message to be displayed by the what() function
	 */
	RowNotFoundException(const std::string& error) : std::runtime_error{ error } {}
};

/**
 * @class LookupTable
 * @brief Base class representing a lookup table index by row and column
 * 
 * Each row is indexed using a matcher class \p RowMatcherClass that adheres to the is_matcher concept that matches on the
 * datatype \p RowDatatype. When performing a lookup on the table the class will iterate through the rows until it
 * finds a matcher that matches and then from the associated row retrieves the data value of type \p CellDatatype based on
 * a lookup in the column matcher.
 * 
 * The TableColumnMatcher matcher is added via the setColumnMatcher() method and must be set prior to attempting to
 * retrieve data from the table.
 * 
 * @tparam RowMatcherClass Class that identifies the table row to use
 * @tparam RowDatatype Data type of rows, used as input to the RowMatcherClass
 * @tparam ColumnMatcherClass Class that identifies the table column to use
 * @tparam ColumnDataType Data type of columns, used as input to the ColumnMatcherClass
 * @tparam CellDatatype Data type stored in the table cells
 */
template<typename RowMatcherClass, typename RowDatatype, typename ColumnMatcherClass, typename ColumnDataType, typename CellDatatype>
	requires table_row_matcher<RowMatcherClass, RowDatatype> && table_column_matcher< ColumnMatcherClass, ColumnDataType>
class LookupTable : public GameRuleData {
public:
	/**
	 * @brief Constructor with id
	 * @param id Unique identifier for the table
	 */
	LookupTable(std::string_view id) : GameRuleData(GameRuleData::generateId("LOOKUPTABLE", id)) {}

	/**
	 * @brief Default destructor to allow polymorphism
	 */
	virtual ~LookupTable() = default;

	/**
	 * @brief Add a row to the table
	 * 
	 * @param matcher Matcher that is used to identify the row
	 * @param row TableRow object that contains the cells in the row
	 */
	void addRow(std::shared_ptr<RowMatcherClass> matcher, TableRow<CellDatatype> row) { table_.emplace(matcher, row); }

	/**
	 * @brief Gets the value of a cell in the table
	 * 
	 * @param row Value to identify the row, must be of the same type as the matcher
	 * @param col Value to match against the column
	 *
	 * @return Contents of the cell from the row that matches \a row and column \a col
	 *
	 * @throw RowNotFoundException if \a row does not match any matchers
	 * @throw ColNotFoundException if \a col is an invalid column
	 */
	const CellDatatype& cell(RowDatatype row, ColumnDataType col) const {
		for (auto& tr : table_) {
			if ((*tr.first).matches(row)) return tr.second.cell(col_matcher_->column(col));
		}
		throw RowNotFoundException("No row was found matching the value: " + std::to_string(row));
	}

	/**
	 * @brief Set the column matcher object
	 * 
	 * Sets the object that will be used to determine the row column to retrieve the cell for
	 * 
	 * @param col_matcher TableColumnMatcher column matcher object
	 */
	void setColumnMatcher(std::unique_ptr<ColumnMatcherClass> col_matcher) { col_matcher_ = std::move(col_matcher); }

private:
	std::map<std::shared_ptr<RowMatcherClass>, TableRow<CellDatatype>> table_; /**< Data structure representing the table */
	std::unique_ptr<ColumnMatcherClass> col_matcher_{}; /**< Object to determine table column to retrieve cell data from */
};
