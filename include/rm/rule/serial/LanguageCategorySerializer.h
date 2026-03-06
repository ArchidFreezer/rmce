#pragma once

#include <PersistentObjectSerializer.h>
#include <LanguageCategoryData.h>

namespace rm::rule::serial {
/**
 * @class LanguageCategorySerializer
 * @brief JSON serializer for LanguageCategoryData objects
 */
class LanguageCategorySerializer : public PersistentObjectSerializer<LanguageCategoryData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a LanguageCategoryData object */
	using PersistentObjectSerializer<LanguageCategoryData>::serializeObject;

	/**
	 * @brief Constructor for the LanguageCategorySerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	LanguageCategorySerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<LanguageCategoryData>(manager) {
	}

	/**
	 * @brief Serialize a LanguageCategoryData object to a JSON value
	 *
	 * @param ref Reference to the LanguageCategoryData object to serialize
	 * @return json::value containing the serialized LanguageCategoryData object
	 */
	json::value serializeObject(const LanguageCategoryData& ref) const override;

	/**
	 * @brief Deserialize a LanguageCategoryData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized LanguageCategoryData object
	 */
	const LanguageCategoryData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial