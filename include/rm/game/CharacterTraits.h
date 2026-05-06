#pragma once

namespace rm::game::character {

/**
 * Variables used by the character AI to assist in making choices during character creation. These are not intended to be used directly by the character but rather to provide a way for the character AI to make informed choices based on the
 * character's traits and preferences.
 */
struct CharacterTraits {
	int caster_{0};      /**< A [1-9] weighting for caster abilities. These include most magic skills and other abilities that are tied to the use of magic, such as familiars training */
	int combat_{0};      /**< A [1-9] weighting for combat oriented abilities, including both attack and CC abilities */
	int information_{0}; /**< A [1-9] weighting for information gathering abilities. These include most wilderness, lore and social skills that are used to gather information about the world and its inhabitants */
	int stealth_{0};     /**< A [1-9] weighting for stealth abilities. These include subterfuge, some urban and other skills that are used to avoid detection and manipulate the state of the world without direct confrontation */
	int support_{0};     /**< A [1-9] weighting for support abilities. These include healing and other abilities that whilst they do not directly have a negative impact on enemies assist the party in or out of combat */
	int utility_{0};     /**< A [1-9] weighting for utility abilities. These are typically active abilities including some urban and other skills that manipulate the state of the world */
};
} // namespace rm::game::character
