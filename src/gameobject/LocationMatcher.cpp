#include <LocationMatcher.h>

bool LocationMatcher::matches(const Location& location) const {

	// If there are any required features make sure one is present in the location, if there are no required features then this is not a criteria and the location is valid
	if (!required_.features().empty()) {
		bool feature_valid{ false };
		for (const auto& feature : required_.features()) {
			if (location.hasFeature(feature)) {
				feature_valid = true;
				break;
			}
		}
		if (!feature_valid) return false;
	}

	// If there are any required terrains make sure one is present in the location, if there are no required terrains then this is not a criteria and the location is valid
	if (!required_.terrains().empty()) {
		bool terrain_valid{ false };
		for (const auto& terrain : required_.terrains()) {
			if (location.hasTerrain(terrain)) {
				terrain_valid = true;
				break;
			}
		}
		if (!terrain_valid) return false;
	}

	// If there are any required vegetation make sure one is present in the location, if there are no required vegetation then this is not a criteria and the location is valid
	if (!required_.vegetation().empty()) {
		bool vegetation_valid{ false };
		for (const auto& vegetation : required_.vegetation()) {
			if (location.hasVegetation(vegetation)) {
				vegetation_valid = true;
				break;
			}
		}
		if (!vegetation_valid) return false;
	}

	// If there are any required water sources make sure one is present in the location, if there are no required water sources then this is not a criteria and the location is valid
	if (!required_.water().empty()) {
		bool water_valid{ false };
		for (const auto& water : required_.water()) {
			if (location.hasWater(water)) {
				water_valid = true;
				break;
			}
		}
		if (!water_valid) return false;
	}

	// If there are any required climates make sure one is present in the location, if there are no required climates then this is not a criteria and the location is valid
	if (!required_.climates().empty()) {
		bool climate_valid{ false };
		for (const auto& climate : required_.climates()) {
			if (location.hasClimate(climate)) {
				climate_valid = true;
				break;
			}
		}
		if (!climate_valid) return false;
	}

	// Check that the location does not have any of the excluded features, terrain, vegetation, water and climates
	for (const auto& feature : excluded_.features()) {
		if (location.hasFeature(feature)) return false;
	}
	for (const auto& terrain : excluded_.terrains()) {
		if (location.hasTerrain(terrain)) return false;
	}
	for (const auto& vegetation : excluded_.vegetation()) {
		if (location.hasVegetation(vegetation)) return false;
	}
	for (const auto& water : excluded_.water()) {
		if (location.hasWater(water)) return false;
	}
	for (const auto& climate : excluded_.climates()) {
		if (location.hasClimate(climate)) return false;
	}

	// If we have got here then the location has passed all of the criteria checks and is valid
	return true;
}