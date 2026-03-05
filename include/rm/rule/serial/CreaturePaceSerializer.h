#pragma once

#include <PersistentObjectSerializer.h>
#include <CreaturePaceData.h>

namespace rm::rule::serial {
/**
 * @class CreaturePaceSerializer
 * @brief JSON serializer for CreaturePaceData objects
 */
class CreaturePaceSerializer : public PersistentObjectSerializer<CreaturePaceData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a CreaturePaceData object */
	using PersistentObjectSerializer<CreaturePaceData>::serializeObject;

	/**
	 * @brief Constructor for the CreaturePaceSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	CreaturePaceSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<CreaturePaceData>(manager) {
	}

	/**
	 * @brief Serialize a CreaturePaceData object to a JSON value
	 *
	 * @param ref Reference to the CreaturePaceData object to serialize
	 * @return json::value containing the serialized CreaturePaceData object
	 */
	json::value serializeObject(const CreaturePaceData& ref) const override;

	/**
	 * @brief Deserialize a CreaturePaceData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized CreaturePaceData object
	 */
	const CreaturePaceData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial