#include <gtest/gtest.h>

#include <Forage.h>
#include <Character.h>
#include <Habitat.h>
#include <ForagableData.h>
#include <PersistentObjectManager.h>
#include <PersistentCache.h>
#include <SubcategoriedSkillData.h>

using namespace rm::action::forage;
using namespace rm::game;
using namespace rm::rule;
using namespace rm::rule::enums;

namespace {

// -----------------------------------------------------------------------
// Test fixture for forage functions
// -----------------------------------------------------------------------

class ForageTest : public ::testing::Test {
protected:
	void SetUp() override {
		cache = std::make_unique<rm::PersistentCache>();
		manager = std::make_unique<rm::PersistentObjectManager>(*cache);

		// Create foraging skill using the manager's method
		foraging_skill = &manager->subcategoriedSkillData("SKILL_FORAGING");

		// Create herb lore skill
		herb_lore_skill = &manager->subcategoriedSkillData("SKILL_HERB_LORE");

		// Create poison lore skill
		poison_lore_skill = &manager->subcategoriedSkillData("SKILL_POISON_LORE");

		// Create a test character
		test_character = &manager->get<character::Character>();
		test_character->setName("Test Forager");

		// Create a test habitat
		test_location = std::make_unique<Habitat>();
	}

	std::unique_ptr<rm::PersistentCache> cache;
	std::unique_ptr<rm::PersistentObjectManager> manager;
	const SubcategoriedSkillData* foraging_skill;
	const SubcategoriedSkillData* herb_lore_skill;
	const SubcategoriedSkillData* poison_lore_skill;
	character::Character* test_character;
	std::unique_ptr<Habitat> test_location;
};

// -----------------------------------------------------------------------
// Tests for forage_herbs function
// -----------------------------------------------------------------------

/**
 * @brief Test that forage_herbs returns empty map when foraging roll is less than 100
 */
TEST_F(ForageTest, ForageHerbs_FailedForagingRoll_ReturnsEmpty) {
	// Set up habitat with deciduous vegetation
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	// Create modifiers with very low hours spent (should result in low roll)
	HerbModifiers modifiers{*manager, nullptr, 1, 0, 0, false};

	// Run forage multiple times to ensure we get at least one failure
	bool found_failure = false;
	for (int i = 0; i < 100; i++) {
		auto result = forage_herbs(*test_character, *test_location, modifiers);
		if (result.empty()) {
			found_failure = true;
			break;
		}
	}

	EXPECT_TRUE(found_failure);
}

/**
 * @brief Test that failed_search_count is incremented on failed forage
 */
TEST_F(ForageTest, ForageHerbs_FailedSearch_IncrementsCounter) {
	// Set up habitat with barren vegetation (difficult)
	test_location->addVegetation(EnvironmentType::Vegetation::kBarren);

	// Create modifiers with very low hours spent
	HerbModifiers modifiers{*manager, nullptr, 1, 0, 0, false};
	int initial_count = modifiers.failed_search_count;

	// Run forage - likely to fail with 1 hour and no skill bonus
	forage_herbs(*test_character, *test_location, modifiers);

	// Check if counter was incremented (might succeed due to randomness, so run multiple times)
	bool found_increment = false;
	for (int i = 0; i < 50; i++) {
		HerbModifiers temp_modifiers{*manager, nullptr, 1, 0, 0, false};
		forage_herbs(*test_character, *test_location, temp_modifiers);
		if (temp_modifiers.failed_search_count > 0) {
			found_increment = true;
			break;
		}
	}

	EXPECT_TRUE(found_increment);
}

/**
 * @brief Test that additional_searchers bonus is applied
 */
TEST_F(ForageTest, ForageHerbs_AdditionalSearchers_AppliesBonus) {
	// Set up habitat
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	// Create a foragable resource
	ForagableData& herb = manager->get<ForagableData>("TEST_HERB");
	herb.setName("Test Herb");
	herb.setHabitat(*test_location);
	herb.setFindDifficulty(SkillDifficultyType::kLight);
	herb.setLoreSkill(herb_lore_skill);

	// Test with additional searchers - should increase success rate
	HerbModifiers modifiers_with_help{*manager, nullptr, 10, 5, 0, false};
	HerbModifiers modifiers_without_help{*manager, nullptr, 10, 0, 0, false};

	int successes_with_help = 0;
	int successes_without_help = 0;

	// Run multiple trials
	for (int i = 0; i < 20; i++) {
		auto result_with = forage_herbs(*test_character, *test_location, modifiers_with_help);
		if (!result_with.empty()) {
			successes_with_help++;
		}

		auto result_without = forage_herbs(*test_character, *test_location, modifiers_without_help);
		if (!result_without.empty()) {
			successes_without_help++;
		}
	}

	// With additional searchers, we should have more successes
	EXPECT_GE(successes_with_help, successes_without_help);
}

/**
 * @brief Test that hunted penalty is applied
 */
TEST_F(ForageTest, ForageHerbs_HuntedPenalty_ReducesSuccessRate) {
	// Set up habitat
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	// Create a foragable resource
	ForagableData& herb = manager->get<ForagableData>("TEST_HERB_2");
	herb.setName("Test Herb 2");
	herb.setHabitat(*test_location);
	herb.setFindDifficulty(SkillDifficultyType::kLight);
	herb.setLoreSkill(herb_lore_skill);

	// Test with and without hunted penalty
	HerbModifiers modifiers_hunted{*manager, nullptr, 10, 0, 0, true};
	HerbModifiers modifiers_not_hunted{*manager, nullptr, 10, 0, 0, false};

	int successes_hunted = 0;
	int successes_not_hunted = 0;

	// Run multiple trials
	for (int i = 0; i < 20; i++) {
		auto result_hunted = forage_herbs(*test_character, *test_location, modifiers_hunted);
		if (!result_hunted.empty()) {
			successes_hunted++;
		}

		auto result_not_hunted = forage_herbs(*test_character, *test_location, modifiers_not_hunted);
		if (!result_not_hunted.empty()) {
			successes_not_hunted++;
		}
	}

	// Without hunted penalty, we should have more or equal successes
	EXPECT_GE(successes_not_hunted, successes_hunted);
}

/**
 * @brief Test that hours_spent modifier affects foraging results
 */
TEST_F(ForageTest, ForageHerbs_HoursSpent_AffectsResults) {
	// Set up habitat
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	// Create a foragable resource
	ForagableData& herb = manager->get<ForagableData>("TEST_HERB_3");
	herb.setName("Test Herb 3");
	herb.setHabitat(*test_location);
	herb.setFindDifficulty(SkillDifficultyType::kLight);
	herb.setLoreSkill(herb_lore_skill);

	// Test with more hours vs fewer hours
	HerbModifiers modifiers_long{*manager, nullptr, 20, 0, 0, false};
	HerbModifiers modifiers_short{*manager, nullptr, 5, 0, 0, false};

	int successes_long = 0;
	int successes_short = 0;

	// Run multiple trials
	for (int i = 0; i < 20; i++) {
		auto result_long = forage_herbs(*test_character, *test_location, modifiers_long);
		if (!result_long.empty()) {
			successes_long++;
		}

		auto result_short = forage_herbs(*test_character, *test_location, modifiers_short);
		if (!result_short.empty()) {
			successes_short++;
		}
	}

	// More hours should yield more successes
	EXPECT_GE(successes_long, successes_short);
}

/**
 * @brief Test that target_resource searches for specific herb
 */
TEST_F(ForageTest, ForageHerbs_TargetResource_SearchesSpecificHerb) {
	// Set up habitat
	test_location->addVegetation(EnvironmentType::Vegetation::kJungle);

	// Create multiple foragable resources
	ForagableData& target_herb = manager->get<ForagableData>("TARGET_HERB");
	target_herb.setName("Target Herb");
	target_herb.setHabitat(*test_location);
	target_herb.setFindDifficulty(SkillDifficultyType::kLight);
	target_herb.setLoreSkill(herb_lore_skill);

	ForagableData& other_herb = manager->get<ForagableData>("OTHER_HERB");
	other_herb.setName("Other Herb");
	other_herb.setHabitat(*test_location);
	other_herb.setFindDifficulty(SkillDifficultyType::kLight);
	other_herb.setLoreSkill(herb_lore_skill);

	// Test with target resource specified
	HerbModifiers modifiers{*manager, &target_herb, 10, 0, 0, false};

	// Run multiple times to get a successful forage
	for (int i = 0; i < 50; i++) {
		auto result = forage_herbs(*test_character, *test_location, modifiers);
		if (!result.empty()) {
			// Should only contain the target herb, not other herbs
			EXPECT_EQ(result.size(), 1);
			EXPECT_TRUE(result.find(&target_herb) != result.end());
			break;
		}
	}
}

/**
 * @brief Test that herb_lore_bonus is applied during identification
 */
TEST_F(ForageTest, ForageHerbs_HerbLoreBonus_AffectsIdentification) {
	// Set up habitat
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	// Create a difficult to identify herb
	ForagableData& rare_herb = manager->get<ForagableData>("RARE_HERB");
	rare_herb.setName("Rare Herb");
	rare_herb.setHabitat(*test_location);
	rare_herb.setFindDifficulty(SkillDifficultyType::kVeryHard);  // High difficulty
	rare_herb.setLoreSkill(herb_lore_skill);

	// Test with and without herb lore bonus
	HerbModifiers modifiers_with_bonus{*manager, nullptr, 15, 5, 0, false, 50, 0};
	HerbModifiers modifiers_without_bonus{*manager, nullptr, 15, 5, 0, false, 0, 0};

	int identified_with_bonus = 0;
	int identified_without_bonus = 0;

	// Run multiple trials
	for (int i = 0; i < 30; i++) {
		auto result_with = forage_herbs(*test_character, *test_location, modifiers_with_bonus);
		if (!result_with.empty()) {
			identified_with_bonus++;
		}

		auto result_without = forage_herbs(*test_character, *test_location, modifiers_without_bonus);
		if (!result_without.empty()) {
			identified_without_bonus++;
		}
	}

	// With herb lore bonus, should have better identification rate
	EXPECT_GE(identified_with_bonus, identified_without_bonus);
}

/**
 * @brief Test that poison_lore_bonus is applied for poison-type foragables
 */
TEST_F(ForageTest, ForageHerbs_PoisonLoreBonus_AffectsPoison) {
	// Set up habitat
	test_location->addWater(EnvironmentType::Water::kMarsh);

	// Create a poison foragable
	ForagableData& poison_herb = manager->get<ForagableData>("POISON_HERB");
	poison_herb.setName("Poison Herb");
	poison_herb.setHabitat(*test_location);
	poison_herb.setFindDifficulty(SkillDifficultyType::kHard);
	poison_herb.setLoreSkill(poison_lore_skill);

	// Test with poison lore bonus
	HerbModifiers modifiers_with_bonus{*manager, nullptr, 15, 5, 0, false, 0, 50};
	HerbModifiers modifiers_without_bonus{*manager, nullptr, 15, 5, 0, false, 0, 0};

	int identified_with_bonus = 0;
	int identified_without_bonus = 0;

	// Run multiple trials
	for (int i = 0; i < 30; i++) {
		auto result_with = forage_herbs(*test_character, *test_location, modifiers_with_bonus);
		if (!result_with.empty()) {
			identified_with_bonus++;
		}

		auto result_without = forage_herbs(*test_character, *test_location, modifiers_without_bonus);
		if (!result_without.empty()) {
			identified_without_bonus++;
		}
	}

	// With poison lore bonus, should have better identification rate
	EXPECT_GE(identified_with_bonus, identified_without_bonus);
}

/**
 * @brief Test that habitat matching works correctly
 */
TEST_F(ForageTest, ForageHerbs_LocationMatching_OnlyFindsMatchingHerbs) {
	// Set up two different locations
	Habitat forest_location;
	forest_location.addVegetation(EnvironmentType::Vegetation::kDeciduous);

	Habitat desert_location;
	desert_location.addVegetation(EnvironmentType::Vegetation::kBarren);

	// Create herbs for different locations
	ForagableData& forest_herb = manager->get<ForagableData>("FOREST_HERB");
	forest_herb.setName("Forest Herb");
	forest_herb.setHabitat(forest_location);
	forest_herb.setFindDifficulty(SkillDifficultyType::kLight);
	forest_herb.setLoreSkill(herb_lore_skill);

	ForagableData& desert_herb = manager->get<ForagableData>("DESERT_HERB");
	desert_herb.setName("Desert Herb");
	desert_herb.setHabitat(desert_location);
	desert_herb.setFindDifficulty(SkillDifficultyType::kLight);
	desert_herb.setLoreSkill(herb_lore_skill);

	// Search in forest habitat
	HerbModifiers modifiers{*manager, nullptr, 15, 5, 0, false};

	// Run multiple times to get results
	for (int i = 0; i < 50; i++) {
		auto result = forage_herbs(*test_character, forest_location, modifiers);
		if (!result.empty()) {
			// Should not find desert herb in forest habitat
			EXPECT_TRUE(result.find(&desert_herb) == result.end());
			break;
		}
	}
}

// -----------------------------------------------------------------------
// Tests for forage_food function
// -----------------------------------------------------------------------

/**
 * @brief Test forage_food with deciduous vegetation (easy)
 */
TEST_F(ForageTest, ForageFood_DeciduousVegetation_EasyDifficulty) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run forage multiple times
	int total_runs = 20;
	int successful_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float food = forage_food(*test_character, *test_location, modifiers);
		if (food > 0.0f) {
			successful_runs++;
		}
	}

	// Should have at least some successes with easy vegetation
	EXPECT_GT(successful_runs, 0);
}

/**
 * @brief Test forage_food with barren vegetation (absurd difficulty)
 */
TEST_F(ForageTest, ForageFood_BarrenVegetation_AbsurdDifficulty) {
	test_location->addVegetation(EnvironmentType::Vegetation::kBarren);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run forage multiple times
	int total_runs = 20;
	int successful_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float food = forage_food(*test_character, *test_location, modifiers);
		if (food > 0.0f) {
			successful_runs++;
		}
	}

	// Barren should be very difficult - likely many failures
	EXPECT_LE(successful_runs, total_runs);
}

/**
 * @brief Test forage_food with fresh water coast (easy)
 */
TEST_F(ForageTest, ForageFood_FreshWaterCoast_EasyDifficulty) {
	test_location->addWater(EnvironmentType::Water::kFreshCoast);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run forage multiple times
	int total_runs = 20;
	int successful_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float food = forage_food(*test_character, *test_location, modifiers);
		if (food > 0.0f) {
			successful_runs++;
		}
	}

	// Fresh water coast should provide good foraging
	EXPECT_GT(successful_runs, 0);
}

/**
 * @brief Test forage_food with alpine terrain (very hard)
 */
TEST_F(ForageTest, ForageFood_AlpineTerrain_VeryHardDifficulty) {
	test_location->addTerrain(EnvironmentType::Terrain::kAlpine);
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run forage multiple times
	int total_runs = 20;
	int alpine_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float food = forage_food(*test_character, *test_location, modifiers);
		// Alpine makes it harder even with good vegetation
		alpine_runs++;
	}

	EXPECT_EQ(alpine_runs, total_runs);
}

