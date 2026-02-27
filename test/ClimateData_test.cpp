#include <gtest/gtest.h>

#include <ClimateData.h>

using namespace rm;

namespace {
	TEST(ClimateData, General) {
		ClimateData c1("CLIM1_ID");
		c1.setName("Cold and temperate");
		c1.setTemperature(rule::enums::HabitatType::Temperature::kCold);
		c1.addPrecipitation(rule::enums::HabitatType::Precipitation::kTemperate);

		EXPECT_STREQ(c1.name().c_str(), "Cold and temperate");
		EXPECT_EQ(c1.temperature(), rule::enums::HabitatType::Temperature::kCold);
		EXPECT_TRUE(c1.hasPrecipitaton(rule::enums::HabitatType::Precipitation::kTemperate));
		EXPECT_FALSE(c1.hasPrecipitaton(rule::enums::HabitatType::Precipitation::kArid));
		EXPECT_TRUE(c1.isValid(rule::enums::HabitatType::Temperature::kCold, rule::enums::HabitatType::Precipitation::kTemperate));
		EXPECT_FALSE(c1.isValid(rule::enums::HabitatType::Temperature::kWarm, rule::enums::HabitatType::Precipitation::kTemperate));
		EXPECT_FALSE(c1.isValid(rule::enums::HabitatType::Temperature::kCold, rule::enums::HabitatType::Precipitation::kRainy));

		c1.addPrecipitation(rule::enums::HabitatType::Precipitation::kRainy);
		EXPECT_TRUE(c1.isValid(rule::enums::HabitatType::Temperature::kCold, rule::enums::HabitatType::Precipitation::kRainy));
	}
}