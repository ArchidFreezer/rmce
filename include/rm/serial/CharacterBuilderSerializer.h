#pragma once

#include <PersistentJsonSerializer.h>
#include <CharacterBuilder.h>

namespace rm::serial {

using namespace rm::game::character;

/**
 * @class CharacterBuilderSerializer
 * @brief JSON serializer for CharacterBuilder objects
 */
class CharacterBuilderSerializer : public PersistentJsonSerializer<CharacterBuilder> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a CharacterBuilder object */
	using PersistentJsonSerializer<CharacterBuilder>::serializeObject;

	/**
	 * @brief Constructor for the CharacterBuilderSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	CharacterBuilderSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<CharacterBuilder>(manager) {
	}

	/**
	 * @brief Serialize a CharacterBuilder object to a JSON value
	 *
	 * @param ref Reference to the CharacterBuilder object to serialize
	 * @return json::value containing the serialized CharacterBuilder object
	 */
	json::value serializeObject(const CharacterBuilder& ref) const override;

	/**
	 * @brief Deserialize a CharacterBuilder object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized CharacterBuilder object
	 */
	const CharacterBuilder& deserializeObject(json::object& jsonObj) const override;

	/**
	 * @brief Serialize the available hobby choices for a CharacterBuilder object to a JSON value
	 *
	 * This function is used to serialize the available hobby choices for a character being created, which are determined based on the character's initial choices and stats. The returned JSON value contains the necessary information for the
	 * client to display the available hobby choices to the player during character creation.
	 *
	 * @param ref Reference to the CharacterBuilder object for which to serialize the hobby choices
	 * @return json::value containing the serialized hobby choices for the CharacterBuilder object
	 */
	json::value serializeHobbyChoices(const CharacterBuilder& ref) const;
};
} // namespace rm::serial