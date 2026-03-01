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
		explicit TrainingPackageCostTable(std::string_view id)
			: LookupTable<PersistentMatcher, const TrainingPackageData*, TableColumnProfessionMatcher, const ProfessionData*, int>(id) {}
	};

} // namespace rm::rule::table