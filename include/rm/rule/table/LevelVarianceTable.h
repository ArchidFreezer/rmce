#pragma once
#include <LevelVarianceType.h>
#include <table/BoundIntRowLookupTable.h>

namespace rm::rule {

	/**
	 * @class TableColumnLevelVarianceCodeMatcher
	 * @brief Class to match the level variance code to a table column index
	 *
	 * This is used as the column matcher for the LevelVarianceType class.It takes a LevelVarianceType and returns the corresponding column index for that code.
	 */
	class TableColumnLevelVarianceCodeMatcher {
	public:
		/**
		 * @brief Get the index of the table column
		 * @return int table column index
		 */
		int column(rule::enums::LevelVarianceType::Type match) const {
			switch (match) {
			case rule::enums::LevelVarianceType::Type::kNone: return 0;
			case rule::enums::LevelVarianceType::Type::kA: return 1;
			case rule::enums::LevelVarianceType::Type::kB: return 2;
			case rule::enums::LevelVarianceType::Type::kC: return 3;
			case rule::enums::LevelVarianceType::Type::kD: return 4;
			case rule::enums::LevelVarianceType::Type::kE: return 5;
			case rule::enums::LevelVarianceType::Type::kF: return 6;
			case rule::enums::LevelVarianceType::Type::kG: return 7;
			case rule::enums::LevelVarianceType::Type::kH: return 8;
			}
			return 0;
		}
	};

	/**
	 * @class LevelVarianceTable
	 * @brief Class to contain the creature level variance table
	 *
	 * This is a simple lookup table that takes a bonus XP code and a character level, returning the difference from the average level for a creature of this type.
	 */
	class LevelVarianceTable : public BoundIntRowLookupTable<TableColumnLevelVarianceCodeMatcher, rule::enums::LevelVarianceType::Type, int> {
	public:

		/**
		 * @brief Constructor defining the name of the table
		 *
		 */
		LevelVarianceTable(std::string_view id) : BoundIntRowLookupTable(id, -100, 500) {}

		/**
		 * @brief In-game name of the table as used by players and NPCs
		 * @return Name of the table
		 */
		const std::string& name() const { return "Creature Level Variance Table"; }

	};

} // namespace rm::rule