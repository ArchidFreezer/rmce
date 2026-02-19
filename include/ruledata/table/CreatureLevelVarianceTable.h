#pragma once
#include <CreatureLevelVarianceType.h>
#include <table/BoundIntRowLookupTable.h>

/**
 * @class TableColumnLevelVarianceCodeMatcher
 * @brief Class to match the level variance code to a table column index
 *
 * This is used as the column matcher for the CreatureLevelVarianceType class.It takes a CreatureLevelVarianceType and returns the corresponding column index for that code.
 */
class TableColumnLevelVarianceCodeMatcher {
public:
	/**
	 * @brief Get the index of the table column
	 * @return int table column index
	 */
	int column(CreatureLevelVarianceType::Type match) const {
		switch (match) {
		case CreatureLevelVarianceType::Type::kNone: return 0;
		case CreatureLevelVarianceType::Type::kA: return 1;
		case CreatureLevelVarianceType::Type::kB: return 2;
		case CreatureLevelVarianceType::Type::kC: return 3;
		case CreatureLevelVarianceType::Type::kD: return 4;
		case CreatureLevelVarianceType::Type::kE: return 5;
		case CreatureLevelVarianceType::Type::kF: return 6;
		case CreatureLevelVarianceType::Type::kG: return 7;
		case CreatureLevelVarianceType::Type::kH: return 8;
		}
		return 0;
	}
};

/**
 * @class CreatureLevelVarianceTable
 * @brief Class to contain the creature level variance table
 *
 * This is a simple lookup table that takes a bonus XP code and a character level, returning the difference from the average level for a creature of this type.
 */
class CreatureLevelVarianceTable : public BoundIntRowLookupTable<TableColumnLevelVarianceCodeMatcher, CreatureLevelVarianceType::Type, int> {
public:

	/**
	 * @brief Constructor defining the name of the table
	 *
	 */
	CreatureLevelVarianceTable(std::string_view id) : BoundIntRowLookupTable(id, -100, 500) {}

	/**
	 * @brief In-game name of the table as used by players and NPCs
	 * @return Name of the table
	 */
	const std::string& name() const { return "Creature Level Variance Table"; }

};
