#pragma once

#include <PersistentJsonSerializer.h>
#include <AutoCharacterBuilder.h>

namespace rm::serial {

using namespace rm::game::character;

/**
 * @class AutoCharacterBuilderSerializer
 * @brief JSON serializer for AutoCharacterBuilder objects
 */
class AutoCharacterBuilderSerializer : public PersistentJsonSerializer<AutoCharacterBuilder> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a AutoCharacterBuilder object */
	using PersistentJsonSerializer<AutoCharacterBuilder>::serializeObject;

	/**
	 * @brief Constructor for the AutoCharacterBuilderSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	AutoCharacterBuilderSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<AutoCharacterBuilder>(manager) {
	}

	/**
	 * @brief Serialize a AutoCharacterBuilder object to a JSON value
	 *
	 * @param ref Reference to the AutoCharacterBuilder object to serialize
	 * @return json::value containing the serialized AutoCharacterBuilder object
	 */
	json::value serializeObject(const AutoCharacterBuilder& ref) const override;

	/**
	 * @brief Deserialize a AutoCharacterBuilder object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized AutoCharacterBuilder object
	 */
	const AutoCharacterBuilder& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial