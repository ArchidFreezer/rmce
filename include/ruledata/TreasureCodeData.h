#pragma once

#include <GameRuleData.h>
#include <TreasureValueType.h>

/**
 * @brief Class to represent the data for a treasure code
 * 
 * A treasure code is a set of rules that determine the relative value of items and wealth in a treasure pile.
 */
class TreasureCodeData : public GameRuleData {
public:
	/**
	 * @brief Default constructor is deleted to ensure the base class is initialised
	 */
	TreasureCodeData() = delete;

	/**
	 * @brief Constructor to initialise base class
	 * @param id Unique identifier for the treasure code
	 */
	TreasureCodeData(std::string_view id) : GameRuleData(id) {};

	/**
	 * @brief Set the relative value of items in a treasure pile
	 * @param items_value_type Relative value of items in a treasure pile
	 */
	void setItemsValueType(TreasureValueType::Type items_value_type) { items_value_type_ = items_value_type; }

	/**
	 * @brief Get the relative value of items in a treasure pile
	 * @return Relative value of items in a treasure pile
	 */
	TreasureValueType::Type itemsValueType() const { return items_value_type_; }

	/**
	 * @brief Set the relative value of wealth in a treasure pile
	 * @param wealth_value_type Relative value of wealth in a treasure pile
	 */
	void setWealthValueType(TreasureValueType::Type wealth_value_type) { wealth_value_type_ = wealth_value_type; }

	/**
	 * @brief Get the relative value of wealth in a treasure pile
	 * @return Relative value of wealth in a treasure pile
	 */
	TreasureValueType::Type wealthValueType() const { return wealth_value_type_; }

private:
	TreasureValueType::Type items_value_type_{}; /**< Relative value of items in a treasure pile */
	TreasureValueType::Type wealth_value_type_{}; /**< Relative value of wealth in a treasure pile */
};
