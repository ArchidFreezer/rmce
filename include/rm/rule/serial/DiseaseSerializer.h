#pragma once

#include <PersistentObjectSerializer.h>
#include <DiseaseData.h>

namespace rm::rule::serial {
/**
 * @class DiseaseSerializer
 * @brief JSON serializer for DiseaseData objects
 */
class DiseaseSerializer : public PersistentObjectSerializer<DiseaseData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a DiseaseData object */
	using PersistentObjectSerializer<DiseaseData>::serializeObject;

	/**
	 * @brief Constructor for the DiseaseSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	DiseaseSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<DiseaseData>(manager) {
	}

	/**
	 * @brief Serialize a DiseaseData object to a JSON value
	 *
	 * @param ref Reference to the DiseaseData object to serialize
	 * @return json::value containing the serialized DiseaseData object
	 */
	json::value serializeObject(const DiseaseData& ref) const override;

	/**
	 * @brief Deserialize a DiseaseData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized DiseaseData object
	 */
	const DiseaseData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial