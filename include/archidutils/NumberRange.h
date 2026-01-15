#pragma once

/**
 * @brief Stores a range of contiguous numbers allowing checking against another number
 * 
 * This class is a basic building block for the game lookup tables which typically have rows based on ranges of dice rolls.
 * The row indexes may be stored in this class and then the actual rolls compared to the range to determine the correct
 * row to use
 * 
 * @tparam T Number type, which can be either an integral or floating point
 * @implements is_matcher
 */
template<typename T> requires std::integral<T> || std::floating_point<T>
class NumberRange {
public:
	/**
	 * @brief Constructor
	 * @param min lower bound of the range
	 * @param max upper bound of the range
	 */
	NumberRange(T min, T max) : min_{ min }, max_{ max } {}

	/**
	 * @brief Implement an operator< method for the class
	 * 
	 * This allows the class to be used in ordered collections such as the key in a `std::map`
	 * 
	 * @param other NumberRange<T> to compare against
	 * @return `true` if this object is considered less than \a other
	 * @return `false` if this object is not considered less than \a other
	 */
	bool operator<(const NumberRange<T>& other) const {
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
	inline bool matches(T val) const { return (val >= min_) && (val <= max_); }

private:
	T min_{}; /**< Lower bound of the range */
	T max_{}; /**< Upper bound of the range */
};
