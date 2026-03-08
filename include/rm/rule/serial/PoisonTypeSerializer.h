#pragma once

#include <PersistentJsonSerializer.h>
#include <PoisonTypeData.h>

namespace rm::rule::serial {
/**
 * @class PoisonTypeSerializer
 * @brief JSON serializer for PoisonTypeData objects
 */
class PoisonTypeSerializer : public PersistentJsonSerializer<PoisonTypeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a PoisonTypeData object */
	using PersistentJsonSerializer<PoisonTypeData>::serializeObject;

	/**
	 * @brief Constructor for the PoisonTypeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	PoisonTypeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<PoisonTypeData>(manager) {
	}

	/**
	 * @brief Serialize a PoisonTypeData object to a JSON value
	 *
	 * @param ref Reference to the PoisonTypeData object to serialize
	 * @return json::value containing the serialized PoisonTypeData object
	 */
	json::value serializeObject(const PoisonTypeData& ref) const override;

	/**
	 * @brief Deserialize a PoisonTypeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized PoisonTypeData object
	 */
	const PoisonTypeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial