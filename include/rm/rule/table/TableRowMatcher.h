#pragma once

#include <concepts>

namespace rm::rule::table {

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
	template <class MatcherClass, typename MatcherDatatype>
	concept table_row_matcher =
		requires(MatcherClass v, MatcherDatatype n) {
			{ v.operator<(v) } -> std::convertible_to<bool>;
			{ v.matches(n) } -> std::convertible_to<bool>;
	};

} // namespace rm::rule::table