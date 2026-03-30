#pragma once

#include <PersistentJsonSerializer.h>
#include <PoisonData.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class PoisonSerializer
 * @brief JSON serializer for PoisonData objects
 */
class PoisonSerializer : public PersistentJsonSerializer<PoisonData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a PoisonData object */
	using PersistentJsonSerializer<PoisonData>::serializeObject;

	/**
	 * @brief Constructor for the PoisonSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	PoisonSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<PoisonData>(manager) {
	}

	/**
	 * @brief Serialize a PoisonData object to a JSON value
	 *
	 * @param ref Reference to the PoisonData object to serialize
	 * @return json::value containing the serialized PoisonData object
	 */
	json::value serializeObject(const PoisonData& ref) const override;

	/**
	 * @brief Deserialize a PoisonData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized PoisonData object
	 */
	const PoisonData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial