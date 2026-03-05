#pragma once

#include <PersistentObjectSerializer.h>
#include <table/AttackTable.h>

namespace rm::rule::serial {
/**
 * @class AttackTableSerializer
 * @brief JSON serializer for AttackTableData objects
 */
class AttackTableSerializer : public PersistentObjectSerializer<rm::rule::table::AttackTable> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a BookData object */
	using PersistentObjectSerializer<rm::rule::table::AttackTable>::serializeObject;

	/**
	 * @brief Constructs an AttackTableSerializer with the specified PersistentObjectManager.
	 * @param manager The PersistentObjectManager used to manage persistent objects during serialization and deserialization.
	 */
	AttackTableSerializer(rm::PersistentObjectManager& manager)	: PersistentObjectSerializer<rm::rule::table::AttackTable>(manager) {}

	/**
	 * @brief Serializes an AttackTable object to a JSON value. 
	 * @param ref The AttackTable object to serialize.
	 * @return A json::value containing the serialized AttackTable object.
	 */
	json::value serializeObject(const rm::rule::table::AttackTable& ref) const override;

	/**
	 * @brief Deserializes an AttackTable object from a JSON object.
	 * @param jsonObj The JSON object containing the data to deserialize.
	 * @return A reference to the deserialized AttackTable object.
	 */
	const rm::rule::table::AttackTable& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::rule::serial