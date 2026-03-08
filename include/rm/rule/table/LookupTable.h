#pragma once

#include <string>
#include <map>
#include <memory>
#include <GameRuleData.h>
#include <table/TableColumnMatcher.h>
#include <table/TableRowMatcher.h>
#include <table/TableRow.h>

/**
 * @namespace rm::rule::table
 * 
 * This namespace contains the classes that are used to represent lookup tables that are indexed by row and column.
 * The main class is the LookupTable class which is a base class that can be used to create specific tables for different game rules. The LookupTable class uses matcher classes to identify the rows and columns to retrieve the cell data from.
 * The matcher classes must adhere to the is_matcher concept and are used to match against the row and column index values provided when retrieving cell data from the table.
 */
namespace rm::rule::table {

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
		requires table_row_matcher<RowMatcherClass, RowDatatype>&& table_column_matcher< ColumnMatcherClass, ColumnDataType>
	class LookupTable : public rule::GameRuleData {
	public:
		/**
		 * @brief Constructor with id
		 * @param id Unique identifier for the table
		 */
		LookupTable(std::string_view id) : GameRuleData(id) {}

		/**
		 * @brief Default destructor to allow polymorphism
		 */
		virtual ~LookupTable() = default;

		/**
		 * @brief Add a row to the table that may be accessed by any row index value
		 *
		 * @param matcher Matcher that is used to identify the row
		 * @param row TableRow object that contains the cells in the row
		 */
		void addRow(const RowMatcherClass* matcher, TableRow<CellDatatype> row) { modified_rows_.emplace(matcher, row); }

		/**
		 * @brief Add a row to the table that may only be accessed by row index values that have not been modified
		 *
		 * @param matcher Matcher that is used to identify the row
		 * @param row TableRow object that contains the cells in the row
		 */
		void addUnmodifiedRow(const RowMatcherClass* matcher, TableRow<CellDatatype> row) { unmodified_rows_.emplace(matcher, row); }

		/**
		 * @brief Gets the value of a cell in the table
		 *
		 * @param col_index Value to match against the column
		 * @param row_index Value to match against the row
		 *
		 * @return Contents of the cell from the row that matches \a row_index and column \a col_index
		 *
		 * @throw RowNotFoundException if \a row_index does not match any matchers
		 * @throw ColNotFoundException if \a col_index is an invalid column
		 */
		const CellDatatype& cell(ColumnDataType col_index, RowDatatype row_index) const {
			return cell(col_index, row_index, row_index, false);
		}

		/**
		 * @brief Gets the value of a cell in the table
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
		const CellDatatype& cell(ColumnDataType col_index, RowDatatype row_index, RowDatatype unmodified_row_index) const {
			return cell(col_index, row_index, unmodified_row_index, true);
		}

		/**
		 * @brief Set the column matcher object
		 *
		 * Sets the object that will be used to determine the row column to retrieve the cell for
		 *
		 * @param col_matcher TableColumnMatcher column matcher object
		 */
		void setColumnMatcher(std::unique_ptr<ColumnMatcherClass> col_matcher) { col_matcher_ = std::move(col_matcher); }

		/**
		 * @brief Get the map with the rows that may be returned by any row index value
		 * @return std::map<std::unique_ptr<RowMatcherClass>, TableRow<CellDatatype>> Map of rows
		 */
		const std::map<const RowMatcherClass*, TableRow<CellDatatype>>& modified() const { return modified_rows_; }

		/**
		 * @brief Get the map with the rows that may be only returned by unmodified row index values
		 * @return std::map<std::unique_ptr<RowMatcherClass>, TableRow<CellDatatype>> Map of rows
		 */
		const std::map<const RowMatcherClass*, TableRow<CellDatatype>>& unmodified() const { return unmodified_rows_; }

	private:
		/**
		 * @brief Gets the value of a cell in the table
		 *
		 * This function implements the core lookup functionality and is used by all other methods. It has a flag that indicates
		 * whether there should be any check against unmodified rows as this may be called by a function that does not expect this.
		 *
		 * @param col_index Value to match against the column
		 * @param row_index Value to match against the row
		 * @param unmodified_row_index Value to match against the rows that are for unmodified values
		 * @param use_unmodified Whether to test for unmodified rows
		 *
		 * @return Contents of the cell from the row that matches the indexes
		 *
		 * @throw RowNotFoundException if neither \a row_index nor \a unmodified_row_index are matched
		 * @throw ColNotFoundException if \a col_index is an invalid column
		 */
		const CellDatatype& cell(ColumnDataType col_index, RowDatatype row_index, RowDatatype unmodified_row_index, bool use_unmodified) const;

		std::map<const RowMatcherClass*, TableRow<CellDatatype>> modified_rows_; /**< Container for rows that may be returned by any row index value */
		std::map<const RowMatcherClass*, TableRow<CellDatatype>> unmodified_rows_; /**< Container for rows that may be only returned by unmodified row index values */
		std::unique_ptr<ColumnMatcherClass> col_matcher_{}; /**< Object to determine table column to retrieve cell data from */
	};

	template<typename RowMatcherClass, typename RowDatatype, typename ColumnMatcherClass, typename ColumnDataType, typename CellDatatype>
		requires table_row_matcher<RowMatcherClass, RowDatatype>&& table_column_matcher< ColumnMatcherClass, ColumnDataType>
	inline const CellDatatype& LookupTable<RowMatcherClass, RowDatatype, ColumnMatcherClass, ColumnDataType, CellDatatype>::cell(ColumnDataType col_index, RowDatatype row_index, RowDatatype unmodified_row_index, bool use_unmodified) const {
		if (use_unmodified) {
			for (auto& tr : unmodified_rows_) {
				if ((*tr.first).matches(unmodified_row_index)) return tr.second.cell(col_matcher_->column(col_index));
			}
		}

		for (auto& tr : modified_rows_) {
			if ((*tr.first).matches(row_index)) return tr.second.cell(col_matcher_->column(col_index));
		}

		// We didn't find a row so build the message to put in the exception
		std::string msg{ "No rows found matching index(s)" };
		throw RowNotFoundException(msg);
	}

} // namespace rmrm::rule::table