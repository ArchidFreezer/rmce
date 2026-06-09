#pragma once

#include <PersistentJsonSerializer.h>
#include <ForagableData.h>
#include <JsonConverter.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class ForagableSerializer
 * @brief JSON serializer for ForagableData objects
 */
class ForagableSerializer : public PersistentJsonSerializer<ForagableData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a ForagableData object */
	using PersistentJsonSerializer<ForagableData>::serializeObject;

	/**
	 * @brief Constructor for the ForagableSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	ForagableSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<ForagableData>(manager) {
	}

	/**
	 * @brief Serialize a ForagableData object to a JSON value
	 *
	 * @param ref Reference to the ForagableData object to serialize
	 * @return json::value containing the serialized ForagableData object
	 */
	json::value serializeObject(const ForagableData& ref) const override;

	/**
	 * @brief Deserialize a ForagableData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized ForagableData object
	 */
	const ForagableData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial