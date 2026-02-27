#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm {

	/**
	 * @brief Contains the CriticalType::Type enum and associated functions
	 *
	 * During combat and in other situations a characte may receive critical damage with the actual damage taken determined
	 * by a lookup table. There are different types of critical based on the source of the damage and this enum lists all
	 * the possible types.
	 */
	namespace CriticalType {

		/**
		 * @brief Enumeration class representing types of critical damage
		 */
		enum class Type {
			kBrawling, /**< Combat without weapons */
			kGrapple, /**< Combat where the attacker is trying to grab the target */
			kImpact, /**< Certain spell attacks such a bolts, or the use of pure force */
			kKrush, /**< Weapons that use blunt force */
			kMartialArtsStrikes, /**< Soft martial arts such as Judo or Wrestling */
			kMartialArtsSweeps, /**< Hard martial atrs such as Karate, Boxing or Tackling */
			kPuncture, /**< Weapons that use a point to pierce the target such as arrows or animal teeth/horns */
			kSlash, /**< Weapons that use an edge to attack */
			kSubdual, /**< The point is immobilize or render a target unconscious */
			kTiny, /**< Creatures of #rm::CreatureSizeType::kTiny or other irritants */
			kUnbalance, /**< Attempt to knock the target over or off-balance them */
			kAether, /**< Elemental force of creation */
			kCold, /**< Elemental cold damage */
			kElectrical,  /**< Elemental electrical damage */
			kHeat,  /**< Elemental heat damage */
			kNether /**< Elemental force of destruction */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param type The CriticalType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type type);

		/**
		 * @brief Teach operator<< how to print a CriticalType::Type
		 * @param out Output stream that the enum should be printed to
		 * @param type Enum value to output
		 * @return Output stream reference containing the output enum value
		 */
		inline std::ostream& operator<<(std::ostream& out, Type type) {
			return out << toString(type);
		}

		/**
		 * @brief Get the enumeration value based on a string
		 *
		 * Accepts values that are returned by toString() in a case insensitive manner.
		 *
		 * Any characters in \a sv that are not letters or numbers will be removed before comparison
		 *
		 * @param sv string_view to get the enumeration for
		 * @return CriticalType::Type represented by the string
		 *
		 * @see toString()
		 */
		const std::optional<Type> fromString(std::string_view sv);

		/**
	 * @brief Get the enumeration value based on a string and set it to the provided reference
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @param type reference to set the CriticalType::Type represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid CriticalType::Type
	 *
	 * @see toString()
	 */
		void fromString(std::string_view sv, CriticalType::Type& type);

	}

} // namespace rm