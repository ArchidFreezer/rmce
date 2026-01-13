#pragma once

/**
 * @brief Stores a range of contiguous numbers allowing checking against another number
 * 
 * This class is a basic building block for the game lookup tables which typically have rows based on ranges of dice rolls.
 * The row indexes may be stored in this class and then the actual rolls compared to the range to determine the correct
 * row to use
 * 
 * @tparam T Number type, which can be either an integral or floating point
 */
template<typename T>
requires std::integral<T> || std::floating_point<T>
class NumberRange {
public:
	/**
	 * @brief Constructor
	 * @param min lower bound of the range
	 * @param max upper bound of the range
	 */
	NumberRange(T min, T max) : min_{ min }, max_{ max } {}

	/**
	 * @brief Determines if the value lies in the range or not.
	 * 
	 * The check is performed against the bound min and max.
	 * 
	 * @param val Number to check is in the range
	 * @return `true` if \a val is in the range; `false` otherwise
	 */
	inline bool inRange(T val) { return (val >= min_) && (val <= max_); }

private:
	T min_{};
	T max_{};
};
