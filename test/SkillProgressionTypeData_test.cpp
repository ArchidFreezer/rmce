#include <gtest/gtest.h>

#include "SkillProgressionTypeData.h"

namespace {

  // Demonstrate some basic assertions.
  TEST(SkillProgressionData, Constructor) {
    SkillProgressionTypeData spd("COMBINED_ID", "Combined", -30.0, 5.0, 3.0, 1.5, 0.5);
    EXPECT_EQ(0, strcmp(spd.name().c_str(), "Combined"));
    EXPECT_EQ(spd.zero(), -30.0);
    EXPECT_EQ(spd.ten(), 5.0);
    EXPECT_EQ(spd.twenty(), 3.0);
    EXPECT_EQ(spd.thirty(), 1.5);
    EXPECT_EQ(spd.remaining(), 0.5);
  }

  TEST(SkillProgressionData, GetBonusCombined) {
    SkillProgressionTypeData spd("COMBINED_ID", "Combined", -30.0, 5.0, 3.0, 1.5, 0.5);
    EXPECT_EQ(spd.bonus(0), -30);
    EXPECT_EQ(spd.bonus(7), 35);
    EXPECT_EQ(spd.bonus(17), 71);
    EXPECT_EQ(spd.bonus(27), 90);
    EXPECT_EQ(spd.bonus(37), 98);
  }
}