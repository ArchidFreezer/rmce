#pragma once

#include <PersistentJsonSerializer.h>
#include <CultureData.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class CultureSerializer
 * @brief JSON serializer for CultureData objects
 */
class CultureSerializer : public PersistentJsonSerializer<CultureData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a CultureData object */
	using PersistentJsonSerializer<CultureData>::serializeObject;

	/**
	 * @brief Constructor for the CultureSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	CultureSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<CultureData>(manager) {
	}

	/**
	 * @brief Serialize a CultureData object to a JSON value
	 *
	 * @param ref Reference to the CultureData object to serialize
	 * @return json::value containing the serialized CultureData object
	 */
	json::value serializeObject(const CultureData& ref) const override;

	/**
	 * @brief Deserialize a CultureData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized CultureData object
	 */
	const CultureData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial