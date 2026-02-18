#pragma once
#include <AnimalData.h>
#include <table/BoundIntRowLookupTable.h>

/**
 * @class TableColumnBonusXpCodeMatcher
 * @brief Class to match the bonus XP code to a table column index
 *
 * This is used as the column matcher for the CreatureBonusXpTable class. It takes a BonusXpCode and returns the corresponding column index for that code.
 */
class TableColumnBonusXpCodeMatcher {
public:
	/**
	 * @brief Get the index of the table column
	 * @return int table column index
	 */
	int column(AnimalData::BonusXpCode match) const {
		switch (match) {
		case AnimalData::BonusXpCode::kA: return 0;
		case AnimalData::BonusXpCode::kB: return 1;
		case AnimalData::BonusXpCode::kC: return 2;
		case AnimalData::BonusXpCode::kD: return 3;
		case AnimalData::BonusXpCode::kE: return 4;
		case AnimalData::BonusXpCode::kF: return 5;
		case AnimalData::BonusXpCode::kG: return 6;
		case AnimalData::BonusXpCode::kH: return 7;
		case AnimalData::BonusXpCode::kI: return 8;
		case AnimalData::BonusXpCode::kJ: return 9;
		case AnimalData::BonusXpCode::kK: return 10;
		case AnimalData::BonusXpCode::kL: return 11;
		}
		return 0;
	}
};

/**
 * @class CreatureBonusXpTable
 * @brief Class to contain the creature bonus XP table
 *
 * This is a simple lookup table that takes a bonus XP code and a character level, returning the bonus XP awarded for killing or subduing an animal.
 * The bonus XP code is defined in the AnimalData class
 */
class CreatureBonusXpTable : public BoundIntRowLookupTable<TableColumnBonusXpCodeMatcher, AnimalData::BonusXpCode, int> {
public:

	/**
	 * @brief Constructor defining the name of the table
	 *
	 */
	CreatureBonusXpTable(std::string_view id) : BoundIntRowLookupTable(id, 1, 250) {}

	/**
	 * @brief Constructor defining the name of the table
	 *
	 * It should be noted that this class hard-codes the type of object used in the game data id.
	 */
	CreatureBonusXpTable() : CreatureBonusXpTable("CREATURE_BONUS_XP_TABLE") {}

	/**
	 * @brief In-game name of the table as used by players and NPCs
	 * @return Name of the table
	 */
	const std::string& name() const { return "Creature Bonus XP Table"; }

};
