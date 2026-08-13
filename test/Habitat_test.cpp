#include <gtest/gtest.h>

#include <Habitat.h>

using namespace rm;
using game::Habitat;

namespace {
	TEST(LocationTest, TestFeatures) {
		Habitat location;
		location.addFeature(rule::enums::EnvironmentType::Feature::kCave);
		location.addFeature(rule::enums::EnvironmentType::Feature::kRuins);
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kRuins));
		EXPECT_FALSE(location.hasFeature(rule::enums::EnvironmentType::Feature::kEnchanted));
	}

	TEST(LocationTest, TestTerrains) {
		Habitat location;
		location.addTerrain(rule::enums::EnvironmentType::Terrain::kAlpine);
		location.addTerrain(rule::enums::EnvironmentType::Terrain::kRough);
		EXPECT_TRUE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kAlpine));
		EXPECT_TRUE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kRough));
		EXPECT_FALSE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kUnderground));
	}

	TEST(LocationTest, TestVegetation) {
		Habitat location;
		location.addVegetation(rule::enums::EnvironmentType::Vegetation::kBarren);
		location.addVegetation(rule::enums::EnvironmentType::Vegetation::kJungle);
		EXPECT_TRUE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kBarren));
		EXPECT_TRUE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kJungle));
		EXPECT_FALSE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kDeciduous));
	}

	TEST(LocationTest, TestWater) {
		Habitat location;
		location.addWater(rule::enums::EnvironmentType::Water::kOasis);
		location.addWater(rule::enums::EnvironmentType::Water::kSaltCoast);
		EXPECT_TRUE(location.hasWater(rule::enums::EnvironmentType::Water::kOasis));
		EXPECT_TRUE(location.hasWater(rule::enums::EnvironmentType::Water::kSaltCoast));
		EXPECT_FALSE(location.hasWater(rule::enums::EnvironmentType::Water::kFreshCoast));
	}

	TEST(LocationTest, TestClimate) {
		Habitat location;
		location.addClimateGroup(rule::enums::ClimateType::KoppenGroup::kTropical);
		location.addClimateGroup(rule::enums::ClimateType::KoppenGroup::kDry);
		EXPECT_TRUE(location.hasClimateGroup(rule::enums::ClimateType::KoppenGroup::kTropical));
		EXPECT_TRUE(location.hasClimateGroup(rule::enums::ClimateType::KoppenGroup::kDry));
		EXPECT_FALSE(location.hasClimateGroup(rule::enums::ClimateType::KoppenGroup::kTemperate));
	}

	TEST(LocationTest, TestDuplicate) {
		Habitat location;
		location.addFeature(rule::enums::EnvironmentType::Feature::kCave);
		location.addFeature(rule::enums::EnvironmentType::Feature::kCave);
		EXPECT_TRUE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
	}

	TEST(LocationTest, TestEmpty) {
		Habitat location;
		EXPECT_FALSE(location.hasFeature(rule::enums::EnvironmentType::Feature::kCave));
		EXPECT_FALSE(location.hasTerrain(rule::enums::EnvironmentType::Terrain::kAlpine));
		EXPECT_FALSE(location.hasVegetation(rule::enums::EnvironmentType::Vegetation::kBarren));
		EXPECT_FALSE(location.hasWater(rule::enums::EnvironmentType::Water::kOasis));
	}

	TEST(LocationTest, TestSetters) {
		Habitat location;
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
		Habitat location;
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
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Habitat specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		EXPECT_FALSE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherEmpty) {
		Habitat location;
		Habitat specific_location;
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		EXPECT_TRUE(location.matches(specific_location));
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherPartial) {
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Habitat specific_location;
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
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Habitat specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.matches(specific_location));
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		EXPECT_FALSE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherSubset) {
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		Habitat specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherSuperset) {
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Habitat specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherNoMatch) {
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Habitat specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kEnchanted });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kUnderground });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kDeciduous });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kFreshCoast });
		EXPECT_FALSE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherMatchNoFeature) {
		Habitat location;
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Habitat specific_location;
		specific_location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave, rule::enums::EnvironmentType::Feature::kRuins });
		specific_location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine, rule::enums::EnvironmentType::Terrain::kRough });
		specific_location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren, rule::enums::EnvironmentType::Vegetation::kJungle });
		specific_location.setWater({ rule::enums::EnvironmentType::Water::kOasis, rule::enums::EnvironmentType::Water::kSaltCoast });
		EXPECT_TRUE(location.matches(specific_location));
	}

	TEST(LocationTest, TestMatcherEmptySpecific) {
		Habitat location;
		location.setFeatures({ rule::enums::EnvironmentType::Feature::kCave });
		location.setTerrains({ rule::enums::EnvironmentType::Terrain::kAlpine });
		location.setVegetation({ rule::enums::EnvironmentType::Vegetation::kBarren });
		location.setWater({ rule::enums::EnvironmentType::Water::kOasis });
		Habitat specific_location;
		EXPECT_FALSE(location.matches(specific_location));
	}
} // namespace