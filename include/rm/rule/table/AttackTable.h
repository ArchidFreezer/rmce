#pragma once
#include <table/BoundIntRowLookupTable.h>
#include <table/TableColumnArmourTypeMatcher.h>

namespace rm::rule {

	/**
	 * @class AttackTable
	 * @brief Class to contain a game attack table
	 *
	 * There are many attack tables in the game, such as for weapons and spells, that take a dice roll is used for the rows and
	 * creature ArmourType::Type is used for the columns.
	 *
	 * Has the ability to handle rows that may only be used on unmodified rolls and these will be checked before any check on
	 * the normal rows that are used to match attack rolls with modifiers applied.
	 *
	 * Defines the minimum and maximum roll value that the table has rows for. If a roll given that is outside these values the
	 * min and max values will be used instead. This permits open ended roll results be used against the table and have a row
	 * returned without being concerned about the extents of the table rows.
	 *
	 * Defines the type of game data object and the creation of an instance is used to set the unique element of the id. The
	 * implication of this is that any class that derives from this cannot set its own subtype.
	 */
	class AttackTable : public BoundIntRowLookupTable<TableColumnArmourTypeMatcher, rule::enums::ArmourType::Type, std::string> {
	public:
		/**
		 * @brief Constructor defining the name of the table
		 *
		 * It should be noted that this class hard-codes the type of object used in the game data id.
		 * @param id std::string Unique identifier of the table, typically the table name
		 */
		AttackTable(std::string id) : AttackTable(id, 150) {}

		/**
	 * @brief Constructor defining the name of the table
	 *
	 * It should be noted that this class hard-codes the type of object used in the game data id.
	 * @param id std::string Unique identifier of the table, typically the table name
	 * @param max_rows The maximum roll that the table has a row matcher for
	 */
		AttackTable(std::string id, int max_rows) : BoundIntRowLookupTable(id, 0, max_rows) {}

		/**
		 * @brief Set the name of the table
		 * @param name table name
		 */
		void setName(std::string_view name) { name_ = name; }
		/**
		 * @brief In-game name of the table as used by players and NPCs
		 * @return Name of the table
		 */
		const std::string& name() const { return name_; }

	private:
		std::string name_{}; /**< Name of the attack table */
	};

} // namespace rmrm::rule