#pragma once

#include <set>

#include <ClimateType.h>
#include <EnvironmentType.h>

namespace rm::game {

/**
 * @class Habitat
 * @brief Definition of athe characteristics of a habitat, consisting of a combination of the features, terrain, vegetation, water and climates that may be found there
 *
 * This class has two primary uses.
 * - The first is to define characteristics of a specific habitat in the game world, such as a specific region, island, city, mountain range, etc.
 * - The second is to define the natural habitat of fauna or flora
 * 
 * These may then be compared so the habitat of a specific animal may be compoared against the habitaat(s) in a locationm to determine if the animal has a chance of being found there.
 */
class Habitat {
public:
	/**
	 * @brief Struct to define which environmental features, terrain, vegetation and water are required for a habitat to match
	 *
	 * This is used in conjunction with the Location::matches() method to determine if a specific habitat matches the criteria defined in this object.
	 */
	struct RequiredEnvironments {
		bool feature{false};    /**< Whether a feature is required for the habitat to match */
		bool terrain{false};    /**< Whether a terrain is required for the habitat to match */
		bool vegetation{false}; /**< Whether a vegetation is required for the habitat to match */
		bool water{false};      /**< Whether a water is required for the habitat to match */
	};

	/**
	 * @brief Add an environmental feature to those that may be found in the habitat
	 * @param feature EnvironmentType::Feature to add
	 */
	void addFeature(rule::enums::EnvironmentType::Feature feature) {
		if (!hasFeature(feature))
			features_.emplace(feature);
	}

	/**
	 * @brief Set the environmental features that may be found in the habitat
	 * @param features Set of EnvironmentType::Feature to set
	 */
	void setFeatures(std::set<rule::enums::EnvironmentType::Feature> features) {
		features_ = std::move(features);
	}

	/**
	 * @brief Check if an environmental feature may be found in the habitat
	 * @param feature EnvironmentType::Feature to check
	 * @return `true` if the habitat may have the feature
	 * @return `false` if the habitat may not have the feature
	 */
	bool hasFeature(rule::enums::EnvironmentType::Feature feature) const {
		return (features_.find(feature) != features_.end());
	}

	/**
	 * @brief Get the environmental features that may be found in the habitat
	 * @return std::set<EnvironmentType::Feature> features
	 */
	const std::set<rule::enums::EnvironmentType::Feature> features() const {
		return features_;
	}

	/**
	 * @brief Add a terrain type to those that may be found in the habitat
	 * @param terrain EnvironmentType::Terrain to add
	 */
	void addTerrain(rule::enums::EnvironmentType::Terrain terrain) {
		if (!hasTerrain(terrain))
			terrains_.emplace(terrain);
	}

	/**
	 * @brief Set the terrain types that may be found in the habitat
	 * @param terrains Set of EnvironmentType::Terrain to set
	 */
	void setTerrains(std::set<rule::enums::EnvironmentType::Terrain> terrains) {
		terrains_ = std::move(terrains);
	}

	/**
	 * @brief Check if a terrain type may be found in the habitat
	 * @param terrain EnvironmentType::Terrain to check
	 * @return `true` if the habitat may have the terrain
	 * @return `false` if the habitat may not have the terrain
	 */
	bool hasTerrain(rule::enums::EnvironmentType::Terrain terrain) const {
		return (terrains_.find(terrain) != terrains_.end());
	}

	/**
	 * @brief Get the terrain types that may be found in the habitat
	 * @return std::set<EnvironmentType::Terrain> terrains
	 */
	const std::set<rule::enums::EnvironmentType::Terrain> terrains() const {
		return terrains_;
	}

	/**
	 * @brief Add a vegetation type to those that may be found in the habitat
	 * @param vegetation EnvironmentType::Vegetation to add
	 */
	void addVegetation(rule::enums::EnvironmentType::Vegetation vegetation) {
		if (!hasVegetation(vegetation))
			vegetation_.emplace(vegetation);
	}

	/**
	 * @brief Set the vegetation types that may be found in the habitat
	 * @param vegetation Set of EnvironmentType::Vegetation to set
	 */
	void setVegetation(std::set<rule::enums::EnvironmentType::Vegetation> vegetation) {
		vegetation_ = std::move(vegetation);
	}

	/**
	 * @brief Check if a vegetation type may be found in the habitat
	 * @param vegetation EnvironmentType::Vegetation to check
	 * @return `true` if the habitat may have the vegetation
	 * @return `false` if the habitat may not have the vegetation
	 */
	bool hasVegetation(rule::enums::EnvironmentType::Vegetation vegetation) const {
		return (vegetation_.find(vegetation) != vegetation_.end());
	}

	/**
	 * @brief Get the vegetation types that may be found in the habitat
	 * @return std::set<EnvironmentType::Vegetation> vegetation
	 */
	const std::set<rule::enums::EnvironmentType::Vegetation> vegetation() const {
		return vegetation_;
	}

	/**
	 * @brief Add a water type to those that may be found in the habitat
	 * @param water EnvironmentType::Water to add
	 */
	void addWater(rule::enums::EnvironmentType::Water water) {
		if (!hasWater(water))
			water_.emplace(water);
	}

	/**
	 * @brief Set the water types that may be found in the habitat
	 * @param water Set of EnvironmentType::Water to set
	 */
	void setWater(std::set<rule::enums::EnvironmentType::Water> water) {
		water_ = std::move(water);
	}

	/**
	 * @brief Check if a water type may be found in the habitat
	 * @param water EnvironmentType::Water to check
	 * @return `true` if the habitat may have the water
	 * @return `false` if the habitat may not have the water
	 */
	bool hasWater(rule::enums::EnvironmentType::Water water) const {
		return (water_.find(water) != water_.end());
	}

	/**
	 * @brief Get the water types that may be found in the habitat
	 * @return std::set<EnvironmentType::Water> water
	 */
	const std::set<rule::enums::EnvironmentType::Water> water() const {
		return water_;
	}

	/**
	 * @brief Add a climate group to those that may be found in the habitat
	 * @param group ClimateType::KoppenGroup to add
	 */
	void addClimateGroup(rule::enums::ClimateType::KoppenGroup group) {
		if (!hasClimateGroup(group))
			climate_groups_.emplace(group);
	}

	/**
	 * @brief Set the climate groups that may be found in the habitat
	 * @param groups Set of ClimateType::KoppenGroup to set
	 */
	void setClimateGroups(std::set<rule::enums::ClimateType::KoppenGroup> groups) {
		climate_groups_ = std::move(groups);
	}

	/**
	 * @brief Check if a climate group may be found in the habitat
	 * @param group ClimateType::KoppenGroup to check
	 * @return `true` if the habitat may have the climate group
	 * @return `false` if the habitat may not have the climate group
	 */
	bool hasClimateGroup(rule::enums::ClimateType::KoppenGroup group) const {
		return (climate_groups_.find(group) != climate_groups_.end());
	}

	/**
	 * @brief Get the climate groups that may be found in the habitat
	 * @return std::set<ClimateType::KoppenGroup> climate groups
	 */
	const std::set<rule::enums::ClimateType::KoppenGroup> climateGroups() const {
		return climate_groups_;
	}

	/**
	 * @brief Add a climate subgroup to those that may be found in the habitat
	 * @param subGroup ClimateType::KoppenSubGroup to add
	 */
	void addClimateSubGroup(rule::enums::ClimateType::KoppenSubGroup subGroup) {
		if (!hasClimateSubGroup(subGroup))
			climate_sub_groups_.emplace(subGroup);
	}

	/**
	 * @brief Set the climate subgroups that may be found in the habitat
	 * @param subGroups Set of ClimateType::KoppenSubGroup to set
	 */
	void setClimateSubGroups(std::set<rule::enums::ClimateType::KoppenSubGroup> subGroups) {
		climate_sub_groups_ = std::move(subGroups);
	}

	/**
	 * @brief Check if a climate subgroup may be found in the habitat
	 * @param subGroup ClimateType::KoppenSubGroup to check
	 * @return `true` if the habitat may have the climate subgroup
	 * @return `false` if the habitat may not have the climate subgroup
	 */
	bool hasClimateSubGroup(rule::enums::ClimateType::KoppenSubGroup subGroup) const {
		return (climate_sub_groups_.find(subGroup) != climate_sub_groups_.end());
	}

	/**
	 * @brief Get the climate subgroups that may be found in the habitat
	 * @return std::set<ClimateType::KoppenSubGroup> climate subgroups
	 */
	const std::set<rule::enums::ClimateType::KoppenSubGroup> climateSubGroups() const {
		return climate_sub_groups_;
	}

	/**
	 * @brief Check if a specific habitat matches the criteria defined in this object
	 *
	 * Checks whether @a specified habitat is considered be a subset of this habitat, menaning for if there is a feature, terrain, vegetation, water or climate that is defined in this habitat
	 * the specific habitat must also have one defined to be considered a match. If there is no feature, terrain, vegetation, water or climate defined in this habitat then it is not considered
	 * when matching and the specific habitat may or may not have it without impacting the result.
	 *
	 * @param specific_location Location to check against the criteria defined in this object
	 * @return `true` if the habitat matches the criteria defined in this object
	 * @return `false` if the habitat does not match the criteria defined in this object
	 * */
	bool matches(const Habitat& specific_location) const;

	/**
	 * @brief Check if a specific habitat matches the climate criteria defined in this object
	 *
	 * Checks whether @a specified habitat is considered be a subset of this habitat in terms of climate, meaning if there is a climate group or subgroup defined in this habitat,
	 * the specific habitat must also have one defined to be considered a match. If there is no climate group or subgroup defined in this habitat then it is not considered
	 * when matching and the specific habitat may or may not have it without impacting the result.
	 *
	 * @param specific_location Location to check against the climate criteria defined in this object
	 * @return `true` if the habitat matches the climate criteria defined in this object
	 * @return `false` if the habitat does not match the climate criteria defined in this object
	 * */
	bool matchesClimate(const Habitat& specific_location) const;

	/**
	 * @brief Check if a specific habitat matches the environmental criteria defined in this object
	 *
	 * Checks whether @a specified habitat is considered be a subset of this habitat in terms of environmental features, terrain, vegetation and water, meaning if there is a feature, terrain,
	 * vegetation or water defined in this habitat, the specific habitat must also have one defined to be considered a match. If there is no feature, terrain, vegetation or water defined in this
	 * habitat then it is not considered when matching and the specific habitat may or may not have it without impacting the result.
	 *
	 * The @a required_environments parameter is used to specify which of the environmental features, terrain, vegetation and water are required for a habitat to match. If a specific habitat does not meet the required criteria, it will
	 * not be considered a match. The use of this parameter is based on the intersection of both the current habitat and the @a specific habitat.
	 *
	 * If all values are set to 'false' then there must be one matching environmental feature, terrain, vegetation or water for the habitat to match, but the grouping that it is in is not important. If any of the values are set to 'true'
	 * then there must be at least one matching environmental element from that group. If multiple values are set to 'true' then there must be at least one matching environmental element from each of those groups for the habitat to match.
	 *
	 * @param specific_location Location to check against the environmental criteria defined in this object
	 * @param required_environments RequiredEnvironments struct defining which environmental features, terrain, vegetation and water are required for a habitat to match
	 * @return `true` if the habitat matches the environmental criteria defined in this object
	 * @return `false` if the habitat does not match the environmental criteria defined in this object
	 * */
	bool matchesEnvironment(const Habitat& specific_location, RequiredEnvironments required_environments) const;

	/**
	 * @brief Check if a specific habitat matches the criteria defined in this object
	 *
	 * Checks whether @a specified habitat is considered be a subset of this habitat, meaning for if there is a feature, terrain, vegetation, water or climate that is defined in this habitat
	 * the specific habitat must also have one defined to be considered a match. If there is no feature, terrain, vegetation, water or climate defined in this habitat then it is not considered
	 * when matching and the specific habitat may or may not have it without impacting the result.
	 *
	 * The @a required_environments parameter is used to specify which of the environmental features, terrain, vegetation and water are required for a habitat to match. If a specific habitat does not meet the required criteria, it will
	 * not be considered a match. The use of this parameter is based on the intersection of both the current habitat and the @a specific habitat.
	 *
	 * If all values are set to 'false' then there must be one matching environmental feature, terrain, vegetation or water for the habitat to match, but the grouping that it is in is not important. If any of the values are set to 'true'
	 * then there must be at least one matching environmental element from that group. If multiple values are set to 'true' then there must be at least one matching environmental element from each of those groups for the habitat to match.
	 *
	 * @param specific_location Location to check against the criteria defined in this object
	 * @param required_environments RequiredEnvironments struct defining which environmental features, terrain, vegetation and water are required for a habitat to match
	 * @return `true` if the habitat matches the criteria defined in this object
	 * @return `false` if the habitat does not match the criteria defined in this object
	 * */
	bool matches(const Habitat& specific_location, RequiredEnvironments required_environments) const;

private:
	std::set<rule::enums::EnvironmentType::Feature> features_{};              /**< Set of environmental features that are found in the habitat */
	std::set<rule::enums::EnvironmentType::Terrain> terrains_{};              /**< Set of terrain types that are found in the habitat */
	std::set<rule::enums::EnvironmentType::Vegetation> vegetation_{};         /**< Set of vegetation types that are found in the habitat */
	std::set<rule::enums::EnvironmentType::Water> water_{};                   /**< Set of water types that are found in the habitat */
	std::set<rule::enums::ClimateType::KoppenGroup> climate_groups_{};        /**< Set of climate groups that are found in the habitat */
	std::set<rule::enums::ClimateType::KoppenSubGroup> climate_sub_groups_{}; /**< Set of climate subgroups that are found in the habitat */
};

} // namespace rm::game