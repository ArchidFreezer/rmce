#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

/**
 * @brief The habitats that fauna and flora may be found in the world.
 * 
 * These will give an indication of the weather that may found in each habitat, though there will often be seasonal
 * variations.
 * 
 */
namespace HabitatType {

	/**
	* @brief Thrown to indicate that no HabitatType enum was found
	*
	*/
	class HabitatTypeNotFoundException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		HabitatTypeNotFoundException(const std::string& error) : std::runtime_error{ error }	{}
	};

	/**
	 * @brief Enumeration class representing the temperature of habitats
	 */
	enum class Temperature {
		kHot, /**< Hot habitats */
		kWarm, /**< Warm habitats */
		kTemperate, /**< Temperate habitats */
		kCool, /**< Cool habitats */
		kCold, /**< Cold habitats */
	};

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return HabitatType::Temperature represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Temperature> temperature(std::string_view sv);

	/**
	 * @brief Get the enumeration value based on a string and set it to the provided reference
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @param type reference to set the HabitatType::Temperature represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid HabitatType::Temperature
	 *
	 * @see toString()
	 */
	void fromString(std::string_view sv, HabitatType::Temperature& type);

	/**
	 * @brief Get the string representation of the given enum
	 * @param temperature The HabitatType::Temperature to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Temperature temperature);

	/**
	 * @brief Teach operator<< how to print a HabitatType::Temperature
	 * @param out Output stream that the enum should be printed to
	 * @param temperature Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Temperature temperature) {
		return out << toString(temperature);
	}

	/**
	 * @brief Enumeration class representing the precipitation of habitats
	 */
	enum class Precipitation {
		kRainy, /**< Significant precipitation over the course of a year */
		kHumid, /**< Days can expect to be humid */
		kTemperate, /**< Balanced moisture content in the air */
		kDry, /**< Low precipitation, though may have periods of intense rain and flooding */
		kArid, /**< Minimal precipitation */
	};

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return HabitatType::Precipitation represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Precipitation> precipitation(std::string_view sv);

	/**
	 * @brief Get the enumeration value based on a string and set it to the provided reference
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @param type reference to set the HabitatType::Precipitation represented by the string to
	 * @throws std::invalid_argument if the string does not represent a valid HabitatType::Precipitation
	 *
	 * @see toString()
	 */
	void fromString(std::string_view sv, HabitatType::Precipitation& type);

	/**
	 * @brief Get the string representation of the given enum
	 * @param precipitation The HabitatType::Precipitation to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Precipitation precipitation);

	/**
	 * @brief Teach operator<< how to print a HabitatType::Precipitation
	 * @param out Output stream that the enum should be printed to
	 * @param precipitation Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Precipitation precipitation) {
		return out << toString(precipitation);
	}


}