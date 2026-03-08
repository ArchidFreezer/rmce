#pragma once

#include <PersistentJsonSerializer.h>
#include <TreasureCodeData.h>

namespace rm::rule::serial {
/**
 * @class TreasureCodeSerializer
 * @brief JSON serializer for TreasureCodeData objects
 */
class TreasureCodeSerializer : public PersistentJsonSerializer<TreasureCodeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a TreasureCodeData object */
	using PersistentJsonSerializer<TreasureCodeData>::serializeObject;

	/**
	 * @brief Constructor for the TreasureCodeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	TreasureCodeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<TreasureCodeData>(manager) {
	}

	/**
	 * @brief Serialize a TreasureCodeData object to a JSON value
	 *
	 * @param ref Reference to the TreasureCodeData object to serialize
	 * @return json::value containing the serialized TreasureCodeData object
	 */
	json::value serializeObject(const TreasureCodeData& ref) const override;

	/**
	 * @brief Deserialize a TreasureCodeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized TreasureCodeData object
	 */
	const TreasureCodeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial