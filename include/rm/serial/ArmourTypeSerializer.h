#pragma once

#include <PersistentJsonSerializer.h>
#include <ArmourTypeData.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class ArmourTypeSerializer
 * @brief JSON serializer for ArmourTypeData objects
 */
class ArmourTypeSerializer : public PersistentJsonSerializer<ArmourTypeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a ArmourTypeData object */
	using PersistentJsonSerializer<ArmourTypeData>::serializeObject;

	/**
	 * @brief Constructor for the ArmourTypeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	ArmourTypeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<ArmourTypeData>(manager) {
	}

	/**
	 * @brief Serialize a ArmourTypeData object to a JSON value
	 *
	 * @param ref Reference to the ArmourTypeData object to serialize
	 * @return json::value containing the serialized ArmourTypeData object
	 */
	json::value serializeObject(const ArmourTypeData& ref) const override;

	/**
	 * @brief Deserialize a ArmourTypeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized ArmourTypeData object
	 */
	const ArmourTypeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial