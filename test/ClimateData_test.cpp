#include <gtest/gtest.h>

#include <ClimateData.h>

namespace {
	TEST(ClimateData, General) {
		ClimateData c1("CLIM1_ID");
		c1.setName("Cold and temperate");
		c1.setTemperature(HabitatType::Temperature::kCold);
		c1.addPrecipitation(HabitatType::Precipitation::kTemperate);

		EXPECT_STREQ(c1.name().c_str(), "Cold and temperate");
		EXPECT_EQ(c1.temperature(), HabitatType::Temperature::kCold);
		EXPECT_TRUE(c1.hasPrecipitaton(HabitatType::Precipitation::kTemperate));
		EXPECT_FALSE(c1.hasPrecipitaton(HabitatType::Precipitation::kArid));
		EXPECT_TRUE(c1.isValid(HabitatType::Temperature::kCold, HabitatType::Precipitation::kTemperate));
		EXPECT_FALSE(c1.isValid(HabitatType::Temperature::kWarm, HabitatType::Precipitation::kTemperate));
		EXPECT_FALSE(c1.isValid(HabitatType::Temperature::kCold, HabitatType::Precipitation::kRainy));

		c1.addPrecipitation(HabitatType::Precipitation::kRainy);
		EXPECT_TRUE(c1.isValid(HabitatType::Temperature::kCold, HabitatType::Precipitation::kRainy));
	}
}