#pragma once
#include <string>
#include <string_view>
#include <ManoeuvreDifficultyType.h>
#include <GameRuleData.h>

namespace rm::rule {

	/**
	 * @class CreaturePaceData
	 *
	 * This class represents the relative pace of the creature compared to it's base rate of movement.
	 *
	 * A creature's actual movement rate is determined by its Base Rate as modified by its pace (up to its Max Pace) and possibly a maneuver roll using its Movement and Maneuver Bonus (MM Bonus).
	 * To determine a creature's movement rate for a given round, the GM should first decide on the creature's pace and then the base movement rate can be multiplied by the movement rate multiplier.
	 *
	 * A creature's pace also determines how many exhaustion points it uses each round. A creature's exhaustion points is dependent upon its Constitution
	 *
	 * @brief Class representing data about creature paces
	 */
	class CreaturePaceData : public GameRuleData {
	public:

		CreaturePaceData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

		/**
		 * @brief Constructor
		 * @param id Unique identifier for the creature pace
		 */
		CreaturePaceData(std::string_view id) : GameRuleData(id) {}

		/**
	 * @brief Set the name of the creature pace as seen in-game
	 * @param name CreaturePace name
	 */
		void setName(std::string_view name) { name_ = name; }

		/**
		 * @brief Get the name of the creature pace as seen in-game
		 * @return CreaturePace name as a string reference
		 */
		const std::string& name() const { return name_; }

		/**
		 * @brief Set the multiplier to the base movement rate of the creature when using this pace
		 * @param movement_rate_multiplier Multiplier to the base movement rate of the creature when using this pace
		 */
		void setMovementRateMultiplier(float movement_rate_multiplier) { movement_rate_multiplier_ = movement_rate_multiplier; }

		/**
		 * @brief Get the multiplier to the base movement rate of the creature when using this pace
		 * @return Multiplier to the base movement rate of the creature when using this pace
		 */
		float movementRateMultiplier() const { return movement_rate_multiplier_; }

		/**
		 * @brief Set the multiplier to the exhaustion cost of the creature when using this pace
		 * @param exhaustion_cost_multiplier Multiplier to the exhaustion cost of the creature when using this pace
		 */
		void setExhaustionCostMultiplier(float exhaustion_cost_multiplier) { exhaustion_cost_multiplier_ = exhaustion_cost_multiplier; }

		/**
		 * @brief Get the multiplier to the exhaustion cost of the creature when using this pace
		 * @return Multiplier to the exhaustion cost of the creature when using this pace
		 */
		float exhaustionCostMultiplier() const { return exhaustion_cost_multiplier_; }

		/**
		 * @brief Set the difficulty type of any manoeuvres attempted by the creature when using this pace
		 * @param manoeuvre_difficulty_type The difficulty type of any manoeuvres attempted by the creature when using this pace
		 */
		void setManoeuvreDifficultyType(rule::enums::ManoeuvreDifficultyType::Type manoeuvre_difficulty_type) { manoeuvre_difficulty_type_ = manoeuvre_difficulty_type; }

		/**
		 * @brief Get the difficulty type of any manoeuvres attempted by the creature when using this pace
		 * @return The difficulty type of any manoeuvres attempted by the creature when using this pace
		 */
		rule::enums::ManoeuvreDifficultyType::Type manoeuvreDifficultyType() const { return manoeuvre_difficulty_type_; }

	private:
		std::string name_{}; /** Name of the creaturepace as seen in-game */
		float movement_rate_multiplier_{}; /** Multiplier to the base movement rate of the creature when using this pace */
		float exhaustion_cost_multiplier_{}; /** Multiplier to the exhaustion cost of the creature when using this pace */
		rule::enums::ManoeuvreDifficultyType::Type manoeuvre_difficulty_type_{}; /** The difficulty type of any manoeuvres attempted by the creature when using this pace */
	};

} // namespace rm::rule