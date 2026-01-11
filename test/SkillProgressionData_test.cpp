#include <gtest/gtest.h>

#include "SkillProgressionData.h"

namespace {

  SkillProgressionData spd(SkillProgressionType::Type::kCombined);

  // Demonstrate some basic assertions.
  TEST(SkillProgressionData, Constructor) {
    SkillProgressionData spd(SkillProgressionType::Type::kCombined);
    EXPECT_EQ(0, strcmp(spd.id().c_str(), "Combined"));
  }

  TEST(SkillProgressionData, GetBonusCombined) {
    SkillProgressionData spd(SkillProgressionType::Type::kCombined);
    EXPECT_EQ(spd.getBonus(0), -30);
    EXPECT_EQ(spd.getBonus(7), 35);
    EXPECT_EQ(spd.getBonus(17), 71);
    EXPECT_EQ(spd.getBonus(27), 90);
    EXPECT_EQ(spd.getBonus(37), 98);
  }
}