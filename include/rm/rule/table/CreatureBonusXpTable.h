#pragma once
#include <CreatureBonusXpType.h>
#include <table/BoundIntRowLookupTable.h>

namespace rm {

	/**
	 * @class TableColumnBonusXpCodeMatcher
	 * @brief Class to match the bonus XP code to a table column index
	 *
	 * This is used as the column matcher for the CreatureBonusXpTable class.It takes a BonusXpCode and returns the corresponding column index for that code.
	 */
	class TableColumnBonusXpCodeMatcher {
	public:
		/**
		 * @brief Get the index of the table column
		 * @return int table column index
		 */
		int column(rule::enums::CreatureBonusXpType::Type match) const {
			switch (match) {
			case rule::enums::CreatureBonusXpType::Type::kNone: return 0;
			case rule::enums::CreatureBonusXpType::Type::kA: return 1;
			case rule::enums::CreatureBonusXpType::Type::kB: return 2;
			case rule::enums::CreatureBonusXpType::Type::kC: return 3;
			case rule::enums::CreatureBonusXpType::Type::kD: return 4;
			case rule::enums::CreatureBonusXpType::Type::kE: return 5;
			case rule::enums::CreatureBonusXpType::Type::kF: return 6;
			case rule::enums::CreatureBonusXpType::Type::kG: return 7;
			case rule::enums::CreatureBonusXpType::Type::kH: return 8;
			case rule::enums::CreatureBonusXpType::Type::kI: return 9;
			case rule::enums::CreatureBonusXpType::Type::kJ: return 10;
			case rule::enums::CreatureBonusXpType::Type::kK: return 11;
			case rule::enums::CreatureBonusXpType::Type::kL: return 12;
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
	class CreatureBonusXpTable : public BoundIntRowLookupTable<TableColumnBonusXpCodeMatcher, rule::enums::CreatureBonusXpType::Type, int> {
	public:

		/**
		 * @brief Constructor defining the name of the table
		 *
		 */
		CreatureBonusXpTable(std::string_view id) : BoundIntRowLookupTable(id, 1, 250) {}

		/**
		 * @brief In-game name of the table as used by players and NPCs
		 * @return Name of the table
		 */
		const std::string& name() const { return "Creature Bonus XP Table"; }

	};

} // namespace rm