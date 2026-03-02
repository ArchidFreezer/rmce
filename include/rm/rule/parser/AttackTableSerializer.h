#pragma once

#include <PersistentObjectJsonSerializer.h>
#include <table/AttackTable.h>

namespace rm::rule::parser {

/**
 * @class AttackTableSerializer
 * @brief Process AttackTable objects for json files
 *
 * This class can read a well-formed stream creating JSON formatted objects parsing the contents and adding them to a cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a stream in well-formed JSON format.
 *
 * @see AttackTable
 */

class AttackTableSerializer : public PersistentObjectJsonSerializer {
public:
	/**
	 * @brief Type alias for the data class handled by this serializer
	 */
	using DataClass = rm::rule::table::AttackTable;

	/**
	 * @brief Constructor
	 * @param object_manager Reference to an object manager to handle the data objects
	 */
	AttackTableSerializer(rm::PersistentObjectManager& object_manager) : PersistentObjectJsonSerializer(object_manager, "AttackTable") {
	}

private:
	/**
	 * @brief Parse a ptree into AttackTable objects
	 *
	 * Parse a boost::ptree containing the attack table rule data, convert to AttackTable objects and store in a data cache
	 */
	void parse(pt::ptree& ptree) override;

	/**
	 * @brief Populate the given boost tree with the data from an attack table
	 * @param key Id of the attacktable to populate from
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