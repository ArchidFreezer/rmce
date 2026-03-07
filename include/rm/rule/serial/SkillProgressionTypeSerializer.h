#pragma once

#include <PersistentJsonSerializer.h>
#include <SkillProgressionTypeData.h>

namespace rm::rule::serial {
/**
 * @class SkillProgressionTypeSerializer
 * @brief JSON serializer for SkillProgressionTypeData objects
 */
class SkillProgressionTypeSerializer : public PersistentJsonSerializer<SkillProgressionTypeData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a SkillProgressionTypeData object */
	using PersistentJsonSerializer<SkillProgressionTypeData>::serializeObject;

	/**
	 * @brief Constructor for the SkillProgressionTypeSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	SkillProgressionTypeSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<SkillProgressionTypeData>(manager) {
	}

	/**
	 * @brief Serialize a SkillProgressionTypeData object to a JSON value
	 *
	 * @param ref Reference to the SkillProgressionTypeData object to serialize
	 * @return json::value containing the serialized SkillProgressionTypeData object
	 */
	json::value serializeObject(const SkillProgressionTypeData& ref) const override;

	/**
	 * @brief Deserialize a SkillProgressionTypeData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized SkillProgressionTypeData object
	 */
	const SkillProgressionTypeData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial