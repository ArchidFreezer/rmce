#pragma once
#include <CreatureSizeType.h>
#include <table/TableColumnMatcher.h>

/**
 * @class TableColumnCreatureSizeMatcher
 * @brief Class to identify the column index in a table using CreatureSizeType::Type as the header
 * 
 * Has CreatureSizeType::kTiny as the default type
 */
class TableColumnCreatureSizeMatcher : public TableColumnMatcher {
public:
	int getColumn() {
		using CreatureSizeType::Type;
		switch (match_) {
		case Type::kTiny: return 0;
		case Type::kSmall: return 1;
		case Type::kMedium: return 2;
		case Type::kLarge: return 3;
		case Type::kSuperLarge: return 4;
		}
		return 0;
	}

	/**
	 * @brief Sets the value to identify the column index
	 * 
	 * This function returns a reference to the matcher allowing for method chaining, an example of usage would be:
	 * 
	 * @code
	 * // Use a named matcher
	 * TableColumnCreatureSizeMatcher matcher = TableColumnCreatureSizeMatcher();
	 * int col = matcher.setMatch(CreatureSizeType::kLarge).getColumn();
	 * 
	 * // Use an anonymous matcher
	 * int col = TableColumnCreatureSizeMatcher().setMatch(CreatureSizeType::kMedium).getColumn();
	 * @endcode
	 * 
	 * @param match Value to match
	 * @return TableColumnCreatureSizeMatcher& reference to this matcher
	 */
	TableColumnCreatureSizeMatcher& setMatch(CreatureSizeType::Type match) { match_ = match; return *this;  }

private:
	CreatureSizeType::Type match_{ CreatureSizeType::kTiny }; /**< Stores the type to get the column for */
};
