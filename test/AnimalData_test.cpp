#include <gtest/gtest.h>
#include <AnimalData.h>

namespace {
	TEST(AnimalDataTest, HuntingModifier) {
		AnimalData animal("TEST_ANIMAL");
		animal.setFrequencyFactor(1);
		EXPECT_EQ(animal.huntingModifier(), 30);
		animal.setFrequencyFactor(5);
		EXPECT_EQ(animal.huntingModifier(), -10);
		animal.setFrequencyFactor(9);
		EXPECT_EQ(animal.huntingModifier(), -70);
	}
	TEST(AnimalDataTest, ExistencePercentageChance) {
		AnimalData animal("TEST_ANIMAL");
		animal.setFrequencyFactor(1);
		EXPECT_EQ(animal.existencePercentageChance(), 100);
		animal.setFrequencyFactor(5);
		EXPECT_EQ(animal.existencePercentageChance(), 40);
		animal.setFrequencyFactor(9);
		EXPECT_EQ(animal.existencePercentageChance(), 4);
	}
}