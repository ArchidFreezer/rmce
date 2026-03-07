#pragma once
#include <boost/json.hpp>
#include <PersistentSerializer.h>

namespace rm::rule::serial {

namespace json = boost::json;

/**
 * @class PersistentJsonSerializer
 * @brief Abstract class for serializing and deserializing persistent objects to and from JSON format.
 *
 * The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers.
 */
template<persistent_object PersistentObject>
class PersistentJsonSerializer : public PersistentSerializer<PersistentObject, json:: value, json::object> {
public:
	/**
	 * @brief Constructor for the PersistentJsonSerializer class.
	 *
	 * @param manager Reference to the PersistentObjectManager used for managing persistent objects during serialization and deserialization.
	 */
	PersistentJsonSerializer(rm::PersistentObjectManager& manager) : PersistentSerializer<PersistentObject, json::value, json::object>(manager) {
	}

	/**
	 * @brief Virtual destructor for the PersistentJsonSerializer class.
	 */
	virtual ~PersistentJsonSerializer() = default;

};

} // namespace rm::rule::serial
