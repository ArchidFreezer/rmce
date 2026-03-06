#pragma once

#include <PersistentObjectSerializer.h>
#include <ProfessionData.h>

namespace rm::rule::serial {
/**
 * @class ProfessionSerializer
 * @brief JSON serializer for ProfessionData objects
 */
class ProfessionSerializer : public PersistentObjectSerializer<ProfessionData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a ProfessionData object */
	using PersistentObjectSerializer<ProfessionData>::serializeObject;

	/**
	 * @brief Constructor for the ProfessionSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	ProfessionSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<ProfessionData>(manager) {
	}

	/**
	 * @brief Serialize a ProfessionData object to a JSON value
	 *
	 * @param ref Reference to the ProfessionData object to serialize
	 * @return json::value containing the serialized ProfessionData object
	 */
	json::value serializeObject(const ProfessionData& ref) const override;

	/**
	 * @brief Deserialize a ProfessionData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized ProfessionData object
	 */
	const ProfessionData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial