#pragma once

#include <stdexcept>
#include <vector>

namespace rm {

	/**
	 * @class ColNotFoundException
	 * @brief Exception throw if an attempt was made to access a column index in the row that does not exist
	 */
	class ColNotFoundException : public std::runtime_error {
	public:
		/**
		 * @brief Constructor
		 * @param error Error message to be displayed by the what() function
		 */
		ColNotFoundException(const std::string& error) : std::runtime_error{ error } {}
	};

	/**
	 * @class TableRow
	 * @brief Class representing a row in a table
	 * @tparam CellDataType Type of the value being stored in each cell in the row
	 */
	template<typename CellDataType>
	class TableRow {
	public:

		/**
		 * @brief Return a specific cell in the row
		 * @param index Column number of the cell to retrieve, with the first column having an index of 0
		 * @return CellDataType contents of the cell
		 * @throws ColNotFoundException if an attempt is made to access a column that does not exist
		 */
		const CellDataType& cell(int index) const {
			if (index >= colCount() || index < 0) throw ColNotFoundException("Attempt to access a column in the row that does not exist");

			return cells_[index];
		}

		/**
		 * @brief Appends a new column to the row
		 * The new column is added at the end of the row
		 * @param val Cell to add
		 */
		TableRow& addCell(CellDataType val) { cells_.push_back(val); return *this; }

		/**
		 * @brief Gets the number of columns in the row
		 * @return int number of columns
		 */
		int colCount() const { return cells_.size(); }

	private:
		std::vector<CellDataType> cells_; /**< Container storing the cells in the row */
	};

} // namespace rm