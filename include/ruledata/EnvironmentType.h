#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <string_view>

#include "StringUtils.h"

/**
 * @brief Specific environmental area or features.
 * 
 * These are not necessarily tied to a specific habitat or climate though there will be some combinations that do not occur
 * naturally together such as marshes in an very arid region.
 * 
 * Many fauna and flora can only be found in specific environments or the lack of certain environmental features.
 */
namespace EnvironmentType {

	/**
	* @brief Thrown to indicate that no EnvironmentType enum was found
	*
	*/
	class EnvironmentTypeNotFoundException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		EnvironmentTypeNotFoundException(const std::string& error) : std::runtime_error{ error }	{}
	};

	/**
	 * @brief Special features that may be found in an environment.
	 * 
	 * An environment may contain multiple of these features. e.g. a battlefield in a rural cultivated area.
	 */
	enum class Feature {
		kBattlefield, /**< Battlefields and/or shipwrecks */
		kBurial, /**< Burial sites */
		kCave, /**< Cave entrances, overhangs and/or lairs */
		kCavern, /**< Cavern complexes */
		kDimention, /**< Cross-over points for dimentions */
		kEnchanted, /**< Enchanted or magical places */
		kHabitation, /**< Villages, towns, cities and/or castles */
		kRuins, /**< Riuns */
		kRural, /**< Rural inhabited and/or cultivated areas */
		kVolcanic /**< Volcanic areas */
	};

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return EnvironmentType::Feature represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Feature> feature(std::string_view sv);

	/**
	 * @brief Get the string representation of the given enum
	 * @param feature The EnvironmentType::Feature to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Feature feature);

	/**
	 * @brief Get the description of the given enum
	 * 
	 * This gives more detail on the enum value for use in-game
	 * 
	 * @param feature The EnvironmentType::Feature to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string description(Feature feature);

	/**
	 * @brief Teach operator<< how to print a EnvironmentType::Feature
	 * @param out Output stream that the enum should be printed to
	 * @param feature Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Feature feature) {
		return out << toString(feature);
	}

}