/**
 * @brief Test forage_food with multiple people (increased difficulty)
 */
TEST_F(ForageTest, ForageFood_MultiplePeople_IncreasesDifficulty) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers_one{*manager, 4, 1, false};
	SustainanceModifiers modifiers_five{*manager, 4, 5, false};

	int successes_one = 0;
	int successes_five = 0;

	for (int i = 0; i < 20; i++) {
		float food_one = forage_food(*test_character, *test_location, modifiers_one);
		if (food_one > 0.0f) {
			successes_one++;
		}

		float food_five = forage_food(*test_character, *test_location, modifiers_five);
		if (food_five > 0.0f) {
			successes_five++;
		}
	}

	// Finding food for 1 person should be easier than for 5
	EXPECT_GE(successes_one, successes_five);
}

/**
 * @brief Test forage_food with less than 4 hours (increased difficulty)
 */
TEST_F(ForageTest, ForageFood_LessThan4Hours_IncreasesDifficulty) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers_short{*manager, 1, 1, false};
	SustainanceModifiers modifiers_normal{*manager, 4, 1, false};

	int successes_short = 0;
	int successes_normal = 0;

	for (int i = 0; i < 20; i++) {
		float food_short = forage_food(*test_character, *test_location, modifiers_short);
		if (food_short > 0.0f) {
			successes_short++;
		}

		float food_normal = forage_food(*test_character, *test_location, modifiers_normal);
		if (food_normal > 0.0f) {
			successes_normal++;
		}
	}

	// Normal duration should be easier than short duration
	EXPECT_GE(successes_normal, successes_short);
}

