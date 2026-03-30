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

template<typename E>
constexpr auto to_underlying(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}

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

template<typename E>
EnumRange<E> enum_range(E begin, E end) {
	return EnumRange<E>(begin, end);
}

} // namespace archid