#include <gtest/gtest.h>

#include <LocationMatcher.h>

namespace {
	TEST(LocationMatcherTest, Matches) {
		Location required;
		required.addFeature(EnvironmentType::Feature::kCave);
		required.addTerrain(EnvironmentType::Terrain::kRough);
		required.addVegetation(EnvironmentType::Vegetation::kDeciduous);
		required.addVegetation(EnvironmentType::Vegetation::kGrasslands);
		required.addWater(EnvironmentType::Water::kLake);
		ClimateData cl_reqd("CLIMATE_REQD_ID");
		required.addClimate(&cl_reqd);

		Location excluded;
		excluded.addFeature(EnvironmentType::Feature::kDimention);
		excluded.addTerrain(EnvironmentType::Terrain::kWaste);
		excluded.addVegetation(EnvironmentType::Vegetation::kBarren);
		excluded.addWater(EnvironmentType::Water::kOcean);
		ClimateData cl_excl("CLIMATE_EXCL_ID");
		excluded.addClimate(&cl_excl);
		LocationMatcher location_matcher(required, excluded);

		Location location;
		location.addFeature(EnvironmentType::Feature::kCave);
		location.addTerrain(EnvironmentType::Terrain::kRough);
		location.addVegetation(EnvironmentType::Vegetation::kDeciduous);
		location.addWater(EnvironmentType::Water::kLake);
		location.addClimate(&cl_reqd);
		EXPECT_TRUE(location_matcher.matches(location));
		location.addFeature(EnvironmentType::Feature::kDimention);
		EXPECT_FALSE(location_matcher.matches(location));
	}

	TEST(LocationMatcherTest, MatchesNoRequired) {
		Location required;
		Location excluded;
		excluded.addFeature(EnvironmentType::Feature::kDimention);
		excluded.addTerrain(EnvironmentType::Terrain::kWaste);
		excluded.addVegetation(EnvironmentType::Vegetation::kBarren);
		excluded.addWater(EnvironmentType::Water::kOcean);
		ClimateData cl_excl("CLIMATE_EXCL_ID");
		excluded.addClimate(&cl_excl);
		LocationMatcher location_matcher(required, excluded);
		Location location;
		location.addFeature(EnvironmentType::Feature::kCave);
		location.addTerrain(EnvironmentType::Terrain::kRough);
		location.addVegetation(EnvironmentType::Vegetation::kDeciduous);
		location.addWater(EnvironmentType::Water::kLake);
		EXPECT_TRUE(location_matcher.matches(location));
		location.addFeature(EnvironmentType::Feature::kDimention);
		EXPECT_FALSE(location_matcher.matches(location));
	}

	TEST(LocationMatcherTest, MatchesNoExcluded) {
		Location required;
		required.addFeature(EnvironmentType::Feature::kCave);
		required.addTerrain(EnvironmentType::Terrain::kRough);
		required.addVegetation(EnvironmentType::Vegetation::kDeciduous);
		required.addWater(EnvironmentType::Water::kLake);
		ClimateData cl_reqd("CLIMATE_REQD_ID");
		required.addClimate(&cl_reqd);
		Location excluded;
		LocationMatcher location_matcher(required, excluded);
		Location location;
		location.addFeature(EnvironmentType::Feature::kCave);
		location.addTerrain(EnvironmentType::Terrain::kRough);
		location.addVegetation(EnvironmentType::Vegetation::kDeciduous);
		location.addWater(EnvironmentType::Water::kLake);
		location.addClimate(&cl_reqd);
		EXPECT_TRUE(location_matcher.matches(location));
		location.addFeature(EnvironmentType::Feature::kDimention);
		EXPECT_TRUE(location_matcher.matches(location));
	}
} // namespace