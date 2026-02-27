#pragma once
#include <ArmourType.h>

namespace rm {

	/**
	 * @class TableColumnArmourTypeMatcher
	 * @brief Class to identify the column index in a table using ArmourType::Type as the header
	 *
	 * @implements column_row_matcher
	 */
	class TableColumnArmourTypeMatcher {
	public:
		/**
		 * @brief Get the index of the table column
		 * @return int table column index
		 */
		int column(rule::enums::ArmourType::Type match) const {
			using rule::enums::ArmourType::Type;
			switch (match) {
			case Type::kAT1: return 0;
			case Type::kAT2: return 1;
			case Type::kAT3: return 2;
			case Type::kAT4: return 3;
			case Type::kAT5: return 4;
			case Type::kAT6: return 5;
			case Type::kAT7: return 6;
			case Type::kAT8: return 7;
			case Type::kAT9: return 8;
			case Type::kAT10: return 9;
			case Type::kAT11: return 10;
			case Type::kAT12: return 11;
			case Type::kAT13: return 12;
			case Type::kAT14: return 13;
			case Type::kAT15: return 14;
			case Type::kAT16: return 15;
			case Type::kAT17: return 16;
			case Type::kAT18: return 17;
			case Type::kAT19: return 18;
			case Type::kAT20: return 19;
			}
			return 0;
		}
	};

} // namespace rm