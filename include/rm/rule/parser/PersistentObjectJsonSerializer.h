#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <PersistentObjectSerializer.h>

namespace rm::rule::parser {

namespace pt = boost::property_tree;

/**
 * @class PersistentObjectJsonSerializer
 * @brief Class to serialize and deserialize persistent data objects to and from json files using boost property trees
 *
 * This class is a concrete implementation of the PersistentObjectSerializer interface to serialize and deserialize persistent data objects to and from json
 * files using boost property trees. The class uses the GameRuleBoostConverter utility class to convert persistent data objects to and from boost ptree objects
 * for serialization.
 */
class PersistentObjectJsonSerializer : public PersistentObjectSerializer {
public:
	/**
	 * @brief Consructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 * @param datatype String containing the name of the type of data being processed
	 */
	PersistentObjectJsonSerializer(rm::PersistentObjectManager& object_manager, std::string_view datatype)
	    : PersistentObjectSerializer(object_manager, datatype) {
	}

	virtual ~PersistentObjectJsonSerializer() = default; /**< Default destructor to allow for polymorphism */

	/**
	 * @brief Read JSON data from an input stream, convert to objects and store in the persistent data cache
	 *
	 * Uses the parse function to perform the conversion
	 *
	 * @param is Input stream to read from
	 */
	void read(std::istream& is);

	/**
	 * @brief Write all persistent objects of a given type from the cache to a file
	 * 
	 * The function retrieves all the keys of the objects of the given type from the cache and uses the populateDatum function to convert each object to a boost
	 * ptree, which is then written to the output stream as a json array under the root node specified by the rootNode function.
	 * 
	 * @tparam PersistentObject Type of persistent object to write to the file, must be derived from Persistent
	 * @param os Output stream to write the output to
	 */
	template<persistent_object PersistentObject>
	void save(std::ostream& os);

protected:
	/**
	 * @brief Parse a ptree into game data objects
	 *
	 * Parse a boost::ptree containing the language rule data, convert to objects and store in a data cache
	 */
	virtual void parse(pt::ptree& ptree) = 0;

	/**
	 * @brief Populate a ptree with the data from a persistent object
	 *
	 * Populate a boost::ptree with the data from a persistent object, converting it to a format suitable for writing to a json stream
	 *
	 * @param key Key of the object type to populate the ptree with
	 * @param datum Reference to the ptree to populate with the data
	 */
	virtual void populateDatum(std::string key, pt::ptree& datum) = 0;

	/**
	 * @brief Get the name of the root node to write the data to in the json file
	 *
	 * This is used to ensure that the data is written to the correct location in the json file.
	 *
	 * @return String containing the name of the root node to write the data to in the json file.
	 */
	virtual std::string rootNode() const = 0; /**< Get the name of the root node to write the data to in the json file */
};

} // namespace rm::rule::parser
