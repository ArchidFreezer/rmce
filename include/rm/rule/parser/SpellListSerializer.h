#pragma once

#include <PersistentObjectJsonSerializer.h>
#include <SpellListData.h>

namespace rm::rule::parser {

/**
 * @class SpellListSerializer
 * @brief Process SpellListData objects for json files
 *
 * This class can read a well-formed stream creating JSON formatted objects parsing the contents and adding them to a cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a stream in well-formed JSON format.
 *
 * @see SpellListData
 */

class SpellListSerializer : public PersistentObjectJsonSerializer {
public:
	/**
	 * @brief Type alias for the data class handled by this serializer
	 */
	using DataClass = SpellListData;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 */
	SpellListSerializer(rm::PersistentObjectManager& object_manager) : PersistentObjectJsonSerializer(object_manager, "SpellList") {
	}

private:
	/**
	 * @brief Parse a ptree into SpellListData objects
	 *
	 * Parse a boost::ptree containing the spell list rule data, convert to SpellListData objects and store in a data cache
	 */
	void parse(pt::ptree& ptree) override;

	/**
	 * @brief Populate the given boost tree with the data from a spell list
	 * @param key Id of the spell list to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string key, pt::ptree& datum) override;

	/**
	 * @brief Getter for the root node name to use when writing the data to a json file
	 * @return String containing the root node name to use when writing the data to a json file
	 */
	std::string rootNode() const;
};

} // namespace rm::rule::parser