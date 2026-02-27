#pragma once

#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums {

	/**
	 * @brief Contains the CriticalTableType::Type enum and associated functions
	 *
	 * Normally when resolving a critical strike the type of critical determines the lookup table used, but for large
	 * creatures it is the size of the creature and the nature of the damage that determines this, with Physical and Spell
	 * damage being the only factors. This represents the valid types of critical table to use.
	 */
	namespace CriticalTableType {

		/**
		* @brief Thrown to indicate that no CriticalTableype::Type enum was found
		*
		*/
		class CriticalTableTypeNotFoundException : public std::runtime_error {
		public:
			/**
			 * @brief Exception constructor
			 *
			 * std::runtime_error will handle the string
			 *
			 * @param error String to display for the error
			 */
			CriticalTableTypeNotFoundException(const std::string& error) : std::runtime_error{ error }	{}
		};

		/**
		 * @brief Enumeration class representing the type of critical tables
		 */
		enum class Type {
			kNormal, /**< Normal criticals are those that do not fit in the other categories */
			kLargeCreaturePhysical, /**< These are non-magical attacks against creatures with a CreatureSizeType::kLarge */
			kHugeCreaturePhysical, /**< These are non-magical attacks against creatures with a CreatureSizeType::kHuge */
			kLargeCreatureSpell, /**< These are magical attacks against creatures with CreatureSizeType::kLarge */
			kHugeCreatureSpell /**< These are magical attacks against creatures with CreatureSizeType::kHuge */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param type The CriticalTableType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type type);

		/**
		 * @brief Teach operator<< how to print a CriticalTableType::Type
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
		 * @param sv String to get the enumeration for
		 * @return CriticalTableType::Type represented by the string
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
		 * @param type reference to set the CriticalTableType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid CriticalTableType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, CriticalTableType::Type& type);

	}

} // namespace rm::rule::enums