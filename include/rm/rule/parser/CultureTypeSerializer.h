#pragma once

#include <PersistentObjectJsonSerializer.h>
#include <CultureTypeData.h>

namespace rm::rule::parser {

/**
 * @class CultureTypeSerializer
 * @brief Process CultureTypeData objects for json files
 *
 * This class can read a well-formed stream creating JSON formatted objects parsing the contents and adding them to a cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a stream in well-formed JSON format.
 *
 * @see CultureTypeData
 */

class CultureTypeSerializer : public PersistentObjectJsonSerializer {
public:
	/**
	 * @brief Type alias for the data class handled by this serializer
	 */
	using DataClass = CultureTypeData;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 */
	CultureTypeSerializer(rm::PersistentObjectManager& object_manager) : PersistentObjectJsonSerializer(object_manager, "CultureType") {
	}

private:
	/**
	 * @brief Parse a ptree into CultureTypeData objects
	 *
	 * Parse a boost::ptree containing the culture type rule data, convert to CultureTypeData objects and store in a data cache
	 */
	void parse(pt::ptree& ptree) override;

	/**
	 * @brief Populate the given boost tree with the data from a culture type
	 * @param key Id of the culture type to populate from
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