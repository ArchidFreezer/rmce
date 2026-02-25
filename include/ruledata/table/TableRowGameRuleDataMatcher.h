#pragma once
#include <GameRuleData.h>

/**
 * @class TableRowGameRuleDataMatcher
 * @brief Class to identify the row index in a table using GameRuleData objects as the key
 * 
 * @tparam T Type of GameRuleData object that this row matches
 */
template <GameRuleDataObject T>
class TableRowGameRuleDataMatcher {
public:
	/**
	 * @brief Constructor
	 * @param game_data GameRuleData object that this row matches
	 */
	TableRowGameRuleDataMatcher(const T& game_data) : game_data_{ &game_data } {}

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
	bool matches(const T* game_data) const {
		return game_data_->id() == game_data->id();
	}

private:
	const T* game_data_{ nullptr }; /**< GameRuleData object that this row matches */
};
