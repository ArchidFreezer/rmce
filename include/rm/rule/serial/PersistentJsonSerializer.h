#pragma once
#include <string>
#include <boost/json.hpp>
#include <PersistentObjectManager.h>

namespace rm::rule::serial {

namespace json = boost::json;

/**
 * @class PersistentJsonSerializer
 * @brief Abstract class for serializing and deserializing persistent objects to and from JSON format.
 *
 * The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers.
 */
template<persistent_object PersistentObject>
class PersistentJsonSerializer {
public:
	/**
	 * @brief Constructor for the PersistentJsonSerializer class.
	 *
	 * @param manager Reference to the PersistentObjectManager used for managing persistent objects during serialization and deserialization.
	 */
	PersistentJsonSerializer(rm::PersistentObjectManager& manager) : manager_(manager) {
	}

	/**
	 * @brief Virtual destructor for the PersistentJsonSerializer class.
	 */
	virtual ~PersistentJsonSerializer() = default;

	/*
	 * We delete all the copy/move constructors and assignment operators to prevent slicing and ensure that the class is only used as a base class for specific serializers.
	 */
	PersistentJsonSerializer(const PersistentJsonSerializer&) = delete;            /**< Delete the copy constructor */
	PersistentJsonSerializer& operator=(const PersistentJsonSerializer&) = delete; /**< Delete the copy assignment operator */
	PersistentJsonSerializer(PersistentJsonSerializer&&) = delete;                 /**< Delete the move constructor */
	PersistentJsonSerializer& operator=(PersistentJsonSerializer&&) = delete;      /**< Delete the move assignment operator */

	/**
	 * @brief Serialize a single object to a JSON value
	 *
	 * @param obj Reference to the object to serialize
	 * @return json::value containing the serialized object
	 */
	virtual json::value serializeObject(const PersistentObject& obj) const = 0;

	/**
	 * @brief Serialize a single object to a JSON value by its ID
	 *
	 * @param id Unique ID of the object to serialize
	 * @return json::value containing the serialized object
	 */
	json::value serializeObject(const std::string& id) const {
		return serializeObject(manager_.get<PersistentObject>(id));
	}

	/**
	 * @brief Deserialize a single object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized object
	 */
	virtual const PersistentObject& deserializeObject(json::object& jsonObj) const = 0;

	/**
	 * @brief Get the PersistentObjectManager used by this serializer
	 *
	 * @return Reference to the PersistentObjectManager
	 */
	rm::PersistentObjectManager& manager() const {
		return manager_;
	}

protected:
	rm::PersistentObjectManager& manager_; /**< Object manager to retrive persistent objects from or add to */
};

} // namespace rm::rule::serial
