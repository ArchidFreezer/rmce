#pragma once

#include <PersistentObjectSerializer.h>
#include <SkillCategoryData.h>

namespace rm::rule::serial {
/**
 * @class SkillCategorySerializer
 * @brief JSON serializer for SkillCategoryData objects
 */
class SkillCategorySerializer : public PersistentObjectSerializer<SkillCategoryData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a SkillCategoryData object */
	using PersistentObjectSerializer<SkillCategoryData>::serializeObject;

	/**
	 * @brief Constructor for the SkillCategorySerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	SkillCategorySerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<SkillCategoryData>(manager) {
	}

	/**
	 * @brief Serialize a SkillCategoryData object to a JSON value
	 *
	 * @param ref Reference to the SkillCategoryData object to serialize
	 * @return json::value containing the serialized SkillCategoryData object
	 */
	json::value serializeObject(const SkillCategoryData& ref) const override;

	/**
	 * @brief Deserialize a SkillCategoryData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized SkillCategoryData object
	 */
	const SkillCategoryData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial