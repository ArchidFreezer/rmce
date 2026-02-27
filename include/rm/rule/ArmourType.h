#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm {

	/**
	 * @brief Contains the ArmourType::Type enum and associated functions
	 *
	 * The armour type work by a character has an impact on many actions such as attacks, some manoeuvres and also may impart
	 * a penalty to the characters quickness bonus.
	 */
	namespace ArmourType {

		/**
		* @brief Thrown to indicate that no ArmourType::Type enum was found
		*
		*/
		class ArmourTypeNotFoundException : public std::runtime_error {
		public:
			/**
			 * @brief Exception constructor
			 *
			 * std::runtime_error will handle the string
			 *
			 * @param error String to display for the error
			 */
			ArmourTypeNotFoundException(const std::string& error) : std::runtime_error{ error }	{}
		};

		/**
		 * @brief Enumeration class representing the size of creatures
		 */
		enum class Type {
			kAT1, /**< Skin: assumes normal clothing */
			kAT2, /**< Robes: full-length normally worn by spell users */
			kAT3, /**< Light Hide: natural hide of certain animals (e.g., Deer, Dog, Wolf) */
			kAT4, /**< Heavy Hide: natural hide of certain animals (e.g., Buffalo, Elephant, Bear) */
			kAT5, /**< Leather Jerkin: heavy leather coat without arms and reaching only to the waist or mid-thigh. */
			kAT6, /**< Leather Coat; heavy leather coat covering the arms and torso to mid-thigh */
			kAT7, /**< Reinforced Leather Coat: heavy leather coat to mid-thigh with rigid sections sewn in */
			kAT8, /**< Reinforced Full-Length Leather Coat: heavy leather coat covering lower legs with rigid sections sewn in */
			kAT9, /**< Leather Breastplate: rigid breastplate covering the torso to mid-thigh and part of the upper arms */
			kAT10, /**< Leather Breastplate & Greaves: ridig breastplate with greaves covering forearms and lower legs */
			kAT11, /**< Half-Hide Plate: natural animal hide containing some rigid plates (e.g., Rhinoceros, Alligator) */
			kAT12, /**< Full-Hide Plate: natural animal hide containing many or very rigid plates (e.g., Turtles, Dragons, Giant Crabs) */
			kAT13, /**< Chain Shirt: covers the torso to mid-thigh and half of the upper arms */
			kAT14, /**< Chain Shirt & Greaves: covers the torso plus greaves on forearms and lower legs */
			kAT15, /**< Full Chain: covering most of the body and legs (shirt and leggings) */
			kAT16, /**< Chain Hauberk: full-length, split from the waist in the front and back to facilitate movement */
			kAT17, /**< Metal Breastplate: covers the torso to mid-thigh and half of the upper arms */
			kAT18, /**< Metal Breastplate & Greaves: covers the torso plus greaves on forearms and lower legs */
			kAT19, /**< Half Plate: covering the body, with chain mail between plates, at joints, and covering legs */
			kAT20 /**< Full Plate: covering the body, with overlapping plates at joints, and plate on legs */
		};
		using enum Type;

		/**
		 * @brief Get the string representation of the given enum
		 * @param size The ArmourType::Type to get the string of
		 * @return Game display form of the enum value as a string
		 */
		const std::string toString(Type size);

		/**
		 * @brief Teach operator<< how to print a ArmourType::Type
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
		 * @return ArmourType::Type represented by the string
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
		 * @param type reference to set the ArmourType::Type represented by the string to
		 * @throws std::invalid_argument if the string does not represent a valid ArmourType::Type
		 *
		 * @see toString()
		 */
		void fromString(std::string_view sv, ArmourType::Type& type);

		/**
		 * @brief Get the string representation of the given enum in a game friendly form
		 * @param size The ArmourType::Type to get the string of
		 * @return Game display form of the enum value as a game friendlystring
		 */
		const std::string description(Type size);

		/**
		 * @brief Get the enumeration value based on a string
		 *
		 * Accepts values that are returned by description() in a case insensitive manner.
		 *
		 * @param sv string_view to get the enumeration for
		 * @return ArmourType::Type represented by the string
		 *
		 * @see description()
		 */
		const std::optional<Type> fromDescription(std::string_view sv);
	}

} // namespace rm