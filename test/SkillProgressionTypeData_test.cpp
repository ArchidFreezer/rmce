#include <gtest/gtest.h>

#include "SkillProgressionTypeData.h"

namespace {

  // Demonstrate some basic assertions.
  TEST(SkillProgressionData, Constructor) {
    SkillProgressionTypeData spd("COMBINED_ID");
		spd.setName("Combined");
    EXPECT_EQ(0, strcmp(spd.name().c_str(), "Combined"));
		spd.setZero(-30.0);
    EXPECT_EQ(spd.zero(), -30.0);
		spd.setTen(5.0);
    EXPECT_EQ(spd.ten(), 5.0);
		spd.setTwenty(3.0);
    EXPECT_EQ(spd.twenty(), 3.0);
		spd.setThirty(1.5);
    EXPECT_EQ(spd.thirty(), 1.5);
		spd.setRemaining(0.5);
    EXPECT_EQ(spd.remaining(), 0.5);
  }

  TEST(SkillProgressionData, GetBonusCombined) {
    SkillProgressionTypeData spd("COMBINED_ID");
    spd.setName("Combined");
    spd.setZero(-30.0);
    spd.setTen(5.0);
    spd.setTwenty(3.0);
    spd.setThirty(1.5);
    spd.setRemaining(0.5);
    EXPECT_EQ(spd.remaining(), 0.5);
    EXPECT_EQ(spd.bonus(0), -30);
    EXPECT_EQ(spd.bonus(7), 35);
    EXPECT_EQ(spd.bonus(17), 71);
    EXPECT_EQ(spd.bonus(27), 90);
    EXPECT_EQ(spd.bonus(37), 98);
  }
}