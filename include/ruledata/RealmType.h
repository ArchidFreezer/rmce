#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"
/**
 * @brief Contains the RealmType::Type enum and associated functions
 *
 * Realms of power define the primary area that skiulls or abilities gain their strength from. The magical realms have the
 * most in-game impact in some ways as each character must belong to one and only one of these, be it a primary or hybrid
 * real, and this limits their ability with skills tied to other realms. The non-magical realms are available equally to
 * all characters, with professions being limiting factor on how well the character can develop skills in that realm
 * rather that the character themselves.
 *
 * The majority of realms are realms of magic, with only two, Arms and Subterfuge, being non-magical
 */
namespace RealmType {
	/**
	 * @brief Enumeration class representing the realms in a campaign
	 */
	enum class Type {
		kArcane, /**< Arcane magic realm */
		kArms, /**< Non-magical realm of Arms */
		kChanneling, /**< Magical realm of Channeling */
		kChannelingEssence, /**< Magical hybrid realm of Channeling and Essence */
		kChannelingMentalism,  /**< Magical hybrid realm of Channeling and Mentalism */
		kEssence, /**< Magical realm of Essence */
		kEssenceMentalism,  /**< Magical hybrid realm of Essence and Mentalism */
		kMentalism, /**< Magical realm of Mentalism */
		kMundane, /**< Magical realm of Mundane - not suitable for all campaigns */
		kNeutral, /**< Magical realm of Neutral - not suitable for all campaigns */
		kSubterfuge  /**< Non-magical realm of Subterfuge  - not suitable for all campaigns */
	};
	using enum Type;

	/**
	 * @brief Get the string representation of the given enum
	 * @param realm The RealmType::Type to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Type realm);

	/**
	 * @brief Teach operator<< how to print a RealmType::Type
	 * @param out Output stream that the enum should be printed to
	 * @param realm Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Type realm) {
		return out << toString(realm);
	}

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * Any characters in \a sv that are not letters or numbers will be removed before comparison
	 *
	 * @param sv string_view to get the enumeration for
	 * @return RealmType::Type represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Type> fromString(std::string_view sv);

	/**
	 * @brief Checks if the realm is magical or not
	 * @param realm RealmType::Type to test
	 * @return `true` if the Type is magical
	 * @return `false` if the Type is not magical
	 */
	constexpr bool isMagical(Type realm);
}