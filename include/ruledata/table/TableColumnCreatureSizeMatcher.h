#pragma once
#include <CreatureSizeType.h>
#include <table/TableColumnMatcher.h>

/**
 * @class TableColumnCreatureSizeMatcher
 * @brief Class to identify the column index in a table using CreatureSizeType::Type as the header
 * 
 * Has CreatureSizeType::kTiny as the default type
 */
class TableColumnCreatureSizeMatcher : public TableColumnMatcher<CreatureSizeType::Type> {
public:

	/**
	 * @brief Constructor to set the header type and initialise member variables
	 */
	TableColumnCreatureSizeMatcher() : TableColumnMatcher<CreatureSizeType::Type>() {
		match_ = CreatureSizeType::Type::kTiny;
	}

	int getColumn() const {
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
};
