#pragma once

#include <PersistentJsonSerializer.h>
#include <table/SpecialAttackTable.h>

namespace rm::serial {
/**
 * @class SpecialAttackTableSerializer
 * @brief JSON serializer for SpecialAttackTableData objects
 */
class SpecialAttackTableSerializer : public PersistentJsonSerializer<rm::rule::table::SpecialAttackTable> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a BookData object */
	using PersistentJsonSerializer<rm::rule::table::SpecialAttackTable>::serializeObject;

	/**
	 * @brief Constructs an SpecialAttackTableSerializer with the specified PersistentObjectManager.
	 * @param manager The PersistentObjectManager used to manage persistent objects during serialization and deserialization.
	 */
	SpecialAttackTableSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<rm::rule::table::SpecialAttackTable>(manager) {
	}

	/**
	 * @brief Serializes an SpecialAttackTable object to a JSON value.
	 * @param ref The SpecialAttackTable object to serialize.
	 * @return A json::value containing the serialized SpecialAttackTable object.
	 */
	json::value serializeObject(const rm::rule::table::SpecialAttackTable& ref) const override;

	/**
	 * @brief Deserializes an SpecialAttackTable object from a JSON object.
	 * @param jsonObj The JSON object containing the data to deserialize.
	 * @return A reference to the deserialized SpecialAttackTable object.
	 */
	const rm::rule::table::SpecialAttackTable& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial