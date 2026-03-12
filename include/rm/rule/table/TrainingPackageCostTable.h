#pragma once

#include <table/LookupTable.h>
#include <table/TableColumnProfessionMatcher.h>
#include <table/PersistentMatcher.h>
#include <TrainingPackageData.h>
#include <string_view>

namespace rm::rule::table {

/**
 * @class TrainingPackageCostTable
 * @brief Table for training package costs, using LookupTable as base
 */
class TrainingPackageCostTable : public LookupTable<PersistentMatcher, const TrainingPackageData*, TableColumnProfessionMatcher, const ProfessionData*, int> {
public:
	/**
	 * @brief Default constructor deleted
	 */
	TrainingPackageCostTable() = delete;

	/**
	 * @brief Constructor defining bounds
	 * @param id Unique identifier for the table
	 */
	explicit TrainingPackageCostTable(std::string_view id) : LookupTable<PersistentMatcher, const TrainingPackageData*, TableColumnProfessionMatcher, const ProfessionData*, int>(id) {
	}

	inline static std::string prefix_{"TRAININGPACKAGECOSTTABLE"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return TrainingPackageCostTable::prefix_;
	}
};

} // namespace rm::rule::table