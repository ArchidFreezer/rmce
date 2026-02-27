#pragma once

#include <set>
#include <string_view>

#include <GameRuleData.h>
#include <HabitatType.h>

using namespace rm::rule::enums;

namespace rm::rule {

	/**
	 * @class ClimateData
	 * @brief Definition of a specific climate, consisting of a combination of the rainfall patterns and temperature
	 */
	class ClimateData : public GameRuleData {
	public:
		/**
		 * @brief Default constructor is deleted to ensure the base class is initialised
		 */
		ClimateData() = delete;

		/**
		 * @brief Constructor to initialise base class
		 * @param id Unique identifier for the climate
		 */
		ClimateData(std::string_view id) : GameRuleData(id) {};

		/**
		 * @brief Set the name of the climate
		 * @param name Climate name
		 */
		void setName(std::string_view name) { name_ = name; }

		/**
		 * @brief Get the name of the climate
		 * @return Climate name as a string reference
		 */
		const std::string& name() const { return name_; }

		/**
		 * @brief Set the climate temperature
		 * @param temperature HabitatType::Temperature to set
		 */
		void setTemperature(HabitatType::Temperature temperature) { temperature_ = temperature; }

		/**
		 * @brief Get the tempretaure of the climate
		 * @return HabitatType::Temperature temperature
		 */
		HabitatType::Temperature temperature() const { return temperature_; }

		/**
		 * @brief Add a precipitation to those that the climate may have
		 * @param precipitation HabitatType::Precipitation to add
		 */
		void addPrecipitation(HabitatType::Precipitation precipitation) { if (!hasPrecipitaton(precipitation)) precipitations_.emplace(precipitation); }

		/**
		 * @brief Set the precipitations that may be found in the climate
		 * @param precipitations Set of HabitatType::Precipitation to set
		 */
		void setPrecipitations(std::set<HabitatType::Precipitation> precipitations) { precipitations_ = std::move(precipitations); }

		/**
		 * @brief Check if a precipitation may be found in the cliamte
		 * @param precipitation HabitatType::Precipitation precipitation to check
		 * @return `true` if the climate may have the precipitation
		 * @return `false` if the climate may not have the precipitation
		 */
		bool hasPrecipitaton(HabitatType::Precipitation precipitation) const {
			return (precipitations_.find(precipitation) != precipitations_.end());
		}

		/**
		 * @brief Get the precipitations that may be found in the climate
		 * @return std::set<HabitatType::Precipitation> precipitations
		 */
		const std::set<HabitatType::Precipitation> precipitations() const { return precipitations_; }

		/**
		 * @brief Checks if a tempretaure and precipitation cobination may be found in the climate
		 * @param temperature HabitatType::Temperature temperature to check
		 * @param precipitation HabitatType::Precipitation precipitation to check
		 * @return `true` if the combination are valid for the climate
		 * @return `false` if the combination are not valid for the climate
		 */
		bool isValid(HabitatType::Temperature temperature, HabitatType::Precipitation precipitation) const {
			return (temperature == temperature_) && hasPrecipitaton(precipitation);
		}

	private:
		std::string name_{}; /**< In game name of the climate */
		std::set<HabitatType::Precipitation> precipitations_{}; /**< Set of possible precipitations that define the climate */
		HabitatType::Temperature temperature_{}; /**< General temperature of the climate */
	};

} // namespace rm::rule