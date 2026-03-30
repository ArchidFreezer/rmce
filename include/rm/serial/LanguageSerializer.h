#pragma once

#include <PersistentJsonSerializer.h>
#include <LanguageData.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class LanguageSerializer
 * @brief JSON serializer for LanguageData objects
 */
class LanguageSerializer : public PersistentJsonSerializer<LanguageData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a LanguageData object */
	using PersistentJsonSerializer<LanguageData>::serializeObject;

	/**
	 * @brief Constructor for the LanguageSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	LanguageSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<LanguageData>(manager) {
	}

	/**
	 * @brief Serialize a LanguageData object to a JSON value
	 *
	 * @param ref Reference to the LanguageData object to serialize
	 * @return json::value containing the serialized LanguageData object
	 */
	json::value serializeObject(const LanguageData& ref) const override;

	/**
	 * @brief Deserialize a LanguageData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized LanguageData object
	 */
	const LanguageData& deserializeObject(json::object& jsonObj) const override;
};
} // namespace rm::serial