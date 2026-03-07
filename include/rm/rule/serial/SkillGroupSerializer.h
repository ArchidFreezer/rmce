#pragma once

#include <PersistentObjectSerializer.h>
#include <SkillGroupData.h>

namespace rm::rule::serial {
/**
 * @class SkillGroupSerializer
 * @brief JSON serializer for SkillGroupData objects
 */
class SkillGroupSerializer : public PersistentObjectSerializer<SkillGroupData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a SkillGroupData object */
	using PersistentObjectSerializer<SkillGroupData>::serializeObject;

	/**
	 * @brief Constructor for the SkillGroupSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	SkillGroupSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializer<SkillGroupData>(manager) {
	}

	/**
	 * @brief Serialize a SkillGroupData object to a JSON value
	 *
	 * @param ref Reference to the SkillGroupData object to serialize
	 * @return json::value containing the serialized SkillGroupData object
	 */
	json::value serializeObject(const SkillGroupData& ref) const override;

	/**
	 * @brief Deserialize a SkillGroupData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized SkillGroupData object
	 */
	const SkillGroupData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial