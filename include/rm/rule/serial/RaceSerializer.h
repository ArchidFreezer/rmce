#pragma once

#include <PersistentObjectSerializer.h>
#include <RaceData.h>

namespace rm::rule::serial {
/**
 * @class RaceSerializer
 * @brief JSON serializer for RaceData objects
 */
class RaceSerializer : public PersistentObjectSerializer<RaceData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a RaceData object */
	using PersistentObjectSerializer<RaceData>::serializeObject;

	/**
	 * @brief Constructor for the RaceSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	RaceSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<RaceData>(manager) {
	}

	/**
	 * @brief Serialize a RaceData object to a JSON value
	 *
	 * @param ref Reference to the RaceData object to serialize
	 * @return json::value containing the serialized RaceData object
	 */
	json::value serializeObject(const RaceData& ref) const override;

	/**
	 * @brief Deserialize a RaceData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized RaceData object
	 */
	const RaceData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial