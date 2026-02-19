#pragma once

#include <Location.h>

/**
 * @class LocationMatcher
 * @brief Object used to match a specific location against a set of criteria. An example of this is for animals where they may be found in multiple types of
 * location so a LocationMatcher object is used which contains both permitted and excluded criteria for the features, terrain, vegetation, water and climates where the animal may be found.
 * In this scenario there would be two Location objects in the LocationMatcher, one for the permitted criteria and one for the excluded criteria, that are used to match against a
 * specific Location to determine if the animal may be found there.
 */
class LocationMatcher {
public:
	/**
	 * @brief Constructor
	 * @param required Location containing the criteria for the features, terrain, vegetation, water and climates that must be found in the location
	 * @param excluded Location containing the criteria for the features, terrain, vegetation, water and climates that may not be found in the location
	 */
	LocationMatcher(Location required, Location excluded) : required_(std::move(required)), excluded_(std::move(excluded)) {}

	/**
	 * @brief Check if a specific location matches the criteria defined in this object
	 * @param location Location to check against the criteria defined in this object
	 * @return `true` if the location matches the criteria defined in this object
	 * @return `false` if the location does not match the criteria defined in this object
	 */
	bool matches(const Location& location) const;

private:
	Location required_; /**< Location containing the criteria for the features, terrain, vegetation, water and climates that must be found in the location */
	Location excluded_; /**< Location containing the criteria for the features, terrain, vegetation, water and climates that may not be found in the location */
};
