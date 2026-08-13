#pragma once

#include <optional>
#include <functional> // required for std::reference_wrapper

#include <Character.h>
#include <Habitat.h>
#include <ForagableData.h>
#include <PersistentObjectManager.h>

namespace rm::action::forage {

/**
 * @brief Struct to hold modifiers for the herb foraging action.
 *
 * This struct contains various parameters that can affect the outcome of the herb foraging action, such as the target resource, hours spent foraging, number of additional searchers, and any skill bonuses.
 */
struct HerbModifiers {
	rm::PersistentObjectManager& object_factory;              /**< The persistent object manager */
	const rm::rule::ForagableData* target_resource = nullptr; /**< Optional target resource to forage for, if not provided, all resources will be considered */
	int hours_spent{10};                                      /**< Number of hours spent foraging, used to determine the skill check difficulty and the number of doses found */
	int additional_searchers{0};                              /**< Number of additional searchers */
	int failed_search_count{0};                               /**< Number of unsuccessful search attempts */
	bool hunted{false};                                       /**< Whether there is a hue and cry out on the forager */

	int herb_lore_bonus{0};                                       /**< Generic herb lore skill bonus from items such as a book */
	int poison_lore_bonus{0};                                     /**< Generic poison lore skill bonus from items such as a book */
	std::map<const rm::rule::ForagableData*, int> lore_bonuses{}; /**< Map of foragable resources and any herb lore skill bonuses from items (book) or familiarity with specific plants */
};

/**
 * @brief Forage for herbs in a given habitat using the provided character and modifiers.
 *
 * @param forager The character who is foraging.
 * @param search_habitat The habitat of the habitat where the foraging is taking place.
 * @param modifiers The modifiers that affect the foraging process.
 *
 * @return A map of found and identified foragable resources and the number of doses found for each resource.
 */
std::map<const rm::rule::ForagableData*, int> forage_herbs(rm::game::character::Character& forager, rm::game::Habitat& search_habitat, HerbModifiers& modifiers);

/**
 * @brief Struct to hold modifiers for the sustainance foraging action.
 *
 * This struct contains various parameters that can affect the outcome of the sustainance foraging action, such as the number of hours spent foraging, number of people sustainance is required for, and whether it is the dry season.
 */
struct SustainanceModifiers {
	rm::PersistentObjectManager& object_factory; /**< The persistent object manager */
	int hours_spent{4};                          /**< Number of hours spent foraging, used to determine the skill check difficulty */
	int num_people{1};                           /**< Number of people sustainance is required for */
	bool dry_season{false};                      /**< Whether it is the dry season, which makes foraging more difficult */
};

/**
 * @brief Forage for sustainance in a given habitat using the provided character and modifiers.
 *
 * Foraging for sustainance is for finding water and plants for survival, it includes fishing  and clamming, but does not include hunting for meat. The foraging process is affected by the number of hours spent foraging, the number of people
 * sustainance is required for, and whether it is the dry season.
 *
 * The foraging of food and water are treated as separate skill checks as the difficulty of finding food and water can be different. A single roll is used for both resources but the skill check difficulty is calculated separately for each
 * resource. The amount of food and water found is calculated based on the skill check roll and the number of hours spent foraging.
 *
 * The default duration of the action is one strategic round (4 hrs) and each additional 2 hours spent foraging decreasing the difficulty by 1. The base roll is to provide enough food and water for 1 individual, with each additional person
 * increasing the difficulty by 1. The dry season makes foraging for water more difficult in locations where there are monsoons/dry seasons.
 *
 * @param forager The character who is foraging.
 * @param search_habitat The habitat of the habitat where the foraging is taking place.
 * @param modifiers The modifiers that affect the foraging process.
 *
 * @return A pair of floats representing the amount of food and water found relative to the required amount.
 */
std::pair<float, float> forage_sustainance(rm::game::character::Character& forager, rm::game::Habitat& search_habitat, SustainanceModifiers& modifiers);

/**
 * @brief Forage for food in a given habitat using the provided character and modifiers.
 *
 * This function is a wrapper around forage_sustainance that only returns the amount of food found. It is provided for convenience when only the food foraging result is needed.
 *
 * @param forager The character who is foraging.
 * @param search_habitat The habitat of the habitat where the foraging is taking place.
 * @param modifiers The modifiers that affect the foraging process.
 *
 * @return A float representing the amount of food found relative to the required amount.
 */
float forage_food(rm::game::character::Character& forager, rm::game::Habitat& search_habitat, SustainanceModifiers& modifiers);

/**
 * @brief Forage for water in a given habitat using the provided character and modifiers.
 *
 * This function is a wrapper around forage_sustainance that only returns the amount of water found. It is provided for convenience when only the water foraging result is needed.
 *
 * @param forager The character who is foraging.
 * @param search_habitat The habitat of the habitat where the foraging is taking place.
 * @param modifiers The modifiers that affect the foraging process.
 *
 * @return A float representing the amount of water found relative to the required amount.
 */
float forage_water(rm::game::character::Character& forager, rm::game::Habitat& search_habitat, SustainanceModifiers& modifiers);

} // namespace rm::action::forage
