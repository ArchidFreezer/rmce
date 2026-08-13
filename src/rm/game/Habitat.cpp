#include <Habitat.h>

namespace rm::game {

bool Habitat::matchesClimate(const Habitat& specific_location) const {
	// If there are any climate groups make sure one is present in the habitat, if there are no climate groups then this is not a criteria and does not impact the matching
	if (!climate_groups_.empty()) {
		bool climate_group_valid{false};
		for (const auto& climate_group : climate_groups_) {
			// We use this to prevent unnecessary checks if we have already found a match for the group in the specific habitat subgroups
			if (climate_group_valid)
				break;

			if (specific_location.hasClimateGroup(climate_group)) {
				climate_group_valid = true;
				break;
			} else if (!specific_location.climateSubGroups().empty()) {
				// If haven't found a specific group match we need to check the subgroups in the specific habitat as one of those may match the more general criteria of the group in this habitat.
				for (const auto& climate_sub_group : specific_location.climateSubGroups()) {
					if (climate_group == climate_sub_group) {
						climate_group_valid = true;
						break;
					}
				}
			}
		}

		if (!climate_group_valid)
			return false;
	}

	// If there are any climate subgroups make sure one is present in the habitat, if there are no climate subgroups then this is not a criteria and does not impact the matching
	if (!climate_sub_groups_.empty()) {
		bool climate_sub_group_valid{false};
		for (const auto& climate_sub_group : climate_sub_groups_) {
			if (specific_location.hasClimateSubGroup(climate_sub_group)) {
				climate_sub_group_valid = true;
				break;
			}
		}
		if (!climate_sub_group_valid)
			return false;
	}

	return true;
}

bool Habitat::matchesEnvironment(const Habitat& specific_location, RequiredEnvironments required_environments) const {
	// If there are any features and one is required make sure there is a match, if there are no features then this is not a criteria and does not impact the matching
	if (!features_.empty()) {
		bool feature_valid{false};
		for (const auto& feature : features_) {
			if (specific_location.hasFeature(feature)) {
				feature_valid = true;
				break;
			}
		}
		if (required_environments.feature && !feature_valid)
			return false;
	}

	// If there are any terrains make sure one is present in the habitat, if there are no terrains then this is not a criteria and does not impact the matching
	if (!terrains_.empty()) {
		bool terrain_valid{false};
		for (const auto& terrain : terrains_) {
			if (specific_location.hasTerrain(terrain)) {
				terrain_valid = true;
				break;
			}
		}
		if (required_environments.terrain && !terrain_valid)
			return false;
	}

	// If there are any vegetation make sure one is present in the habitat, if there are no vegetation then this is not a criteria and does not impact the matching
	if (!vegetation_.empty()) {
		bool vegetation_valid{false};
		for (const auto& vegetation : vegetation_) {
			if (specific_location.hasVegetation(vegetation)) {
				vegetation_valid = true;
				break;
			}
		}
		if (required_environments.vegetation && !vegetation_valid)
			return false;
	}

	// If there are any water sources make sure one is present in the habitat, if there are no water sources then this is not a criteria and does not impact the matching
	if (!water_.empty()) {
		bool water_valid{false};
		for (const auto& water : water_) {
			if (specific_location.hasWater(water)) {
				water_valid = true;
				break;
			}
		}
		if (required_environments.water && !water_valid)
			return false;
	}

	return true;
}

bool Habitat::matches(const Habitat& specific_location, RequiredEnvironments required_environments) const {
	bool matches_environment = matchesEnvironment(specific_location, required_environments);
	if (!matches_environment)
		return false;
	bool matches_climate = matchesClimate(specific_location);
	if (!matches_climate)
		return false;
	// If we have not failed any of the criteria then the habitat matches
	return true;
}

bool Habitat::matches(const Habitat& specific_location) const {
	return matches(specific_location, RequiredEnvironments{false, false, false, false});
}


} // namespace rm::game