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

	/**
	 * @brief The general terrain of an environment, if unusual.
	 *
	 * These are typically mutually exclusive, though there can be small areas within a region that may contain more than one
	 * such as pockets of waste land in a rough region.
	 */
	enum class Terrain {
		kAlpine, /**< Alpine, high altitude or mountainous */
		kRough, /**< Rough, rugged or rocky hills */
		kUnderground, /**< Underground */
		kWaste, /**< Waste, barren */
	};

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return EnvironmentType::Terrain represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Terrain> terrain(std::string_view sv);

	/**
	 * @brief Get the string representation of the given enum
	 * @param terrain The EnvironmentType::Terrain to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Terrain terrain);

	/**
	 * @brief Get the description of the given enum
	 *
	 * This gives more detail on the enum value for use in-game
	 *
	 * @param terrain The EnvironmentType::Terrain to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string description(Terrain terrain);

	/**
	 * @brief Teach operator<< how to print a EnvironmentType::Terrain
	 * @param out Output stream that the enum should be printed to
	 * @param terrain Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Terrain terrain) {
		return out << toString(terrain);
	}

	/**
	 * @brief The typical vegetation within a region
	 *
	 * These represent the general vegetation within a region and are can be used to identify the fauna and flora that may be
	 * found there.
	 */
	enum class Vegetation {
		kBarren, /**< Barren, without vegetation */
		kConiferous, /**< Coniferous forest and/or taiga */
		kDeciduous, /**< Deciduous and mixed forest */
		kGrasslands, /**< Grasslands with long grass */
		kHeath, /**< Heath, scrub or moor */
		kJungle, /**< Jungle or rainforest */
		kPlains, /**< Grasslands with short grass */
		kTundra, /**< Tundra */
	};

	/**
	 * @brief Get the enumeration value based on a string
	 *
	 * Accepts values that are returned by toString() in a case insensitive manner.
	 *
	 * @param sv string_view to get the enumeration for
	 * @return EnvironmentType::Vegetation represented by the string
	 *
	 * @see toString()
	 */
	const std::optional<Vegetation> vegetation(std::string_view sv);

	/**
	 * @brief Get the string representation of the given enum
	 * @param vegetation The EnvironmentType::Vegetation to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string toString(Vegetation vegetation);

	/**
	 * @brief Get the description of the given enum
	 *
	 * This gives more detail on the enum value for use in-game
	 *
	 * @param vegetation The EnvironmentType::Vegetation to get the string of
	 * @return Game display form of the enum value as a string
	 */
	const std::string description(Vegetation vegetation);

	/**
	 * @brief Teach operator<< how to print a EnvironmentType::Vegetation
	 * @param out Output stream that the enum should be printed to
	 * @param vegetation Enum value to output
	 * @return Output stream reference containing the output enum value
	 */
	inline std::ostream& operator<<(std::ostream& out, Vegetation vegetation) {
		return out << toString(vegetation);
	}

}