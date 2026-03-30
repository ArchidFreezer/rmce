#pragma once

#include <iostream>
#include <type_traits>

/*
 * @file EnumIterator.h
 * Code copied from: https://medium.com/@ryan_forrester_/c-enum-iteration-comprehensive-guide-647a30f0f2ca
 * 
 * This file provides a utility for iterating over enum values in C++. It defines a template class `EnumIterator` that allows for iterating through the values of an enum type, and a helper function `enum_range` to create a range of enum
 * values for iteration. The `to_underlying` function is used to convert enum values to their underlying integer representation, which is necessary for the iteration process. This utility can be used to easily loop through all values of an
 * enum type in a range-based for loop or any other context where iteration is needed.
 * 
 * There is an underlying assumption that the enum values are contiguous and start from 0, which is common for many enums but may not always be the case. If the enum values are not contiguous or do not start from 0, additional logic would
 * be needed to handle such cases.
 */

namespace archid {

/**
 * @brief Converts an enumeration value to its underlying integral type.
 * 
 * This is a helper function and not expected to be used directly by users of the EnumIterator. It is used internally to convert enum values to their underlying integer representation, which is necessary for the iteration process in the
 * EnumIterator class.
 * 
 * @tparam E The enumeration type.
 * @param e The enumeration value to convert.
 * @return The underlying integral value of the enumeration.
 * 
 * @see enum_range
 * @see EnumIterator
 */
template<typename E>
constexpr auto to_underlying(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}

/**
 * @brief An iterator for iterating over consecutive enum values.
 * 
 * Helper class for iterating over consecutive enum values. It assumes that the enum values are contiguous and start from 0. The iterator works by maintaining an integer value that represents the current position in the enum sequence, and
 * provides the necessary operators for iteration (dereference, increment, and inequality comparison).
 * 
 * @tparam E The enum type to iterate over.
 * 
 * @see enum_range
 */
template<typename E>
class EnumIterator {
	int value;

public:
	explicit EnumIterator(int v) : value(v) {
	}
	E operator*() const {
		return static_cast<E>(value);
	}
	EnumIterator& operator++() {
		++value;
		return *this;
	}
	bool operator!=(const EnumIterator& other) const {
		return value != other.value;
	}
};

/**
 * @brief A range for iterating over a sequence of enum values.
 * 
 * Helper class for creating a range of enum values to iterate over. It takes a starting and ending enum value and provides begin() and end() methods that return EnumIterator instances for the specified range. The end iterator is set to one past the
 * last valid enum value to allow for proper iteration in a range-based for loop or any other context where iteration is needed.
 * 
 * @tparam E The enum type to create a range for.
 * 
 * @see EnumIterator
 * @see enum_range
 */
template<typename E>
class EnumRange {
	int begin_value, end_value;

public:
	EnumRange(E begin, E end) : begin_value(to_underlying(begin)), end_value(to_underlying(end)) {
	}
	EnumIterator<E> begin() const {
		return EnumIterator<E>(begin_value);
	}
	EnumIterator<E> end() const {
		return EnumIterator<E>(end_value + 1);
	}
};

/**
 * @brief Creates a range of enum values for iteration.
 * 
 * Function to create an EnumRange for a given range of enum values. It takes a starting and ending enum value and returns an EnumRange that can be used in a range-based for loop or any other context where iteration is needed. The function assumes
 * that the enum values are contiguous and start from 0, which is common for many enums but may not always be the case. If the enum values are not contiguous or do not start from 0, additional logic would be needed to handle such cases.
 * 
 * @code
 * for (auto value : enum_range(MyEnum::Start, MyEnum::End)) {
 *     // Use value
 * }
 * @endcode
 * 
 * @tparam E The enum type to create a range for.
 * @param begin The starting enum value of the range (inclusive).
 * @param end The ending enum value of the range (inclusive).
 * @return An EnumRange object representing the specified range of enum values.
 * 
 * @see EnumRange
 */
template<typename E>
EnumRange<E> enum_range(E begin, E end) {
	return EnumRange<E>(begin, end);
}

} // namespace archid