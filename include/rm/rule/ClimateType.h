#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include <StringUtils.h>

namespace rm::rule::enums::ClimateType {

/**
 * @brief The climates that fauna and flora may be found in the world using the Koppen climate classification system.
 *
 * These will give an indication of the weather that may found in each climate, though there will often be seasonal
 * variations.
 *
 */
enum class KoppenGroup {
	/** Tropical climates are hot and humid, with significant rainfall throughout the year */
	kTropical,
	/** Dry climates are characterized by low precipitation and high evaporation rates */
	kDry,
	/** Temperate climates have moderate temperatures and precipitation */
	kTemperate,
	/** Continental climates have large temperature variations between summer and winter */
	kContinental,
	/** Polar climates are cold year-round with limited precipitation */
	kPolar
};

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @return KoppenGroup represented by the string
 *
 * @see toString()
 */
const std::optional<KoppenGroup> koppenGroup(std::string_view sv);

/**
 * @brief Get the enumeration value based on a string and set it to the provided reference
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @param type reference to set the KoppenGroup represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid KoppenGroup
 *
 * @see toString()
 */
void fromString(std::string_view sv, KoppenGroup& type);

/**
 * @brief Get the string representation of the given enum
 * @param type The KoppenGroup to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(KoppenGroup type);

/**
 * @brief Teach operator<< how to print a KoppenGroup
 * @param out Output stream that the enum should be printed to
 * @param type Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, KoppenGroup type) {
	return out << toString(type);
}

/**
 * @brief The subgroups of the Koppen climate classification system.
 *
 * These will give an indication of the weather that may found in each climate, though there will often be seasonal
 * variations.
 *
 */
enum class KoppenSubGroup {
	/** Tropical rainforest climates are hot and humid, with significant rainfall throughout the year */
	kRainforest,
	/** Tropical monsoon climates are characterized by a short dry season and heavy rainfall during the wet season */
	kMonsoon,
	/** Tropical savanna climates have a distinct wet and dry season with moderate rainfall */
	kSavanna,
	/** Arid desert climates are extremely dry with very little precipitation */
	kAridDesert,
	/** Arid steppe climates are semi-arid with slightly more precipitation than deserts */
	kAridSteppe,
	/** Mediterranean climates have hot, dry summers and mild, wet winters */
	kMediterranean,
	/** Humid subtropical climates have hot, humid summers and mild winters with moderate precipitation */
	kHumidSubtropical,
	/** Marine west coast climates have mild temperatures and high precipitation throughout the year */
	kMarineWestCoast,
	/** Humid continental climates with hot summers and cold winters */
	kHumidContinentalHotSummer,
	/** Humid continental climates with warm summers and cold winters */
	kHumidContinentalWarmSummer,
	/** Subarctic climates have long, cold winters and short, mild summers */
	kSubarctic,
	/** Tundra climates are cold and dry, with short, cool summers and long, harsh winters */
	kTundra,
	/** Ice cap climates are extremely cold and dry, with permanent ice and snow cover */
	kIceCap
};

/**
 * @brief Get the enumeration value based on a string
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @return KoppenSubGroup represented by the string
 *
 * @see toString()
 */
const std::optional<KoppenSubGroup> koppenSubGroup(std::string_view sv);

/**
 * @brief Get the enumeration value based on a string and set it to the provided reference
 *
 * Accepts values that are returned by toString() in a case insensitive manner.
 *
 * @param sv string_view to get the enumeration for
 * @param type reference to set the KoppenSubGroup represented by the string to
 * @throws std::invalid_argument if the string does not represent a valid KoppenSubGroup
 *
 * @see toString()
 */
void fromString(std::string_view sv, KoppenSubGroup& type);

/**
 * @brief Get the string representation of the given enum
 * @param type The KoppenSubGroup to get the string of
 * @return Game display form of the enum value as a string
 */
const std::string toString(KoppenSubGroup type);

/**
 * @brief Teach operator<< how to print a KoppenSubGroup
 * @param out Output stream that the enum should be printed to
 * @param type Enum value to output
 * @return Output stream reference containing the output enum value
 */
inline std::ostream& operator<<(std::ostream& out, KoppenSubGroup type) {
	return out << toString(type);
}

/**
 * @brief Get the KoppenGroup that the given KoppenSubGroup belongs to
 * @param type The KoppenSubGroup to get the KoppenGroup for
 * @return The KoppenGroup that the given KoppenSubGroup belongs to, or std::nullopt if the KoppenSubGroup is invalid
 */
const std::optional<KoppenGroup> koppenGroup(KoppenSubGroup& type);

/**
 * @brief Compare a KoppenGroup and a KoppenSubGroup for equality
 * @param lhs The KoppenGroup to compare
 * @param rhs The KoppenSubGroup to compare
 * @return true if the KoppenGroup and KoppenSubGroup are equal, false otherwise
 */
bool operator==(KoppenGroup lhs, KoppenSubGroup rhs);

} // namespace rm::rule::enums::ClimateType
