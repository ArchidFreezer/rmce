#include <gtest/gtest.h>

#include <SkillDevelopmentCost.h>

namespace {
	TEST(SkillDevelopmentCost, General) {
		SkillDevelopmentCost c0{};
		EXPECT_FALSE(c0.first());
		EXPECT_FALSE(c0.second());
		EXPECT_FALSE(c0.third());
		EXPECT_FALSE(c0.fourth());
		EXPECT_STREQ(c0.toString().c_str(), "");

		SkillDevelopmentCost c1(3);
		EXPECT_TRUE(c1.first());
		EXPECT_EQ(c1.first().value(), 3);
		EXPECT_FALSE(c1.second());
		EXPECT_FALSE(c1.third());
		EXPECT_FALSE(c1.fourth());
		EXPECT_STREQ(c1.toString().c_str(), "3");

		SkillDevelopmentCost c2(3, 4);
		EXPECT_TRUE(c2.first());
		EXPECT_EQ(c2.first().value(), 3);
		EXPECT_TRUE(c2.second());
		EXPECT_EQ(c2.second().value(), 4);
		EXPECT_FALSE(c2.third());
		EXPECT_FALSE(c2.fourth());
		EXPECT_STREQ(c2.toString().c_str(), "3:4");

		SkillDevelopmentCost c3(3, 4, 5);
		EXPECT_TRUE(c3.first());
		EXPECT_EQ(c3.first().value(), 3);
		EXPECT_TRUE(c3.second());
		EXPECT_EQ(c3.second().value(), 4);
		EXPECT_TRUE(c3.third());
		EXPECT_EQ(c3.third().value(), 5);
		EXPECT_FALSE(c3.fourth());
		EXPECT_STREQ(c3.toString().c_str(), "3:4:5");

		SkillDevelopmentCost c4(3, 4, 5, 6);
		EXPECT_TRUE(c4.first());
		EXPECT_EQ(c4.first().value(), 3);
		EXPECT_TRUE(c4.second());
		EXPECT_EQ(c4.second().value(), 4);
		EXPECT_TRUE(c4.third());
		EXPECT_EQ(c4.third().value(), 5);
		EXPECT_TRUE(c4.fourth());
		EXPECT_EQ(c4.fourth().value(), 6);
		EXPECT_STREQ(c4.toString().c_str(), "3:4:5:6");

		SkillDevelopmentCost c5("");
		EXPECT_FALSE(c5.first());
		EXPECT_FALSE(c5.second());
		EXPECT_FALSE(c5.third());
		EXPECT_FALSE(c5.fourth());
		EXPECT_STREQ(c5.toString().c_str(), "");

		SkillDevelopmentCost c6("3");
		EXPECT_TRUE(c6.first());
		EXPECT_EQ(c6.first().value(), 3);
		EXPECT_FALSE(c6.second());
		EXPECT_FALSE(c6.third());
		EXPECT_FALSE(c6.fourth());
		EXPECT_STREQ(c6.toString().c_str(), "3");

		SkillDevelopmentCost c7("3:4");
		EXPECT_TRUE(c7.first());
		EXPECT_EQ(c7.first().value(), 3);
		EXPECT_TRUE(c7.second());
		EXPECT_EQ(c7.second().value(), 4);
		EXPECT_FALSE(c7.third());
		EXPECT_FALSE(c7.fourth());
		EXPECT_STREQ(c7.toString().c_str(), "3:4");

		SkillDevelopmentCost c8("3:4:5");
		EXPECT_TRUE(c8.first());
		EXPECT_EQ(c8.first().value(), 3);
		EXPECT_TRUE(c8.second());
		EXPECT_EQ(c8.second().value(), 4);
		EXPECT_TRUE(c8.third());
		EXPECT_EQ(c8.third().value(), 5);
		EXPECT_FALSE(c8.fourth());
		EXPECT_STREQ(c8.toString().c_str(), "3:4:5");

		SkillDevelopmentCost c9("3:4:5:6");
		EXPECT_TRUE(c9.first());
		EXPECT_EQ(c9.first().value(), 3);
		EXPECT_TRUE(c9.second());
		EXPECT_EQ(c9.second().value(), 4);
		EXPECT_TRUE(c9.third());
		EXPECT_EQ(c9.third().value(), 5);
		EXPECT_TRUE(c9.fourth());
		EXPECT_EQ(c9.fourth().value(), 6);
		EXPECT_STREQ(c9.toString().c_str(), "3:4:5:6");

	}
}