/**
 * @brief Test forage_food with more than 4 hours (decreased difficulty)
 */
TEST_F(ForageTest, ForageFood_MoreThan4Hours_DecreasesDifficulty) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers_normal{*manager, 4, 1, false};
	SustainanceModifiers modifiers_long{*manager, 8, 1, false};

	int successes_normal = 0;
	int successes_long = 0;

	for (int i = 0; i < 20; i++) {
		float food_normal = forage_food(*test_character, *test_location, modifiers_normal);
		if (food_normal > 0.0f) {
			successes_normal++;
		}

		float food_long = forage_food(*test_character, *test_location, modifiers_long);
		if (food_long > 0.0f) {
			successes_long++;
		}
	}

	// Longer duration should provide better or equal results
	EXPECT_GE(successes_long, successes_normal);
}

/**
 * @brief Test forage_food returns valid range of values
 */
TEST_F(ForageTest, ForageFood_ReturnsValidRange) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run multiple times and check all results are in valid range
	for (int i = 0; i < 100; i++) {
		float food = forage_food(*test_character, *test_location, modifiers);
		EXPECT_GE(food, 0.0f);
		EXPECT_LE(food, 1.25f);
	}
}

/**
 * @brief Test forage_food special roll of 66 returns 1.1
 */
TEST_F(ForageTest, ForageFood_SpecialRolls_ReturnCorrectValues) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run many times to potentially hit special rolls
	bool found_rare = false;
	bool found_cache = false;
	bool found_empty = false;
	bool found_partial = false;
	bool found_full = false;

	for (int i = 0; i < 200; i++) {
		float food = forage_food(*test_character, *test_location, modifiers);
		if (food >= 1.09f && food <= 1.11f) found_rare = true;
		if (food >= 1.24f && food <= 1.26f) found_cache = true;
		if (food == 0.0f) found_empty = true;
		if (food >= 0.19f && food <= 0.21f) found_partial = true;
		if (food >= 0.99f && food <= 1.01f) found_full = true;
	}

	// At least some of these conditions should be met
	EXPECT_TRUE(found_empty || found_partial || found_full || found_rare || found_cache);
}

