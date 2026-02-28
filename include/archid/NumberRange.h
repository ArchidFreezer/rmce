#pragma once

namespace archid {

	/**
	 * @brief Stores a range of contiguous numbers allowing checking against another number
	 *
	 * This class is a basic building block for the game lookup tables which typically have rows based on ranges of dice rolls.
	 * The row indexes may be stored in this class and then the actual rolls compared to the range to determine the correct
	 * row to use
	 *
	 * @tparam NumberType Type that the class uses to match against
	 * @implements table_row_matcher
	 */
	template<typename NumberType> requires std::integral<NumberType> || std::floating_point<NumberType>
	class NumberRange {
	public:
		/**
		 * @brief Constructor
		 * @param min lower bound of the range
		 * @param max upper bound of the range
		 */
		NumberRange(NumberType min, NumberType max) : min_{ min }, max_{ max } {}

		/**
		 * @brief Implement an operator< method for the class
		 *
		 * This allows the class to be used in ordered collections such as the key in a `std::map`
		 *
		 * @param other NumberRange<T> to compare against
		 * @return `true` if this object is considered less than \a other
		 * @return `false` if this object is not considered less than \a other
		 */
		bool operator<(const NumberRange<NumberType>& other) const {
			return std::tie(min_, max_) < std::tie(other.min_, other.max_);
		}

		/**
		 * @brief Determines if the value lies in the range or not.
		 *
		 * The check is performed against the bound min and max.
		 *
		 * @param val Number to check is in the range
		 * @return `true` if \a val is in the range;
		 * @return `false` if \a val is not in the range;
		 */
		bool matches(NumberType val) const { return (val >= min_) && (val <= max_); }

		/**
		 * @brief Get the minimum number that this range matches
		 * @return int Minimum matched number
		 */
		NumberType min() const { return min_; }

		/**
		 * @brief Get the maximum number that this range matches
		 * @return int Maximum matched number
		 */
		NumberType max() const { return max_; }

	private:
		NumberType min_{}; /**< Lower bound of the range */
		NumberType max_{}; /**< Upper bound of the range */
	};
}