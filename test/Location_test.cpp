#include <gtest/gtest.h>

#include <Location.h>

namespace {
	TEST(LocationTest, TestFeatures) {
		Location location;
		location.addFeature(EnvironmentType::Feature::kCave);
		location.addFeature(EnvironmentType::Feature::kRuins);
		EXPECT_TRUE(location.hasFeature(EnvironmentType::Feature::kCave));
		EXPECT_TRUE(location.hasFeature(EnvironmentType::Feature::kRuins));
		EXPECT_FALSE(location.hasFeature(EnvironmentType::Feature::kEnchanted));
	}

	TEST(LocationTest, TestTerrains) {
		Location location;
		location.addTerrain(EnvironmentType::Terrain::kAlpine);
		location.addTerrain(EnvironmentType::Terrain::kRough);
		EXPECT_TRUE(location.hasTerrain(EnvironmentType::Terrain::kAlpine));
		EXPECT_TRUE(location.hasTerrain(EnvironmentType::Terrain::kRough));
		EXPECT_FALSE(location.hasTerrain(EnvironmentType::Terrain::kUnderground));
	}

	TEST(LocationTest, TestVegetation) {
		Location location;
		location.addVegetation(EnvironmentType::Vegetation::kBarren);
		location.addVegetation(EnvironmentType::Vegetation::kJungle);
		EXPECT_TRUE(location.hasVegetation(EnvironmentType::Vegetation::kBarren));
		EXPECT_TRUE(location.hasVegetation(EnvironmentType::Vegetation::kJungle));
		EXPECT_FALSE(location.hasVegetation(EnvironmentType::Vegetation::kDeciduous));
	}

	TEST(LocationTest, TestWater) {
		Location location;
		location.addWater(EnvironmentType::Water::kOasis);
		location.addWater(EnvironmentType::Water::kSaltCoast);
		EXPECT_TRUE(location.hasWater(EnvironmentType::Water::kOasis));
		EXPECT_TRUE(location.hasWater(EnvironmentType::Water::kSaltCoast));
		EXPECT_FALSE(location.hasWater(EnvironmentType::Water::kFreshCoast));
	}

	TEST(LocationTest, TestClimate) {
		Location location;
		ClimateData climate1("CLIMATE_ID_1");
		ClimateData climate2("CLIMATE_ID_2");
		ClimateData climate3("CLIMATE_ID_3");
		location.addClimate(&climate1);
		location.addClimate(&climate2);
		EXPECT_TRUE(location.hasClimate(&climate1));
		EXPECT_TRUE(location.hasClimate(&climate2));
		EXPECT_FALSE(location.hasClimate(&climate3));
	}

	TEST(LocationTest, TestDuplicate) {
		Location location;
		location.addFeature(EnvironmentType::Feature::kCave);
		location.addFeature(EnvironmentType::Feature::kCave);
		EXPECT_TRUE(location.hasFeature(EnvironmentType::Feature::kCave));
	}

	TEST(LocationTest, TestEmpty) {
		Location location;
		EXPECT_FALSE(location.hasFeature(EnvironmentType::Feature::kCave));
		EXPECT_FALSE(location.hasTerrain(EnvironmentType::Terrain::kAlpine));
		EXPECT_FALSE(location.hasVegetation(EnvironmentType::Vegetation::kBarren));
		EXPECT_FALSE(location.hasWater(EnvironmentType::Water::kOasis));
	}

	TEST(LocationTest, TestSetters) {
		Location location;
		location.setFeatures({ EnvironmentType::Feature::kCave, EnvironmentType::Feature::kRuins });
		location.setTerrains({ EnvironmentType::Terrain::kAlpine, EnvironmentType::Terrain::kRough });
		location.setVegetation({ EnvironmentType::Vegetation::kBarren, EnvironmentType::Vegetation::kJungle });
		location.setWater({ EnvironmentType::Water::kOasis, EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.hasFeature(EnvironmentType::Feature::kCave));
		EXPECT_TRUE(location.hasFeature(EnvironmentType::Feature::kRuins));
		EXPECT_FALSE(location.hasFeature(EnvironmentType::Feature::kEnchanted));
		EXPECT_TRUE(location.hasTerrain(EnvironmentType::Terrain::kAlpine));
		EXPECT_TRUE(location.hasTerrain(EnvironmentType::Terrain::kRough));
		EXPECT_FALSE(location.hasTerrain(EnvironmentType::Terrain::kUnderground));
		EXPECT_TRUE(location.hasVegetation(EnvironmentType::Vegetation::kBarren));
		EXPECT_TRUE(location.hasVegetation(EnvironmentType::Vegetation::kJungle));
		EXPECT_FALSE(location.hasVegetation(EnvironmentType::Vegetation::kDeciduous));
		EXPECT_TRUE(location.hasWater(EnvironmentType::Water::kOasis));
		EXPECT_TRUE(location.hasWater(EnvironmentType::Water::kSaltCoast));
		EXPECT_FALSE(location.hasWater(EnvironmentType::Water::kFreshCoast));
	}
} // namespace