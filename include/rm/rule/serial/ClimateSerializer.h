#pragma once

#include <PersistentObjectSerializer.h>
#include <ClimateData.h>

namespace rm::rule::serial {
/**
 * @class ClimateSerializer
 * @brief JSON serializer for ClimateData objects
 */
class ClimateSerializer : public PersistentObjectSerializer<ClimateData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a ClimateData object */
	using PersistentObjectSerializer<ClimateData>::serializeObject;

	/**
	 * @brief Constructor for the ClimateSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	ClimateSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<ClimateData>(manager) {
	}

	/**
	 * @brief Serialize a ClimateData object to a JSON value
	 *
	 * @param ref Reference to the ClimateData object to serialize
	 * @return json::value containing the serialized ClimateData object
	 */
	json::value serializeObject(const ClimateData& ref) const override;

	/**
	 * @brief Deserialize a ClimateData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized ClimateData object
	 */
	const ClimateData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial