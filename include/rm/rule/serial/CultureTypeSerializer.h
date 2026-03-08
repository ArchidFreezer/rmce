#pragma once

#include <PersistentJsonSerializer.h>
#include <CultureTypeData.h>

namespace rm::rule::serial {
/**
 * @class CultureTypeSerializer
 * @brief JSON serializer for CultureTypeData objects
 */
class CultureTypeSerializer : public PersistentJsonSerializer<CultureTypeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a CultureTypeData object */
	using PersistentJsonSerializer<CultureTypeData>::serializeObject;

	/**
	 * @brief Constructor for the CultureTypeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	CultureTypeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<CultureTypeData>(manager) {
	}

	/**
	 * @brief Serialize a CultureTypeData object to a JSON value
	 *
	 * @param ref Reference to the CultureTypeData object to serialize
	 * @return json::value containing the serialized CultureTypeData object
	 */
	json::value serializeObject(const CultureTypeData& ref) const override;

	/**
	 * @brief Deserialize a CultureTypeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized CultureTypeData object
	 */
	const CultureTypeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial