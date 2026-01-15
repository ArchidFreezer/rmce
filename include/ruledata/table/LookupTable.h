#pragma once

#include <string>
#include <memory>
#include <Matcher.h>
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
 * @brief Class representing a lookup table index by row and column
 * 
 * Each row is index using a matcher class \p T that adheres to the is_matcher concept that matches on the datatype \p U.
 * When performing a lookup on the table the class will iterate through the rows until it finds a matcher that matches and
 * then from the associated row retrieves the data value of type \p V.
 * 
 * @tparam MatcherClass Matcher that is used to identify the correct row
 * @tparam MatcherDatatype Data type that the matcher \p MatcherClass matches on
 * @tparam CellDatatype Data type that is stored in each cell in the table
 */
template<typename MatcherClass, typename MatcherDatatype, typename CellDatatype>
	requires is_matcher<MatcherClass, MatcherDatatype>
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
	const CellDatatype& getCell(MatcherDatatype row, int col) {
		for (auto& tr : table_) {
			if ((*tr.first).matches(row)) return tr.second.getCell(col);
		}
		throw RowNotFoundException("No row was found matching the value: " + std::to_string(row));
	}

private:
	std::map<std::shared_ptr<MatcherClass>, TableRow<CellDatatype>> table_; /**< Data structure representing the table */
};
