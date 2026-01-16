#pragma once

#include <string>
#include <map>
#include <memory>
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
 * Each row is indexed using a matcher class \p MatcherClass that adheres to the is_matcher concept that matches on the
 * datatype \p MatcherDatatype. When performing a lookup on the table the class will iterate through the rows until it
 * finds a matcher that matches and then from the associated row retrieves the data value of type \p CellDatatype based on
 * a lookup in the column matcher.
 * 
 * The column matcher is added via the setColumnMatcher() method which must becalled prior to attempting to retrieve data
 * from the table and must be of class TableColumnMatcher. Prior to any call to retrieve cell data this must be set to the
 * appropriate value for the type defining the columns, which are typically enumerations.
 * 
 * 
 * 
 * @tparam MatcherClass Matcher that is used to identify the correct row
 * @tparam MatcherDatatype Data type that the matcher \p MatcherClass matches on
 * @tparam CellDatatype Data type that is stored in each cell in the table
 */
template<typename MatcherClass, typename MatcherDatatype, typename CellDatatype, typename ColumnType>
	requires is_row_matcher<MatcherClass, MatcherDatatype>
class LookupTable {
public:
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
	void addRow(std::shared_ptr<MatcherClass> matcher, TableRow<CellDatatype> row) { table_.emplace(matcher, row); }

	/**
	 * @brief Gets the value of a cell in the table
	 * 
	 * @param row Value to identify the row, must be of the same type as the matcher
	 * @param col Column index from the row
	 * 
	 * @return Contents of the cell from the row that matches \a row and column \a col
	 * 
	 * @throw RowNotFoundException if \a row does not match any matchers
	 * @throw ColNotFoundException if \a col is an invalid column
	 */
	const CellDatatype& cell(MatcherDatatype row, int col) const {
		for (auto& tr : table_) {
			if ((*tr.first).matches(row)) return tr.second.cell(col);
		}
		throw RowNotFoundException("No row was found matching the value: " + std::to_string(row));
	}

	/**
	 * @brief Gets the value of a cell in the table
	 * 
	 * The column is determined by the configuration of the TableColumnMatcher
	 *
	 * @param row Value to identify the row, must be of the same type as the matcher
	 *
	 * @return Contents of the cell from the row that matches \a row and column \a col
	 *
	 * @throw RowNotFoundException if \a row does not match any matchers
	 * @throw ColNotFoundException if \a col is an invalid column
	 */
	const CellDatatype& cell(MatcherDatatype row) const {
		for (auto& tr : table_) {
			if ((*tr.first).matches(row)) return tr.second.cell(col_matcher_->getColumn());
		}
		throw RowNotFoundException("No row was found matching the value: " + std::to_string(row));
	}

	/**
	 * @brief Gets the value of a cell in the table
	 *
	 * The row is randomly generated using an even spread
	 * The column is determined by the configuration of the TableColumnMatcher
	 *
	 * @return Contents of the cell from the row that matches \a row and column \a col
	 *
	 * @throw RowNotFoundException if \a row does not match any matchers
	 * @throw ColNotFoundException if \a col is an invalid column
	 */
	virtual const CellDatatype& cell() const = 0;

	/**
	 * @brief Set the column matcher object
	 * 
	 * Sets the object that will be used to determine the row column to retrieve the cell for
	 * 
	 * @param col_matcher TableColumnMatcher column matcher object
	 */
	void setColumnMatcher(std::unique_ptr<TableColumnMatcher<ColumnType>> col_matcher) { col_matcher_ = std::move(col_matcher); }

	/**
	 * @brief Gets the object to determine the table column to retriev values from
	 * @return TableColumnMatcher table column matcher
	 */
	TableColumnMatcher<ColumnType>& columnMatcher() { return *col_matcher_; }

protected:
	std::map<std::shared_ptr<MatcherClass>, TableRow<CellDatatype>> table_; /**< Data structure representing the table */
	std::unique_ptr<TableColumnMatcher<ColumnType>> col_matcher_{}; /**< Object to determine table column to retrieve cell data from */
};
