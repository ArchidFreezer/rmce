#include <Dice.h>
#include <Forage.h>
#include <Logger.h>
#include <Random.h>
#include <SubcategoriedSkillData.h>

namespace rm::action::forage {

std::map<const rm::rule::ForagableData*, int> forage_herbs(rm::game::character::Character& forager, rm::game::Location& search_location, Config& config) {
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
	const rm::rule::SubcategoriedSkillData& foraging_skill = config.object_factory.subcategoriedSkillData("SKILL_FORAGING");

	// Calculate the skill check roll plus additive modifiers for foraging
	int foraging_roll = d100.roll().result();
	foraging_roll += forager.skillBonus(foraging_skill);
	foraging_roll += config.additional_searchers * 2; // Each additional searcher adds a +2 bonus to the roll
	foraging_roll += config.failed_search_count * 5;  // Each previous failed search attempt adds a 5 bonus to the roll
	LOG_DEBUG("Foraging skill check roll: {} (base roll: {}, skill bonus: {}, additional searchers bonus: {}, failed search bonus: {})", foraging_roll, d100.result(), forager.skillBonus(foraging_skill), config.additional_searchers * 2,
	          config.failed_search_count * 5);

	// Calculate the multiplier based modifiers
	float hours_modifier = static_cast<float>(config.hours_spent) / 10.0f; // Assuming 10 hours is the standard foraging time

	// Get the final forage roll by applying the hours modifier to the skill check roll
	foraging_roll = static_cast<int>(foraging_roll * hours_modifier);

	if (foraging_roll < 100) {
		LOG_INFO("You failed to find any resources. Roll: {}", foraging_roll);
		config.failed_search_count++;
		return foraged_resources;
	}

	// Check if we are looking for a specific resource or a general forage
	if (config.target_resource) {
		const rm::rule::ForagableData& foragable = *config.target_resource;

		// Handle specific resource forage
		if (foragable.location().matches(search_location)) {
			int doses_roll = archid::Dice(20).roll().result();
			int num_doses = std::max(1, 1 + doses_roll + foragable.difficultyRating());
			foraged_resources[&foragable] = num_doses;
		}
	} else {
		// Handle general forage
		for (const auto& foragable_wrapper : config.object_factory.getAll<rm::rule::ForagableData>()) {
			const rm::rule::ForagableData& foragable = foragable_wrapper.get();
			if (foragable.location().matches(search_location)) {
				int doses_roll = d100.roll().result();
				int num_doses = std::max(0, (foraging_roll + doses_roll + foragable.difficultyModifier() - 150) / 10);
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
	const rm::rule::SubcategoriedSkillData& herb_lore_skill = config.object_factory.subcategoriedSkillData("SKILL_HERB_LORE");
	int herb_lore_skill_bonus = forager.skillBonus(herb_lore_skill);

	LOG_INFO("Foraged resources: Roll ({})", foraging_roll);
	LOG_INFO("{:-<{}}", "", 52);
	LOG_INFO("| {:<17} | {:^5} | {:^5} | {:^5} | {:^5} |", "Resource", "Diff", "Roll", "Doses", "Iden");
	LOG_INFO("{:-<{}}", "", 52);


	for (auto& [foragable, num_doses] : foraged_resources) {
		int herb_lore_roll = d100.roll().result();
		herb_lore_roll += herb_lore_skill_bonus;
		herb_lore_roll += config.herb_lore_bonus; // Add any generic herb lore skill bonus from items (book)
		herb_lore_roll += config.herb_lore_bonuses[foragable]; // Add any specific herb lore skill bonus from familiarity with specific plants
		if (herb_lore_roll < (foragable->difficultyRating() * 10)) {
			LOG_INFO("| {:<17} | {:^5} | {:^5} | {:^5} | {:^5} |", foragable->name(), foragable->difficultyModifier(), herb_lore_roll, num_doses, "No");
		} else {
			LOG_INFO("| {:<17} | {:^5} | {:^5} | {:^5} | {:^5} |", foragable->name(), foragable->difficultyModifier(), herb_lore_roll, num_doses, "Yes");
			identified_resources[foragable] = num_doses;
		}
	}

	LOG_INFO("{:-<{}}", "", 52);
	return identified_resources;
}

} // namespace rm::action::forage
