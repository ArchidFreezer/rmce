#pragma once

#include <PersistentJsonSerializer.h>
#include <DiseaseTypeData.h>

namespace rm::rule::serial {
/**
 * @class DiseaseTypeSerializer
 * @brief JSON serializer for DiseaseTypeData objects
 */
class DiseaseTypeSerializer : public PersistentJsonSerializer<DiseaseTypeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a DiseaseTypeData object */
	using PersistentJsonSerializer<DiseaseTypeData>::serializeObject;

	/**
	 * @brief Constructor for the DiseaseTypeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	DiseaseTypeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<DiseaseTypeData>(manager) {
	}

	/**
	 * @brief Serialize a DiseaseTypeData object to a JSON value
	 *
	 * @param ref Reference to the DiseaseTypeData object to serialize
	 * @return json::value containing the serialized DiseaseTypeData object
	 */
	json::value serializeObject(const DiseaseTypeData& ref) const override;

	/**
	 * @brief Deserialize a DiseaseTypeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized DiseaseTypeData object
	 */
	const DiseaseTypeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial