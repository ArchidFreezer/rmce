#pragma once

#include <PersistentObjectManager.h>

namespace rm::rule::parser {
/**
 * @class FixedTableCreator
 * @brief Creates the lookup tables that are not populated from data files, but are instead populated from hardcoded values in the code.
 */
class FixedTableCreator {
public:

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 */
	FixedTableCreator(rm::PersistentObjectManager& object_manager) : object_manager_{object_manager} {
	}

	~FixedTableCreator() = default;                                           /** Default destructor */
	FixedTableCreator(const FixedTableCreator&) = delete;                     /** Deleted copy constructor */
	FixedTableCreator& operator=(const FixedTableCreator&) noexcept = delete; /** Deleted copy assignment operator */
	FixedTableCreator(FixedTableCreator&&) = delete;                          /** Deleted move constructor */
	FixedTableCreator& operator=(FixedTableCreator&&) noexcept = delete;      /** Deleted move assignment operator */

	/**
	 * @brief Build the lookup tables
	 *
	 * This is used to build the lookup tables that are not populated from data files, but are instead populated from hardcoded values in the code.
	 */
	void createFixedTables() {
		buildCreatureBonusXpTable();
		buildLevelVarianceTable();
	}

private:
	rm::PersistentObjectManager& object_manager_;

	/**
	 * @brief Build the lookup table that maps the bonus XP code to a bonus XP value
	 *
	 * This is used to determine how much bonus XP is awarded for killing an animal based on its bonus XP code.
	 */
	void buildCreatureBonusXpTable();

	/**
	 * @brief Build the lookup table that maps the level variance code to a difference in the level of the animal from the average
	 */

	void buildLevelVarianceTable();
};

} // namespace rm::rule::parser
