#pragma once
#include <AttackSizeType.h>
#include <table/BoundIntRowLookupTable.h>
#include <table/TableColumnArmourTypeMatcher.h>

/**
 * @class SpecialAttackTable
 * @brief Class to contain a game special attack table
 *
 * There are several attack tables in the game, mostly animal attacks, that take a dice roll is used for the rows and
 * creature ArmourType::Type is used for the columns. The difference between these and the normal AttackTable is that the
 * these tables also have limit on the highest modified roll that sizes of attacks may use. The AttackSizeType::Type is
 * generally based on the creature size, but is not always an exact match.
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
class SpecialAttackTable : public BoundIntRowLookupTable<TableColumnArmourTypeMatcher, ArmourType::Type, std::string> {
public:
	/**
	 * @brief Default constructor deleted to enforce setting the id
	 */
	SpecialAttackTable() = delete;

	/**
	 * @brief Constructor defining the name of the table and the attack size limits

	 * It should be noted that this class hard-codes the type of object used in the game data id.
	 * @param id std::string Unique identifier of the table, typically the table name
	 * @param small Maximum roll that AttackSizeType::Type.kSmall attacks may make
	 * @param medium Maximum roll that AttackSizeType::Type.kMedium attacks may make
	 * @param large Maximum roll that AttackSizeType::Type.kLarge attacks may make
	 * @param huge Maximum roll that AttackSizeType::Type.kHuge attacks may make
	 */
	SpecialAttackTable(std::string_view id, int small, int medium, int large, int huge) : SpecialAttackTable(id) {
		limits_.emplace(AttackSizeType::kSmall, small);
		limits_.emplace(AttackSizeType::kMedium, medium);
		limits_.emplace(AttackSizeType::kLarge, large);
		limits_.emplace(AttackSizeType::kHuge, huge);
	}

	/**
	 * @brief Constructor defining the name of the table and the attack size limits

	 * It should be noted that this class hard-codes the type of object used in the game data id.
	 * @param id std::string Unique identifier of the table, typically the table name
	 * @param limits Map containing the attack size limits
	 */
	SpecialAttackTable(std::string_view id, std::map<AttackSizeType::Type, int>& limits) : SpecialAttackTable(id) {
		for (auto& limit : limits) {
			limits_.emplace(limit.first, limit.second);
		}
	}
		
	/**
	 * @brief Gets the value of a cell in the table
	 *
	 * Constrains the value of the \a row_index parameter to be within the bounds set on the table so that it will never be larger
	 * than the maximum bound and never smaller than the minimum bound.
	 *
	 * @param armour Armour type the attack is against
	 * @param size Attack size that is being performed
	 * @param row_index Value to identify the row, must be of the same type as the matcher
	 *
	 * @return Contents of the cell from the row that matches \a row_index and column \a col_index
	 *
	 * @throw RowNotFoundException if \a row_index does not match any matchers
	 * @throw ColNotFoundException if \a col_index is an invalid column
	 */
	const std::string& cell(ArmourType::Type armour, AttackSizeType::Type size, int row_index) const {
		row_index = std::min(limits_.find(size)->second, row_index);
		return LookupTable<NumberRange<int>, int, TableColumnArmourTypeMatcher, ArmourType::Type, std::string>::cell(armour, row_index);
	}

	/**
 * @brief Gets the value of a cell in the table
 *
 * Constrains the value of the \a row_index parameter to be within the bounds set on the table so that it will never be larger
 * than the maximum bound and never smaller than the minimum bound.
 *
 * @param armour Armour type the attack is against
 * @param size Attack size that is being performed
 * @param row_index Value to match against the row
 * @param unmodified_row_index Value to match against the rows that are for unmodified values
 *
 * @return Contents of the cell from the row that matches the indexes
 *
 * @throw RowNotFoundException if neither \a row_index nor \a unmodified_row_index are matched
 * @throw ColNotFoundException if \a col_index is an invalid column
 */
	const std::string& cell(ArmourType::Type armour, AttackSizeType::Type size, int row_index, int unmodified_row_index) const {
		row_index = std::min(limits_.find(size)->second, row_index);
		unmodified_row_index = std::min(limits_.find(size)->second, unmodified_row_index);
		return LookupTable<NumberRange<int>, int, TableColumnArmourTypeMatcher, ArmourType::Type, std::string>::cell(armour, row_index, unmodified_row_index);
	}

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

	/**
	 * @brief Get the maximum roll allowed for an attack size
	 * @param type AttakcSizeType to get the limit for
	 * @return int Maximum roll allowed
	 */
	const int limit(AttackSizeType::Type type) { return limits_.at(type); }

private:
	std::string name_{}; /**< Name of the attack table */

	std::map<AttackSizeType::Type, int> limits_{}; /**< The maximum row index that each attack size may use */

	/**
	 * @brief Constructor defining the name of the table
	 *
	 * This is private to enforce the use of a constructor that sets the attack size limits
	 * @param id std::string Unique identifier of the table, typically the table name
	 */
	SpecialAttackTable(std::string_view id) : BoundIntRowLookupTable(generateId("SpecialAttackTable", id), 0, 150) {}
};
