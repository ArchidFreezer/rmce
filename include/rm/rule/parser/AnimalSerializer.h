#pragma once

#include <PersistentObjectJsonSerializer.h>
#include <AnimalData.h>

namespace rm::rule::parser {

/**
 * @class AnimalSerializer
 * @brief Process AnimalData objects for json files
 *
 * This class can read a well-formed stream creating JSON formatted objects parsing the contents and adding them to a cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a stream in well-formed JSON format.
 *
 * @see AnimalData
 */

class AnimalSerializer : public PersistentObjectJsonSerializer {
public:
	/**
	 * @brief Type alias for the data class handled by this serializer
	 */
	using DataClass = AnimalData;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 */
	AnimalSerializer(rm::PersistentObjectManager& object_manager) : PersistentObjectJsonSerializer(object_manager, "Animal") {
	}

private:
	/**
	 * @brief Parse a ptree into AnimalData objects
	 *
	 * Parse a boost::ptree containing the animal rule datas, convert to AnimalData objects and store in a data cache
	 */
	void parse(pt::ptree& ptree) override;

	/**
	 * @brief Populate the given boost tree with the data from a animal
	 * @param key Id of the animal to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string key, pt::ptree& datum) override;

	/**
	 * @brief Getter for the root node name to use when writing the data to a json file
	 * @return String containing the root node name to use when writing the data to a json file
	 */
	std::string rootNode() const;

			/**
	 * @brief Parse an AnimalAttack object and write into a boost ptree
	 *
	 * Parse a boost::ptree containing the data for an animal attack and populate an AnimalAttack object with that data
	 *
	 * @param tree Boost ptree to populate with the data from the object
	 * @param ref Reference to the AnimalAttack object containing the data for the animal attack
	 */
	void populateAnimalAttack(pt::ptree& tree, const rm::game::AnimalAttack& attack);

	/**
	 * @brief Parse a ptree into an AnimalAttack object
	 *
	 * Parse a boost::ptree containing the data for an animal attack and populate an AnimalAttack object with that data
	 *
	 * @param tree Boost ptree containing the data for the animal attack
	 * @param ref Reference to the AnimalAttack object to populate with the data from the boost ptree
	 * @param parse_chance Whether to parse the chance value for the attack, this is used to allow parsing of attacks that are used in a context where the
	 * chance value is not relevant such as conditional attacks.
	 */
	void parseAnimalAttack(rm::game::AnimalAttack& attack, const pt::ptree& tree, bool parse_chance = true);
};

} // namespace rm::rule::parser