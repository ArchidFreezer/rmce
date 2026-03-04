#pragma once
#include <string>
#include <boost/json.hpp>
#include <PersistentObjectManager.h>

namespace rm::rule::serial {

namespace json = boost::json;

/**
 * @class PersistentObjectSerializer
 * @brief Abstract class for serializing and deserializing persistent objects to and from JSON format.
 *
 * The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers.
 */
template<persistent_object PersistentObject>
class PersistentObjectSerializer {
public:
	/**
	 * @brief Constructor for the PersistentObjectSerializer class.
	 *
	 * @param manager Reference to the PersistentObjectManager used for managing persistent objects during serialization and deserialization.
	 */
	PersistentObjectSerializer(rm::PersistentObjectManager& manager) : manager_(manager) {
	}

	/**
	 * @brief Virtual destructor for the PersistentObjectSerializer class.
	 */
	virtual ~PersistentObjectSerializer() = default;

	/*
	 * We delete all the copy/move constructors and assignment operators to prevent slicing and ensure that the class is only used as a base class for specific serializers.
	 */
	PersistentObjectSerializer(const PersistentObjectSerializer&) = delete;            /**< Delete the copy constructor */
	PersistentObjectSerializer& operator=(const PersistentObjectSerializer&) = delete; /**< Delete the copy assignment operator */
	PersistentObjectSerializer(PersistentObjectSerializer&&) = delete;                 /**< Delete the move constructor */
	PersistentObjectSerializer& operator=(PersistentObjectSerializer&&) = delete;      /**< Delete the move assignment operator */

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
