#pragma once

#include <set>

#include <ClimateData.h>
#include <EnvironmentType.h>

namespace rm {

	/**
	 * @class Location
	 * @brief Definition of a specific location, consisting of a combination of the features, terrain, vegetation, water and climates that may be found there
	 *
	 * This class has two pimary uses.
	 * - The first is to define a specific location in the game world, such as a specific region, island or city.
	 * - The second is in an object that is used to match a specific location against a set of criteria. An example of this is for animals where they may be found in multiple types of
	 * location so a LocationMatcher object is used which contains both permitted and excluded criteria for the features, terrain, vegetation, water and climates where the animal may be found.
	 * In this scenario there would be two Location objects in the LocationMatcher, one for the permitted criteria and one for the excluded criteria, that are used to match against a
	 * specific location to determine if the animal may be found there.
	 */
	class Location {
	public:

		/**
		 * @brief Add an environmental feature to those that may be found in the location
		 * @param feature EnvironmentType::Feature to add
		 */
		void addFeature(rule::enums::EnvironmentType::Feature feature) { if (!hasFeature(feature)) features_.emplace(feature); }

		/**
		 * @brief Set the environmental features that may be found in the location
		 * @param features Set of EnvironmentType::Feature to set
		 */
		void setFeatures(std::set<rule::enums::EnvironmentType::Feature> features) { features_ = std::move(features); }

		/**
		 * @brief Check if an environmental feature may be found in the location
		 * @param feature EnvironmentType::Feature to check
		 * @return `true` if the location may have the feature
		 * @return `false` if the location may not have the feature
		 */
		bool hasFeature(rule::enums::EnvironmentType::Feature feature) const {
			return (features_.find(feature) != features_.end());
		}

		/**
		 * @brief Get the environmental features that may be found in the location
		 * @return std::set<EnvironmentType::Feature> features
		 */
		const std::set<rule::enums::EnvironmentType::Feature> features() const { return features_; }

		/**
		 * @brief Add a terrain type to those that may be found in the location
		 * @param terrain EnvironmentType::Terrain to add
		 */
		void addTerrain(rule::enums::EnvironmentType::Terrain terrain) { if (!hasTerrain(terrain)) terrains_.emplace(terrain); }

		/**
		 * @brief Set the terrain types that may be found in the location
		 * @param terrains Set of EnvironmentType::Terrain to set
		 */
		void setTerrains(std::set<rule::enums::EnvironmentType::Terrain> terrains) { terrains_ = std::move(terrains); }

		/**
		 * @brief Check if a terrain type may be found in the location
		 * @param terrain EnvironmentType::Terrain to check
		 * @return `true` if the location may have the terrain
		 * @return `false` if the location may not have the terrain
		 */
		bool hasTerrain(rule::enums::EnvironmentType::Terrain terrain) const {
			return (terrains_.find(terrain) != terrains_.end());
		}

		/**
		 * @brief Get the terrain types that may be found in the location
		 * @return std::set<EnvironmentType::Terrain> terrains
		 */
		const std::set<rule::enums::EnvironmentType::Terrain> terrains() const { return terrains_; }

		/**
		 * @brief Add a vegetation type to those that may be found in the location
		 * @param vegetation EnvironmentType::Vegetation to add
		 */
		void addVegetation(rule::enums::EnvironmentType::Vegetation vegetation) { if (!hasVegetation(vegetation)) vegetation_.emplace(vegetation); }

		/**
		 * @brief Set the vegetation types that may be found in the location
		 * @param vegetation Set of EnvironmentType::Vegetation to set
		 */
		void setVegetation(std::set<rule::enums::EnvironmentType::Vegetation> vegetation) { vegetation_ = std::move(vegetation); }

		/**
		 * @brief Check if a vegetation type may be found in the location
		 * @param vegetation EnvironmentType::Vegetation to check
		 * @return `true` if the location may have the vegetation
		 * @return `false` if the location may not have the vegetation
		 */
		bool hasVegetation(rule::enums::EnvironmentType::Vegetation vegetation) const {
			return (vegetation_.find(vegetation) != vegetation_.end());
		}

		/**
		 * @brief Get the vegetation types that may be found in the location
		 * @return std::set<EnvironmentType::Vegetation> vegetation
		 */
		const std::set<rule::enums::EnvironmentType::Vegetation> vegetation() const { return vegetation_; }

		/**
		 * @brief Add a water type to those that may be found in the location
		 * @param water EnvironmentType::Water to add
		 */
		void addWater(rule::enums::EnvironmentType::Water water) { if (!hasWater(water)) water_.emplace(water); }

		/**
		 * @brief Set the water types that may be found in the location
		 * @param water Set of EnvironmentType::Water to set
		 */
		void setWater(std::set<rule::enums::EnvironmentType::Water> water) { water_ = std::move(water); }

		/**
		 * @brief Check if a water type may be found in the location
		 * @param water EnvironmentType::Water to check
		 * @return `true` if the location may have the water
		 * @return `false` if the location may not have the water
		 */
		bool hasWater(rule::enums::EnvironmentType::Water water) const {
			return (water_.find(water) != water_.end());
		}

		/**
		 * @brief Get the water types that may be found in the location
		 * @return std::set<EnvironmentType::Water> water
		 */
		const std::set<rule::enums::EnvironmentType::Water> water() const { return water_; }

		/**
		 * @brief Add a climate to those that may be found in the location
		 * @param climate ClimateData* to add
		 */
		void addClimate(const rule::ClimateData* climate) { if (!hasClimate(climate)) climates_.emplace(climate); }

		/**
		 * @brief Set the climates that may be found in the location
		 * @param climates Set of ClimateData* to set
		 */
		void setClimates(std::set<const rule::ClimateData*> climates) { climates_ = std::move(climates); }

		/**
		 * @brief Check if a climate may be found in the location
		 * @param climate ClimateData* to check
		 * @return `true` if the location may have the climate
		 * @return `false` if the location may not have the climate
		 */
		bool hasClimate(const rule::ClimateData* climate) const {
			for (const auto& c : climates_) {
				if (c->id() == climate->id()) return true;
			}
			return false;
		}

		/**
		 * @brief Get the climates that may be found in the location
		 * @return std::set<const ClimateData*> climates
		 */
		const std::set<const rule::ClimateData*> climates() const { return climates_; }

		/**
		* @brief Check if a specific location matches the criteria defined in this object
		*
		* Checks whether @a specified location is considered be a subset of this location, menaning for if there is a feature, terrain, vegetation, water or climate that is defined in this location
		* the specific location must also have one defined to be considered a match. If there is no feature, terrain, vegetation, water or climate defined in this location then it is not considered
		* when matching and the specific location may or may not have it without impacting the result.
		*
		* @param specific_location Location to check against the criteria defined in this object
		* @return `true` if the location matches the criteria defined in this object
		* @return `false` if the location does not match the criteria defined in this object
		* */
		bool matches(const Location& specific_location) const;

	private:
		std::set<rule::enums::EnvironmentType::Feature> features_{}; /**< Set of environmental features that are found in the location */
		std::set<rule::enums::EnvironmentType::Terrain> terrains_{}; /**< Set of terrain types that are found in the location */
		std::set<rule::enums::EnvironmentType::Vegetation> vegetation_{}; /**< Set of vegetation types that are found in the location */
		std::set<rule::enums::EnvironmentType::Water> water_{}; /**< Set of water types that are found in the location */
		std::set<const rule::ClimateData*> climates_{}; /**< Set of climates that are found in the location */

	};

} // namespace rm