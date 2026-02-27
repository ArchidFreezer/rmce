#pragma once
#include <GameRuleData.h>

namespace rm::rule {

	/**
	 * @class TableRowGameRuleDataMatcher
	 * @brief Class to identify the row index in a table using GameRuleData objects as the key
	 */
	class TableRowGameRuleDataMatcher {
	public:

		/**
		 * @brief Constructor
		 * @param game_data GameRuleData object that this row matches
		 */
		TableRowGameRuleDataMatcher(const GameRuleData& game_data) : game_data_{ &game_data } {}

		/**
		 * @brief Implement an operator< method for the class
		 *
		 * This allows the class to be used in ordered collections such as the key in a `std::map`
		 *
		 * @param other TableRowGameRuleDataMatcher to compare against
		 * @return `true` if this object is considered less than \a other
		 * @return `false` if this object is not considered less than \a other
		 */
		bool operator<(const TableRowGameRuleDataMatcher& other) const {
			return game_data_->id() < other.game_data_->id();
		}

		/**
		 * @brief Determines if the value lies in the range or not.
		 *
		 * The check is performed against the bound min and max.
		 *
		 * @param game_data GameRuleData object to check is matched by this row
		 * @return `true` if \a game_data is matched by this row;
		 * @return `false` if \a game_data is not matched by this row;
		 */
		bool matches(const GameRuleData* game_data) const {
			return game_data_->id() == game_data->id();
		}

		/**
		 * @brief Get the GameRuleData object that this row matches
		 * @return Pointer to the GameRuleData object that this row matches
		 */
		const GameRuleData* gameData() const { return game_data_; }

	private:
		const GameRuleData* game_data_{ nullptr }; /**< GameRuleData object that this row matches */
	};

} // namespace rm::rule