#pragma once

#include <concepts>

/**
 * @brief Concept that defines the requirements for a matcher class
 * 
 * A matcher contains the definition of a set of values or objects and then allows another object or value to be tested
 * against this set to determine if it is considered to match the defined set or not.
 * 
 * The matcher must also override the less than operator allowing it to be used as a key in the standard library ordered
 * containers. Note that it is only the less than operator that is required and there is no requirement for a custom
 * hashing function so the class is not guaranteed to be usable in unordered containers.
 */
template <typename T, typename U>
concept is_matcher =
	requires(T v, U n) {
		{ v.operator<(v) } -> std::convertible_to<bool>;
		{ v.matches(n) } -> std::convertible_to<bool>;
}
&&
	requires(U n) {
	std::integral<U> || std::floating_point<U>;
};

/**
 * @class Matcher
 * @brief Class that is a container for classes implementing the is_matcher concept
 * 
 * The classes being wrapped must implement a method that matches a given value with the class. A typical use case for this
 * is to store a range within the matcher and then have it return whether a gicenm value is within the range or not.
 * 
 * The wrapper is used so that we can store a reference to the actual matcher and still use it in the STL container library
 * which does not permit references or pointers.
 * 
 * @tparam T Class being stored
 * @tparam U Datatype that the class matches on
 */
template<typename T, typename U>
	requires is_matcher<T, U>
class Matcher {
public:
	/**
	 * @brief Constructor
	 * @param matcher Matcher object to wrap
	 */
	Matcher(T matcher) : matcher_{ &matcher } {}
	
	/**
	 * @brief Determines if the value matches the data stored in the matcher
	 * @param val Value to check
	 * @return `true` if \a val matches
	 * @return `false` if \a val does not match
	 */
	bool matches(U val) const { return matcher_->matches(val); }

	/**
	 * @brief Implement an operator< method for the class
	 *
	 * This allows the class to be used in ordered collections such as the key in a `std::map`
	 *
	 * @param other Matcher to compare against
	 * @return `true` if this object is considered less than \a other
	 * @return `false` if this object is not considered less than \a other
	 */
	bool operator<(const Matcher<T, U>& other) const {
		return (*matcher_ < *other.matcher_);
	}
	
private:
	T* matcher_{}; /**< Matcher object we are wrapping */
};
