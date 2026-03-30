#pragma once

#include <PersistentJsonSerializer.h>
#include <WeaponTypeData.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class WeaponTypeSerializer
 * @brief JSON serializer for WeaponTypeData objects
 */
class WeaponTypeSerializer : public PersistentJsonSerializer<WeaponTypeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a WeaponTypeData object */
	using PersistentJsonSerializer<WeaponTypeData>::serializeObject;

	/**
	 * @brief Constructor for the WeaponTypeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	WeaponTypeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<WeaponTypeData>(manager) {
	}

	/**
	 * @brief Serialize a WeaponTypeData object to a JSON value
	 *
	 * @param ref Reference to the WeaponTypeData object to serialize
	 * @return json::value containing the serialized WeaponTypeData object
	 */
	json::value serializeObject(const WeaponTypeData& ref) const override;

	/**
	 * @brief Deserialize a WeaponTypeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized WeaponTypeData object
	 */
	const WeaponTypeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial