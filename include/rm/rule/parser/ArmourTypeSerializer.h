#pragma once

#include <PersistentObjectJsonSerializer.h>
#include <ArmourTypeData.h>

namespace rm::rule::parser {

/**
 * @class ArmourTypeSerializer
 * @brief Process ArmourTypeData objects for json files
 *
 * This class can read a well-formed stream creating JSON formatted objects parsing the contents and adding them to a cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a stream in well-formed JSON format.
 *
 * @see ArmourTypeData
 */

class ArmourTypeSerializer : public PersistentObjectJsonSerializer {
public:
	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 */
	ArmourTypeSerializer(rm::PersistentObjectManager& object_manager) : PersistentObjectJsonSerializer(object_manager, "ArmourType") {
	}

private:
	/**
	 * @brief Parse a ptree into ArmourTypeData objects
	 *
	 * Parse a boost::ptree containing the armour type rule datas, convert to ArmourTypeData objects and store in a data cache
	 */
	void parse(pt::ptree& ptree) override;

	/**
	 * @brief Populate the given boost tree with the data from an armour type
	 * @param key Id of the armour type to populate from
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