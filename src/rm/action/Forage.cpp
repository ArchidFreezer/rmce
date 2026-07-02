#include <Dice.h>
#include <Forage.h>
#include <Logger.h>
#include <Random.h>
#include <SubcategoriedSkillData.h>

namespace rm::action::forage {

std::map<const rm::rule::ForagableData*, int> forage_herbs(rm::game::character::Character& forager, rm::game::Location& search_location, HerbModifiers& modifiers) {
	archid::Dice d100(100, 5); // Create a d100 dice with open-ended rolls for the top and bottom 5 results

	std::map<const rm::rule::ForagableData*, int> foraged_resources;

	/*
	 * Foraging is split into two steps
	 * 1 - Finding one or more plants that are potentially medicinal/magical (Foraging skill check)
	 * 2 - Identifying the plants and determining if they are useful (Herb Lore skill check)
	 *
	 * Both steps need to be successful before anything useful is found
	 */

	/*
	 * Perform foraging skill check to see what resources are found in the search location. The skill check is modified by the number of hours spent foraging, the number of additional searchers, and the number of previous failed searches.
	 */
	// Get the characters foraging skill bonus
	const rm::rule::SubcategoriedSkillData& foraging_skill = modifiers.object_factory.subcategoriedSkillData("SKILL_FORAGING");

	// Calculate the skill check roll plus additive modifiers for foraging
	int foraging_roll = d100.roll().result();
	foraging_roll += forager.skillBonus(foraging_skill);
	foraging_roll += modifiers.additional_searchers * 2; // Each additional searcher adds a +2 bonus to the roll
	foraging_roll += modifiers.failed_search_count * 5;  // Each previous failed search attempt adds a 5 bonus to the roll
	foraging_roll += modifiers.hunted ? -20 : 0;         // If the forager is being hunted, subtract 20 from the roll
	LOG_DEBUG("Foraging skill check roll: {} (base roll: {}, skill bonus: {}, additional searchers bonus: {}, failed search bonus: {}, hunted penalty: {})", foraging_roll, d100.result(), forager.skillBonus(foraging_skill),
	          modifiers.additional_searchers * 2, modifiers.failed_search_count * 5, modifiers.hunted ? -20 : 0);

	// Calculate the multiplier based modifiers
	float hours_modifier = static_cast<float>(modifiers.hours_spent) / 10.0f; // Assuming 10 hours is the standard foraging time

	// Get the final forage roll by applying the hours modifier to the skill check roll
	foraging_roll = static_cast<int>(foraging_roll * hours_modifier);

	if (foraging_roll < 100) {
		LOG_INFO("You failed to find any resources. Roll: {}", foraging_roll);
		modifiers.failed_search_count++;
		return foraged_resources;
	}

	// Check if we are looking for a specific resource or a general forage
	if (modifiers.target_resource) {
		const rm::rule::ForagableData& foragable = *modifiers.target_resource;

		// Handle specific resource forage
		if (foragable.location().matches(search_location)) {
			int doses_roll = archid::Dice(20).roll().result();
			int num_doses = std::max(1, 1 + doses_roll + foragable.findDifficultyRating());
			foraged_resources[&foragable] = num_doses;
		}
	} else {
		// Handle general forage
		for (const auto& foragable_wrapper : modifiers.object_factory.getAll<rm::rule::ForagableData>()) {
			const rm::rule::ForagableData& foragable = foragable_wrapper.get();
			if (foragable.location().matches(search_location)) {
				int doses_roll = d100.roll().result();
				int num_doses = std::max(0, (foraging_roll + doses_roll + foragable.findDifficultyModifier() - 150) / 10);
				if (num_doses > 0) {
					foraged_resources[&foragable] = num_doses;
				}
			}
		}
	}

	/*
	 * We now have the resources found, but we need to perform an identification skill check to see if the forager can identify the resources found. This is done by rolling a d100 and adding the forager's herb lore skill bonus. If the roll
	 * is greater than or equal to the difficulty rating of the resource, then the resource is identified and can be used. If not, then the resource is not identified and cannot be used.
	 */

	std::map<const rm::rule::ForagableData*, int> identified_resources;

	// Get the characters herb lore skill bonus
	const rm::rule::SubcategoriedSkillData& herb_lore_skill = modifiers.object_factory.subcategoriedSkillData("SKILL_HERB_LORE");
	int herb_lore_skill_bonus = forager.skillBonus(herb_lore_skill);
	const rm::rule::SubcategoriedSkillData& poison_lore_skill = modifiers.object_factory.subcategoriedSkillData("SKILL_POISON_LORE");
	int poison_lore_skill_bonus = forager.skillBonus(poison_lore_skill);

	LOG_INFO("Foraged resources: Roll ({})", foraging_roll);
	LOG_INFO("{:-<{}}", "", 52);
	LOG_INFO("| {:<17} | {:^5} | {:^5} | {:^5} | {:^5} |", "Resource", "Diff", "Roll", "Doses", "Iden");
	LOG_INFO("{:-<{}}", "", 52);

	for (auto& [foragable, num_doses] : foraged_resources) {
		int lore_roll = d100.roll().result();
		if (foragable->loreSkill() == &herb_lore_skill) {
			lore_roll += herb_lore_skill_bonus;
			lore_roll += modifiers.herb_lore_bonus; // Add any generic herb lore skill bonus from items (book)
		} else if (foragable->loreSkill() == &poison_lore_skill) {
			lore_roll += poison_lore_skill_bonus;
			lore_roll += modifiers.poison_lore_bonus; // Add any generic poison lore skill bonus from items (book)
		}
		lore_roll += modifiers.lore_bonuses[foragable]; // Add any specific herb lore skill bonus from familiarity with specific plants
		if (lore_roll < (foragable->findDifficultyRating() * 10)) {
			LOG_INFO("| {:<17} | {:^5} | {:^5} | {:^5} | {:^5} |", foragable->name(), foragable->findDifficultyModifier(), lore_roll, num_doses, "No");
		} else {
			LOG_INFO("| {:<17} | {:^5} | {:^5} | {:^5} | {:^5} |", foragable->name(), foragable->findDifficultyModifier(), lore_roll, num_doses, "Yes");
			identified_resources[foragable] = num_doses;
		}
	}

	LOG_INFO("{:-<{}}", "", 52);
	return identified_resources;
}

float forage_food(rm::game::character::Character& forager, rm::game::Location& search_location, SustainanceModifiers& modifiers) {
	// Get the characters foraging skill bonus
	const rm::rule::SubcategoriedSkillData& foraging_skill = modifiers.object_factory.subcategoriedSkillData("SKILL_FORAGING");
	float found_food = 0.0f;

	using namespace rm::rule::enums::EnvironmentType;
	using namespace rm::rule::enums::SkillDifficultyType;

	// First we get the food difficulty
	SkillDifficultyType::Type difficulty = kAbsurd;

	// Deal with vegetation first
	if (search_location.hasVegetation(Vegetation::kDeciduous) || search_location.hasVegetation(Vegetation::kConiferous) || search_location.hasVegetation(Vegetation::kJungle)) {
		difficulty = std::min(difficulty, kEasy);
	} else if (search_location.hasVegetation(Vegetation::kGrasslands) || search_location.hasVegetation(Vegetation::kHeath)) {
		difficulty = std::min(difficulty, kMedium);
	} else if (search_location.hasVegetation(Vegetation::kPlains)) {
		difficulty = std::min(difficulty, kHard);
	} else if (search_location.hasVegetation(Vegetation::kTundra)) {
		difficulty = std::min(difficulty, kSheerFolly);
	} else if (search_location.hasVegetation(Vegetation::kBarren)) {
		difficulty = std::min(difficulty, kAbsurd);
	}

	// Now deal with water sources
	if (search_location.hasWater(Water::kFreshCoast) || search_location.hasWater(Water::kLake) || search_location.hasWater(Water::kOasis)) {
		difficulty = std::min(difficulty, kEasy);
	} else if (search_location.hasWater(Water::kMarsh)) {
		difficulty = std::min(difficulty, kMedium);
	} else if (search_location.hasWater(Water::kSaltCoast)) {
		difficulty = std::min(difficulty, kHard);
	} else if (search_location.hasWater(Water::kDesert)) {
		difficulty = std::min(difficulty, kSheerFolly);
	} else if (search_location.hasWater(Water::kGlacier)) {
		difficulty = std::min(difficulty, kAbsurd);
	}

	// If it is the dry season, and the water type is breaks or wadis, which are seasonal we set the minimum difficluty to kExtremlyHard
	if (modifiers.dry_season && search_location.hasWater(Water::kBreaks)) {
		difficulty = std::max(difficulty, kExtremelyHard); // Note this is a max because the dry season makes it very difficult to find water in breaks or wadis, even if there is vegetation present
	}

	// Alpine terrain is always diffuclt to forage in, so if it is present, we set the difficulty to very hard regardless of vegetation or water sources
	if (search_location.hasTerrain(Terrain::kAlpine)) {
		difficulty = std::max(difficulty, kVeryHard); // Note this is a max because alpine terrain is very difficult to forage in, even if there is vegetation or water present
	}

	// Apply the modifiers for the number of people searching, each additional person increases the difficulty by 1, capped at kAbsurd
	difficulty = SkillDifficultyType::incrementDifficulty(difficulty, modifiers.num_people - 1); // Each additional person increases the difficulty by 1, capped at kAbsurd

	// Apply the action duration modifiers
	// If the duration is lower than 4 hours, the difficulty increases by 1 for each hour less than 4.
	// If the duration is greater than 4 hours, the difficulty decreases by 1 for each 2 hours greater than 4, up to a maximum of 2 degrees of difficulty.
	if (modifiers.hours_spent < 4) {
		difficulty = SkillDifficultyType::incrementDifficulty(difficulty, 4 - modifiers.hours_spent); // Each hour less than 4 increases the difficulty by 1
	} else if (modifiers.hours_spent > 4) {
		int difficulty_reduction = (std::min(8, modifiers.hours_spent) - 4) / 2; // Each additional 2 hours spent foraging decreases the difficulty by 1 up to 8 hours reducing the dificulty by 2
		difficulty = SkillDifficultyType::incrementDifficulty(difficulty, -difficulty_reduction);
	}

	// Make the skill check roll for foraging food
	archid::Dice d100(100, 5); // Create a d100 dice
	int foraging_roll = d100.roll().result();
	foraging_roll += forager.skillBonus(foraging_skill);

	if (foraging_roll == 66) {
		LOG_INFO("You found a rare food source! Roll: {}", foraging_roll);
		found_food = 1.1f; // Found a rare food source, which provides 10% more food than normal
	} else if (foraging_roll == 100) {
		LOG_INFO("You found a hidden food cache! Roll: {}", foraging_roll);
		found_food = 1.25f; // Found a hidden food cache, which provides 25% more food than normal
	} else if (foraging_roll <= 76) {
		LOG_INFO("You return empty handed so no food today! Roll: {}", foraging_roll);
		found_food = 0.0f; // Found no food
	} else if (foraging_roll <= 90) {
		LOG_INFO("You will be hungry today! Roll: {}", foraging_roll);
		found_food = 0.2f; // Found 20% of the food needed for the day
	} else if (foraging_roll <= 110) {
		LOG_INFO("You get most of what you need! Roll: {}", foraging_roll);
		found_food = 0.8f; // Found 80% of the food needed for the day
	} else if (foraging_roll <= 175) {
		LOG_INFO("Successful foraging food, you find what you need for the day! Roll: {}", foraging_roll);
		found_food = 1.0f; // Found 100% of the food needed for the day
	} else {
		LOG_INFO("Outstanding success foraging food, you find more than you need for the day! Roll: {}", foraging_roll);
		found_food = 1.2f; // Found 120% of the food needed for the day
	}

	return found_food;
}

float forage_water(rm::game::character::Character& forager, rm::game::Location& search_location, SustainanceModifiers& modifiers) {
	// Get the characters foraging skill bonus
	const rm::rule::SubcategoriedSkillData& foraging_skill = modifiers.object_factory.subcategoriedSkillData("SKILL_FORAGING");
	float found_water = 0.0f;

	using namespace rm::rule::enums::ClimateType;
	using namespace rm::rule::enums::EnvironmentType;
	using namespace rm::rule::enums::SkillDifficultyType;

	// First we get the water difficulty
	SkillDifficultyType::Type difficulty = kAbsurd;

	// Deal with climate first
	std::set<KoppenSubGroup> wet_climates{};
	wet_climates.insert(KoppenSubGroup::kRainforest);
	wet_climates.insert(KoppenSubGroup::kMediterranean);
	wet_climates.insert(KoppenSubGroup::kHumidSubtropical);
	wet_climates.insert(KoppenSubGroup::kMarineWestCoast);
	wet_climates.insert(KoppenSubGroup::kHumidContinentalHotSummer);
	wet_climates.insert(KoppenSubGroup::kHumidContinentalWarmSummer);
	if (!modifiers.dry_season) {
		wet_climates.insert(KoppenSubGroup::kMonsoon);
		wet_climates.insert(KoppenSubGroup::kSavanna);
	}

	for (const auto climate : wet_climates) {
		if (search_location.hasClimateSubGroup(climate)) {
			difficulty = std::min(difficulty, kEasy);
			break;
		}
	}

	std::set<KoppenSubGroup> dry_climates{};
	if (modifiers.dry_season) {
		dry_climates.insert(KoppenSubGroup::kMonsoon);
		dry_climates.insert(KoppenSubGroup::kSavanna);
	}

	for (const auto climate : dry_climates) {
		if (search_location.hasClimateSubGroup(climate)) {
			difficulty = std::min(difficulty, kHard);
			break;
		}
	}

	std::set<KoppenSubGroup> arid_climates{};
	arid_climates.insert(KoppenSubGroup::kAridDesertCold);
	arid_climates.insert(KoppenSubGroup::kAridDesertHot);
	arid_climates.insert(KoppenSubGroup::kAridSteppeCold);
	arid_climates.insert(KoppenSubGroup::kAridSteppeHot);

	for (const auto climate : arid_climates) {
		if (search_location.hasClimateSubGroup(climate)) {
			difficulty = std::min(difficulty, kExtremelyHard);
			break;
		}
	}

	// Now deal with water sources
	if (search_location.hasWater(Water::kFreshCoast) || search_location.hasWater(Water::kLake) || search_location.hasWater(Water::kOasis)) {
		difficulty = std::min(difficulty, kEasy);
	} else if (search_location.hasWater(Water::kMarsh)) {
		difficulty = std::min(difficulty, kMedium);
	} else if (search_location.hasWater(Water::kDesert)) {
		difficulty = std::min(difficulty, kSheerFolly);
	}

	// If it is the dry season, and the water type is breaks or wadis, which are seasonal we set the minimum difficluty to kExtremlyHard
	if (modifiers.dry_season && search_location.hasWater(Water::kBreaks)) {
		difficulty = std::max(difficulty, kExtremelyHard); // Note this is a max because the dry season makes it very difficult to find water in breaks or wadis, even if there is vegetation present
	}

	// Alpine terrain is always diffuclt to forage in, so if it is present, we set the difficulty to very hard regardless of vegetation or water sources
	if (search_location.hasTerrain(Terrain::kAlpine)) {
		difficulty = std::max(difficulty, kVeryHard); // Note this is a max because alpine terrain is very difficult to forage in, even if there is vegetation or water present
	}

	// Apply the action duration modifiers
	// If the duration is lower than 4 hours, the difficulty increases by 1 for each hour less than 4.
	// If the duration is greater than 4 hours, the difficulty decreases by 1 for each 2 hours greater than 4, up to a maximum of 2 degrees of difficulty.
	if (modifiers.hours_spent < 4) {
		difficulty = SkillDifficultyType::incrementDifficulty(difficulty, 4 - modifiers.hours_spent); // Each hour less than 4 increases the difficulty by 1
	} else if (modifiers.hours_spent > 4) {
		int difficulty_reduction = (std::min(8, modifiers.hours_spent) - 4) / 2; // Each additional 2 hours spent foraging decreases the difficulty by 1 up to 8 hours reducing the dificulty by 2
		difficulty = SkillDifficultyType::incrementDifficulty(difficulty, -difficulty_reduction);
	}

	// Make the skill check roll for foraging water
	archid::Dice d100(100, 5); // Create a d100 dice
	int foraging_roll = d100.roll().result();
	foraging_roll += forager.skillBonus(foraging_skill);

	if (foraging_roll == 66) {
		LOG_INFO("You found a rare water source! Roll: {}", foraging_roll);
		found_water = 1.1f; // Found a rare water source, which provides 10% more water than normal
	} else if (foraging_roll == 100) {
		LOG_INFO("You found a hidden water cache! Roll: {}", foraging_roll);
		found_water = 1.25f; // Found a hidden water cache, which provides 25% more water than normal
	} else if (foraging_roll <= 76) {
		LOG_INFO("You return empty handed searching for water! Roll: {}", foraging_roll);
		found_water = 0.0f; // Found no water
	} else if (foraging_roll <= 90) {
		LOG_INFO("You will be thirsty today! Roll: {}", foraging_roll);
		found_water = 0.2f; // Found 20% of the water needed for the day
	} else if (foraging_roll <= 110) {
		LOG_INFO("You get most of the water you need! Roll: {}", foraging_roll);
		found_water = 0.8f; // Found 80% of the water needed for the day
	} else if (foraging_roll <= 175) {
		LOG_INFO("Successful foraging water, you find what you need for the day! Roll: {}", foraging_roll);
		found_water = 1.0f; // Found 100% of the water needed for the day
	} else {
		LOG_INFO("Outstanding success foraging water, you find more than you need for the day! Roll: {}", foraging_roll);
		found_water = 1.2f; // Found 120% of the water needed for the day
	}

	return found_water;
}

std::pair<float, float> forage_sustainance(rm::game::character::Character& forager, rm::game::Location& search_location, SustainanceModifiers& modifiers) {
	// Get the characters foraging skill bonus
	const rm::rule::SubcategoriedSkillData& foraging_skill = modifiers.object_factory.subcategoriedSkillData("SKILL_FORAGING");

	// Make the food roll
	float found_food = forage_food(forager, search_location, modifiers);
	float found_water = forage_water(forager, search_location, modifiers);

	return {found_food, found_water};
}

} // namespace rm::action::forage
