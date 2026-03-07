#pragma once

#include <PersistentJsonSerializer.h>
#include <SpellListData.h>

namespace rm::rule::serial {
/**
 * @class SpellListSerializer
 * @brief JSON serializer for SpellListData objects
 */
class SpellListSerializer : public PersistentJsonSerializer<SpellListData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a SpellListData object */
	using PersistentJsonSerializer<SpellListData>::serializeObject;

	/**
	 * @brief Constructor for the SpellListSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	SpellListSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<SpellListData>(manager) {
	}

	/**
	 * @brief Serialize a SpellListData object to a JSON value
	 *
	 * @param ref Reference to the SpellListData object to serialize
	 * @return json::value containing the serialized SpellListData object
	 */
	json::value serializeObject(const SpellListData& ref) const override;

	/**
	 * @brief Deserialize a SpellListData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized SpellListData object
	 */
	const SpellListData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial