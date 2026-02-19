#include <Location.h>

bool Location::matches(const Location& specific_location) const {
	// If there are any features make sure one is present in the location, if there are no features then this is not a criteria and does not impact the matching
	if (!features_.empty()) {
		bool feature_valid{ false };
		for (const auto& feature : features_) {
			if (specific_location.hasFeature(feature)) {
				feature_valid = true;
				break;
			}
		}
		if (!feature_valid) return false;
	}

	// If there are any terrains make sure one is present in the location, if there are no terrains then this is not a criteria and does not impact the matching
	if (!terrains_.empty()) {
		bool terrain_valid{ false };
		for (const auto& terrain : terrains_) {
			if (specific_location.hasTerrain(terrain)) {
				terrain_valid = true;
				break;
			}
		}
		if (!terrain_valid) return false;
	}

	// If there are any vegetation make sure one is present in the location, if there are no vegetation then this is not a criteria and does not impact the matching
	if (!vegetation_.empty()) {
		bool vegetation_valid{ false };
		for (const auto& vegetation : vegetation_) {
			if (specific_location.hasVegetation(vegetation)) {
				vegetation_valid = true;
				break;
			}
		}
		if (!vegetation_valid) return false;
	}

	// If there are any water sources make sure one is present in the location, if there are no water sources then this is not a criteria and does not impact the matching
	if (!water_.empty()) {
		bool water_valid{ false };
		for (const auto& water : water_) {
			if (specific_location.hasWater(water)) {
				water_valid = true;
				break;
			}
		}
		if (!water_valid) return false;
	}

	// If there are any climates make sure one is present in the location, if there are no climates then this is not a criteria and does not impact the matching
	if (!climates_.empty()) {
		bool climate_valid{ false };
		for (const auto& climate : climates_) {
			if (specific_location.hasClimate(climate)) {
				climate_valid = true;
				break;
			}
		}
		if (!climate_valid) return false;
	}

	// If we have not failed any of the criteria then the location matches
	return true;
}