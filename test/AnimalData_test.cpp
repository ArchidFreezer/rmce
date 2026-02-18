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

	// The followimg member function is private and thus cannot be tested directly but the output can be tested through the public interface.
	//TEST(AnimalDataTest, HitsPerLevelDifference) {
	//	AnimalData animal("TEST_ANIMAL");
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kNone);
	//	EXPECT_EQ(animal.hitsPerLevelDifference(), 0);
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kC);
	//	EXPECT_EQ(animal.hitsPerLevelDifference(), 3);
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kH);
	//	EXPECT_EQ(animal.hitsPerLevelDifference(), 15);
	//}

	// The followimg member function is private and thus cannot be tested directly but the output can be tested through the public interface.
	//TEST(AnimalDataTest, BonusExhaustionPoints) {
	//	AnimalData animal("TEST_ANIMAL");
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kNone);
	//	EXPECT_EQ(animal.bonusExhaustionPoints(), 0);
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kD);
	//	EXPECT_EQ(animal.bonusExhaustionPoints(), 0);
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kE);
	//	EXPECT_EQ(animal.bonusExhaustionPoints(), 50);
	//	animal.setBonusConstitutionCode(CreatureBonusConstitutionType::Type::kH);
	//	EXPECT_EQ(animal.bonusExhaustionPoints(), 200);
	//}
}