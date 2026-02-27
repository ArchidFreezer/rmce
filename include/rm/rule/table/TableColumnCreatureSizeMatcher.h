#pragma once
#include <CreatureSizeType.h>

namespace rm {

	/**
	 * @class TableColumnCreatureSizeMatcher
	 * @brief Class to identify the column index in a table using CreatureSizeType::Type as the header
	 *
	 * @implements column_row_matcher
	 */
	class TableColumnCreatureSizeMatcher {
	public:
		/**
		 * @brief Get the index of the table column
		 * @return int table column index
		 */
		int column(CreatureSizeType::Type match) const {
			using CreatureSizeType::Type;
			switch (match) {
			case Type::kTiny: return 0;
			case Type::kSmall: return 1;
			case Type::kMedium: return 2;
			case Type::kLarge: return 3;
			case Type::kHuge: return 4;
			}
			return 0;
		}
	};

} // namespace rm