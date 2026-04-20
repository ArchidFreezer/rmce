#pragma once

#include <PersistentJsonSerializer.h>
#include <Character.h>

namespace rm::serial {

using namespace rm::game::character;

/**
 * @class CharacterSerializer
 * @brief JSON serializer for Character objects
 */
class CharacterSerializer : public PersistentJsonSerializer<Character> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a Character object */
	using PersistentJsonSerializer<Character>::serializeObject;

	/**
	 * @brief Constructor for the CharacterSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	CharacterSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<Character>(manager) {
	}

	/**
	 * @brief Serialize a Character object to a JSON value
	 *
	 * @param ref Reference to the Character object to serialize
	 * @return json::value containing the serialized Character object
	 */
	json::value serializeObject(const Character& ref) const override;

	/**
	 * @brief Deserialize a Character object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized Character object
	 */
	const Character& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial