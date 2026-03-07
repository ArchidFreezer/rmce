#pragma once

#include <PersistentJsonSerializer.h>
#include <SkillData.h>

namespace rm::rule::serial {
/**
 * @class SkillSerializer
 * @brief JSON serializer for SkillData objects
 */
class SkillSerializer : public PersistentJsonSerializer<SkillData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a SkillData object */
	using PersistentJsonSerializer<SkillData>::serializeObject;

	/**
	 * @brief Constructor for the SkillSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	SkillSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<SkillData>(manager) {
	}

	/**
	 * @brief Serialize a SkillData object to a JSON value
	 *
	 * @param ref Reference to the SkillData object to serialize
	 * @return json::value containing the serialized SkillData object
	 */
	json::value serializeObject(const SkillData& ref) const override;

	/**
	 * @brief Deserialize a SkillData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized SkillData object
	 */
	const SkillData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial