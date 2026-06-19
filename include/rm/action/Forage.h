#pragma once

#include <optional>
#include <functional> // required for std::reference_wrapper

#include <Character.h>
#include <Location.h>
#include <ForagableData.h>
#include <PersistentObjectManager.h>

namespace rm::action::forage {

struct Config {
	rm::PersistentObjectManager& object_factory;              /**< The persistent object manager */
	const rm::rule::ForagableData* target_resource = nullptr; /**< Optional target resource to forage for, if not provided, all resources will be considered */
	int hours_spent{10};                                      /**< Number of hours spent foraging, used to determine the skill check difficulty and the number of doses found */
	int additional_searchers{0};                              /**< Number of additional searchers */
	int failed_search_count{0};                               /**< Number of unsuccessful search attempts */

	int herb_lore_bonus{0};                                       /**< Generic herb lore skill bonus from items such as a book */
	int poison_lore_bonus{0};                                     /**< Generic poison lore skill bonus from items such as a book */
	std::map<const rm::rule::ForagableData*, int> lore_bonuses{}; /**< Map of foragable resources and any herb lore skill bonuses from items (book) or familiarity with specific plants */
};

std::map<const rm::rule::ForagableData*, int> forage_herbs(rm::game::character::Character& forager, rm::game::Location& search_location, Config& config);
} // namespace rm::action::forage
