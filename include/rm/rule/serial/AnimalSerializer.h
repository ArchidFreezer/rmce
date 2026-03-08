#pragma once

#include <PersistentJsonSerializer.h>
#include <AnimalData.h>
#include <JsonConverter.h>

namespace rm::rule::serial {
/**
 * @class AnimalSerializer
 * @brief JSON serializer for AnimalData objects
 */
class AnimalSerializer : public PersistentJsonSerializer<AnimalData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a AnimalData object */
	using PersistentJsonSerializer<AnimalData>::serializeObject;

	/**
	 * @brief Constructor for the AnimalSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	AnimalSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<AnimalData>(manager) {
	}

	/**
	 * @brief Serialize a AnimalData object to a JSON value
	 *
	 * @param ref Reference to the AnimalData object to serialize
	 * @return json::value containing the serialized AnimalData object
	 */
	json::value serializeObject(const AnimalData& ref) const override;

	/**
	 * @brief Deserialize a AnimalData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized AnimalData object
	 */
	const AnimalData& deserializeObject(json::object& jsonObj) const override;

private:
	/**
	 * @brief Helper method to parse an AnimalAttack object from a JSON object
	 *
	 * @param jsonObj JSON object containing the data to parse
	 * @param attack Reference to the AnimalAttack object to populate with the parsed data
	 * @param chance Reference to a pair of integers to populate with the parsed chance data, where the first integer is the minimum chance and the second integer is the maximum chance. This is used to allow us to return the chance data
	 * without having to modify the AnimalAttack object, which does not have a setter for the chance data.
	 */
	void deserializeAnimalAttack(json::object& jsonObj, rm::game::AnimalAttack& attack, std::pair<int, int>& chance, bool use_chance) const;

	/**
	 * @brief Helper method to serialize an AnimalAttack object to a JSON object
	 *
	 * @param jsonObj JSON object to populate with the serialized data
	 * @param attack Reference to the AnimalAttack object to serialize
	 */
	void serializeAnimalAttack(JsonConverter::NestedBuilder& location_builder, const rm::game::AnimalAttack& attack) const;
};
} // namespace rm::rule::serial