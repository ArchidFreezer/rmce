#pragma once

#include <GameRuleData.h>
#include <TreasureValueType.h>

namespace rm::rule {

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
		void setItemsValueType(rule::enums::TreasureValueType::Type items_value_type) { items_value_type_ = items_value_type; }

		/**
		 * @brief Get the relative value of items in a treasure pile
		 * @return Relative value of items in a treasure pile
		 */
		rule::enums::TreasureValueType::Type itemsValueType() const { return items_value_type_; }

		/**
		 * @brief Set the relative value of wealth in a treasure pile
		 * @param wealth_value_type Relative value of wealth in a treasure pile
		 */
		void setWealthValueType(rule::enums::TreasureValueType::Type wealth_value_type) { wealth_value_type_ = wealth_value_type; }

		/**
		 * @brief Get the relative value of wealth in a treasure pile
		 * @return Relative value of wealth in a treasure pile
		 */
		rule::enums::TreasureValueType::Type wealthValueType() const { return wealth_value_type_; }

	private:
		rule::enums::TreasureValueType::Type items_value_type_{}; /**< Relative value of items in a treasure pile */
		rule::enums::TreasureValueType::Type wealth_value_type_{}; /**< Relative value of wealth in a treasure pile */
	};

} // namespace rm::rule