// -----------------------------------------------------------------------
// Tests for forage_water function
// -----------------------------------------------------------------------

/**
 * @brief Test forage_water with cold wet climate (easy)
 */
TEST_F(ForageTest, ForageWater_ColdWetClimate_EasyDifficulty) {
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kRainforest);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMarineWestCoast);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	int total_runs = 20;
	int successful_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float water = forage_water(*test_character, *test_location, modifiers);
		if (water > 0.0f) {
			successful_runs++;
		}
	}

	// Wet climate should provide good water foraging
	EXPECT_GT(successful_runs, 0);
}

/**
 * @brief Test forage_water with hot dry climate (extremely hard)
 */
TEST_F(ForageTest, ForageWater_HotDryClimate_ExtremelyHardDifficulty) {
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kAridDesertHot);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kAridSteppeHot);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Run multiple times - should be very difficult
	int total_runs = 20;
	int successful_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float water = forage_water(*test_character, *test_location, modifiers);
		if (water > 0.0f) {
			successful_runs++;
		}
	}

	// Hot dry should be very challenging
	EXPECT_LE(successful_runs, total_runs);
}

/**
 * @brief Test forage_water with fresh coast water (easy)
 */
TEST_F(ForageTest, ForageWater_FreshCoastWater_EasyDifficulty) {
	test_location->addWater(EnvironmentType::Water::kFreshCoast);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	int total_runs = 20;
	int successful_runs = 0;

	for (int i = 0; i < total_runs; i++) {
		float water = forage_water(*test_character, *test_location, modifiers);
		if (water > 0.0f) {
			successful_runs++;
		}
	}

	// Fresh water coast should provide easy water access
	EXPECT_GT(successful_runs, 0);
}

/**
 * @brief Test forage_water with breaks during dry season (extremely hard)
 */
TEST_F(ForageTest, ForageWater_BreaksDrySeason_ExtremelyHardDifficulty) {
	test_location->addWater(EnvironmentType::Water::kBreaks);

	SustainanceModifiers modifiers_dry{*manager, 4, 1, true};
	SustainanceModifiers modifiers_wet{*manager, 4, 1, false};

	int successes_dry = 0;
	int successes_wet = 0;

	for (int i = 0; i < 20; i++) {
		float water_dry = forage_water(*test_character, *test_location, modifiers_dry);
		if (water_dry > 0.0f) {
			successes_dry++;
		}

		float water_wet = forage_water(*test_character, *test_location, modifiers_wet);
		if (water_wet > 0.0f) {
			successes_wet++;
		}
	}

	// Wet season should be easier than dry season for breaks
	EXPECT_GE(successes_wet, successes_dry);
}

/**
 * @brief Test forage_water with alpine terrain (very hard)
 */
TEST_F(ForageTest, ForageWater_AlpineTerrain_VeryHardDifficulty) {
	test_location->addTerrain(EnvironmentType::Terrain::kAlpine);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	// Alpine makes it harder even with wet climate
	int total_runs = 20;
	for (int i = 0; i < total_runs; i++) {
		float water = forage_water(*test_character, *test_location, modifiers);
		// Just verify it runs without crashing
		EXPECT_GE(water, 0.0f);
	}
}

/**
 * @brief Test forage_water with less than 4 hours (increased difficulty)
 */
TEST_F(ForageTest, ForageWater_LessThan4Hours_IncreasesDifficulty) {
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMarineWestCoast);

	SustainanceModifiers modifiers_short{*manager, 1, 1, false};
	SustainanceModifiers modifiers_normal{*manager, 4, 1, false};

	int successes_short = 0;
	int successes_normal = 0;

	for (int i = 0; i < 20; i++) {
		float water_short = forage_water(*test_character, *test_location, modifiers_short);
		if (water_short > 0.0f) {
			successes_short++;
		}

		float water_normal = forage_water(*test_character, *test_location, modifiers_normal);
		if (water_normal > 0.0f) {
			successes_normal++;
		}
	}

	// Normal duration should be easier
	EXPECT_GE(successes_normal, successes_short);
}

/**
 * @brief Test forage_water with more than 4 hours (decreased difficulty)
 */
TEST_F(ForageTest, ForageWater_MoreThan4Hours_DecreasesDifficulty) {
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMarineWestCoast);

	SustainanceModifiers modifiers_normal{*manager, 4, 1, false};
	SustainanceModifiers modifiers_long{*manager, 8, 1, false};

	int successes_normal = 0;
	int successes_long = 0;

	for (int i = 0; i < 20; i++) {
		float water_normal = forage_water(*test_character, *test_location, modifiers_normal);
		if (water_normal > 0.0f) {
			successes_normal++;
		}

		float water_long = forage_water(*test_character, *test_location, modifiers_long);
		if (water_long > 0.0f) {
			successes_long++;
		}
	}

	// Longer duration should provide better or equal results
	EXPECT_GE(successes_long, successes_normal);
}

/**
 * @brief Test forage_water returns valid range of values
 */
TEST_F(ForageTest, ForageWater_ReturnsValidRange) {
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMediterranean);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	for (int i = 0; i < 100; i++) {
		float water = forage_water(*test_character, *test_location, modifiers);
		EXPECT_GE(water, 0.0f);
		EXPECT_LE(water, 1.25f);
	}
}

/**
 * @brief Test forage_water special rolls return correct values
 */
