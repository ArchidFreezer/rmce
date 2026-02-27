#pragma once

#include <concepts>

namespace rm::rule {

	/**
	 * @brief Concept that defines the requirements for a table column matcher class
	 *
	 * A matcher contains the definition of a set of values or objects and then allows another object or value to be tested
	 * against this set to determine if it is considered to match the defined set or not.
	 *
	 */
	template <class MatcherClass, typename MatcherDatatype>
	concept table_column_matcher =
		requires(MatcherClass c, MatcherDatatype d) {
			{ c.column(d) } -> std::convertible_to<int>;
	};

} // namespace rm::rule