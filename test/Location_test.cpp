#include <gtest/gtest.h>

#include <Location.h>

using namespace rm;
using game::Location;

namespace {
	TEST(LocationTest, TestFeatures) {
		Location location;
		location.addFeature(rule::enums::EnvironmentType::Feature::kCave);
		location.addFeature(rule::enums::EnvironmentType::Feature::kRuins);
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kRuins));
		EXPECT_FALSE(location.hasFeature(rule::enums::EnvironmentType::Feature::kEnchanted));
	}

	TEST(LocationTest, TestTerrains) {
		Location location;
		location.addTerrain(rule::enums::EnvironmentType::Terrain::kAlpine);
		location.addTerrain(rule::enums::EnvironmentType::Terrain::kRough);
		EXPECT_TRUE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kAlpine));
		EXPECT_TRUE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kRough));
		EXPECT_FALSE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kUnderground));
	}

	TEST(LocationTest, TestVegetation) {
		Location location;
		location.addVegetation(rule::enums::EnvironmentType::Vegetation::kBarren);
		location.addVegetation(rule::enums::EnvironmentType::Vegetation::kJungle);
		EXPECT_TRUE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kBarren));
		EXPECT_TRUE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kJungle));
		EXPECT_FALSE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kDeciduous));
	}

	TEST(LocationTest, TestWater) {
		Location location;
		location.addWater(rule::enums::EnvironmentType::Water::kOasis);
		location.addWater(rule::enums::EnvironmentType::Water::kSaltCoast);
		EXPECT_TRUE(location.hasWater(rule::enums::EnvironmentType::Water::kOasis));
		EXPECT_TRUE(location.hasWater(rule::enums::EnvironmentType::Water::kSaltCoast));
		EXPECT_FALSE(location.hasWater(rule::enums::EnvironmentType::Water::kFreshCoast));
	}

	TEST(LocationTest, TestClimate) {
		Location location;
		rule::ClimateData climate1("CLIMATE_ID_1");
		rule::ClimateData climate2("CLIMATE_ID_2");
		rule::ClimateData climate3("CLIMATE_ID_3");
		location.addClimate(&climate1);
		location.addClimate(&climate2);
		EXPECT_TRUE(location.hasClimate(&climate1));
		EXPECT_TRUE(location.hasClimate(&climate2));
		EXPECT_FALSE(location.hasClimate(&climate3));
	}

	TEST(LocationTest, TestDuplicate) {
		Location location;
		location.addFeature(rule::enums::EnvironmentType::Feature::kCave);
		location.addFeature(rule::enums::EnvironmentType::Feature::kCave);
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
	}

	TEST(LocationTest, TestEmpty) {
		Location location;
		EXPECT_FALSE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
		EXPECT_FALSE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kAlpine));
		EXPECT_FALSE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kBarren));
		EXPECT_FALSE(location.hasWater(rule::enums::EnvironmentType::Water::kOasis));
	}

	TEST(LocationTest, TestSetters) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kRuins));
		EXPECT_FALSE(location.hasFeature(rule::enums::EnvironmentType::Feature::kEnchanted));
		EXPECT_TRUE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kAlpine));
		EXPECT_TRUE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kRough));
		EXPECT_FALSE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kUnderground));
		EXPECT_TRUE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kBarren));
		EXPECT_TRUE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kJungle));
		EXPECT_FALSE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kDeciduous));
		EXPECT_TRUE(location.hasWater(rule::enums::EnvironmentType::Water::kOasis));
		EXPECT_TRUE(location.hasWater(rule::enums::EnvironmentType::Water::kSaltCoast));
		EXPECT_FALSE(location.hasWater(rule::enums::EnvironmentType::Water::kFreshCoast));
	}

	TEST(LocationTest, TestEmptySetters) {
		Location location;
		location.setFeatures({});
		location.setTerrains({});
		location.setVegetation({});
		location.setWater({});
		EXPECT_FALSE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
		EXPECT_FALSE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kAlpine));
		EXPECT_FALSE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kBarren));
		EXPECT_FALSE(location.hasWater(rule::enums::EnvironmentType::Water::kOasis));
	}

	TEST(LocationTest, TestMatcher) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		EXPECT_FALSE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherEmpty) {
		Location location;
		Location specific_location;
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		EXPECT_TRUE(location.matches(specific_location));
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherPartial) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		EXPECT_FALSE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kRuins });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherMultiple) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		EXPECT_FALSE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherSubset) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherSuperset) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherNoMatch) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kUnderground });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kDeciduous });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kFreshCoast });
		EXPECT_FALSE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherMatchNoFeature) {
		Location location;
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Location specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherEmptySpecific) {
		Location location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Location specific_location;
		EXPECT_FALSE(location.matches(specific_location));
	}
} // namespace