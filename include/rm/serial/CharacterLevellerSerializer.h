#pragma once

#include <PersistentJsonSerializer.h>
#include <CharacterLeveller.h>

namespace rm::serial {

using namespace rm::game::character;

/**
 * @class CharacterLevellerSerializer
 * @brief JSON serializer for CharacterLeveller objects
 */
class CharacterLevellerSerializer : public PersistentJsonSerializer<CharacterLeveller> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a CharacterLeveller object */
	using PersistentJsonSerializer<CharacterLeveller>::serializeObject;

	/**
	 * @brief Constructor for the CharacterLevellerSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	CharacterLevellerSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<CharacterLeveller>(manager) {
	}

	/**
	 * @brief Serialize a CharacterLeveller object to a JSON value
	 *
	 * @param ref Reference to the CharacterLeveller object to serialize
	 * @return json::value containing the serialized CharacterLeveller object
	 */
	json::value serializeObject(const CharacterLeveller& ref) const override;

	/**
	 * @brief Deserialize a CharacterLeveller object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized CharacterLeveller object
	 */
	const CharacterLeveller& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial