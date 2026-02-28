#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

	/**
	 * @brief Contains the AnimalOutlookType::Type enum and associated functions
	 *
	 * These are meant to be a general measure of the creature's outlook, attitude, and/or reaction/behaviour pattern towards the player character.
	 */
	namespace AnimalOutlookType {

		/**
		 * @brief Enumeration class representing the size of creatures
		 */
		enum class Type {
			kAggressive, /**< Aggressive and will attack if provoked or hungry */
			kAloof, /**<  Ignores other creatures unless interfered with, or attacked */
			kAltruistic, /**< Has an unselfish regard for the interests of others, often to the extent of risking his own safety */
			kBelligerent, /**< Often attacks without provocation */
			kBerserk, /**< Attacks closest living creature until it is destroyed */
			kCarefree, /**<  Does not believe that danger or misfortune exists for it */
			kCruel, /**< Not only hostile, but delights in death, pain, and suffering */
			kDomineering, /**< Desires power, attempts to control or dominate other creatures */
			kGood, /**<  Opposed to “evil” (e.g., those who are cruel, hostile, belligerent, etc.); supportive of those who are also good */
			kGreedy, /**<  Will attack or attempt to steal from other creatures if the risk does not seem too high */
			kHostile, /**<  Normally attacks other creatures on sight */
			kHungry, /**<  If hungry, will attack anything edible; otherwise Normal */
			kInquisitive, /**<  Inquisitive and curious; will approach and examine unusual situations */
			kJumpy, /**<  Normally bolts at any sign of other creatures */
			kNormal, /**<  Watches and is wary of other creatures; will sometimes attack if hungry */
			kPassive, /**<  Ignores the presence of other creatures unless threatened */
			kPlayful, /**<  Mischievous, but playful; will attempt to play with or play pranks on other creatures */
			kProtective, /**< Protective of a thing, place, other creature, etc. */
			kTimid /**<  Skittish around other creatures, runs at the slightest hint of danger */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The AnimalOutlookType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a AnimalOutlookType::Type
		 * @param out Output stream that the enum should be printed to
		 * @param size Enum value to output
		 * @return Output stream reference containing the output enum value
		 */
		inline std::ostream& operator<<(std::ostream& out, Type size) {
			return out << toString(size);
		}

		/**
		 * @brief Get the enumeration value based on a string
		 *
		 * Accepts values that are returned by toString() in a case insensitive manner.
		 *
		 * @param sv string_view to get the enumeration for
		 * @return AnimalOutlookType::Type represented by the string
		 *
		 * @see toString()
		 */
		const std::optional<AnimalOutlookType::Type> fromString(std::string_view sv);

		/**
		 * @brief Get the enumeration value based on a string and set it to the provided reference
		 *
		 * Accepts values that are returned by toString() in a case insensitive manner.
		 *
		 * @param sv string_view to get the enumeration for
		 * @param type reference to set the AnimalOutlookType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid AnimalOutlookType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, AnimalOutlookType::Type& type);

		/**
		 * @brief Get the string representation of the given enum in a game friendly form
		 * @param size The AnimalOutlookType::Type to get the string of
		 * @return Game display form of the enum value as a game friendlystring
		 */
		const std::string description(Type size);

	}

} // namespace rm::rule::enums