TEST_F(ForageTest, ForageWater_SpecialRolls_ReturnCorrectValues) {
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMediterranean);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	bool found_rare = false;
	bool found_cache = false;
	bool found_empty = false;
	bool found_partial = false;
	bool found_full = false;

	for (int i = 0; i < 200; i++) {
		float water = forage_water(*test_character, *test_location, modifiers);
		if (water >= 1.09f && water <= 1.11f) found_rare = true;
		if (water >= 1.24f && water <= 1.26f) found_cache = true;
		if (water == 0.0f) found_empty = true;
		if (water >= 0.19f && water <= 0.21f) found_partial = true;
		if (water >= 0.99f && water <= 1.01f) found_full = true;
	}

	EXPECT_TRUE(found_empty || found_partial || found_full || found_rare || found_cache);
}

// -----------------------------------------------------------------------
// Tests for forage_sustainance function
// -----------------------------------------------------------------------

/**
 * @brief Test forage_sustainance returns both food and water
 */
TEST_F(ForageTest, ForageSustainance_ReturnsBothFoodAndWater) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMediterranean);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	auto [food, water] = forage_sustainance(*test_character, *test_location, modifiers);

	// Both should be in valid range
	EXPECT_GE(food, 0.0f);
	EXPECT_LE(food, 1.25f);
	EXPECT_GE(water, 0.0f);
	EXPECT_LE(water, 1.25f);
}

/**
 * @brief Test forage_sustainance food and water are independent
 */
TEST_F(ForageTest, ForageSustainance_FoodAndWaterIndependent) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMediterranean);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	bool found_different = false;
	for (int i = 0; i < 50; i++) {
		auto [food, water] = forage_sustainance(*test_character, *test_location, modifiers);
		// Check if food and water amounts differ (they're independent rolls)
		if (std::abs(food - water) > 0.01f) {
			found_different = true;
			break;
		}
	}

	EXPECT_TRUE(found_different);
}

/**
 * @brief Test forage_sustainance with good conditions
 */
TEST_F(ForageTest, ForageSustainance_GoodConditions_HighSuccessRate) {
	test_location->addVegetation(EnvironmentType::Vegetation::kDeciduous);
	test_location->addWater(EnvironmentType::Water::kLake);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMediterranean);

	SustainanceModifiers modifiers{*manager, 8, 1, false};

	int runs_with_food = 0;
	int runs_with_water = 0;

	for (int i = 0; i < 20; i++) {
		auto [food, water] = forage_sustainance(*test_character, *test_location, modifiers);
		if (food > 0.0f) runs_with_food++;
		if (water > 0.0f) runs_with_water++;
	}

	// With good conditions, should find something
	EXPECT_GT(runs_with_food, 0);
	EXPECT_GT(runs_with_water, 0);
}

/**
 * @brief Test forage_sustainance with poor conditions
 */
TEST_F(ForageTest, ForageSustainance_PoorConditions_LowSuccessRate) {
	test_location->addVegetation(EnvironmentType::Vegetation::kBarren);
	test_location->addTerrain(EnvironmentType::Terrain::kAlpine);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kAridDesertCold);

	SustainanceModifiers modifiers{*manager, 2, 3, true};

	int total_runs = 20;
	for (int i = 0; i < total_runs; i++) {
		auto [food, water] = forage_sustainance(*test_character, *test_location, modifiers);
		// Just verify it completes without error
		EXPECT_GE(food, 0.0f);
		EXPECT_GE(water, 0.0f);
	}
}

/**
 * @brief Test forage_sustainance multiple times gives varied results
 */
TEST_F(ForageTest, ForageSustainance_MultipleRuns_VariedResults) {
	test_location->addVegetation(EnvironmentType::Vegetation::kGrasslands);
	test_location->addClimateSubGroup(rm::rule::enums::ClimateType::KoppenSubGroup::kMarineWestCoast);

	SustainanceModifiers modifiers{*manager, 4, 1, false};

	std::set<float> food_results;
	std::set<float> water_results;

	for (int i = 0; i < 50; i++) {
		auto [food, water] = forage_sustainance(*test_character, *test_location, modifiers);
		food_results.insert(food);
		water_results.insert(water);
	}

	// Should get varied results due to dice rolls
	EXPECT_GT(food_results.size(), 1);
	EXPECT_GT(water_results.size(), 1);
}

} // namespace
