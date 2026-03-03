#pragma once

#include <PersistentObjectSerializerJson.h>
#include <BookData.h>

namespace rm::rule::serial {
/**
 * @class BookJsonSerializer
 * @brief JSON serializer for BookData objects
 */
class BookJsonSerializer : public PersistentObjectSerializerJson<BookData> {
public:

	/**
	 * @brief Constructor for the BookJsonSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	BookJsonSerializer(rm::PersistentObjectManager& manager) : PersistentObjectSerializerJson<BookData>(manager) {
	}

	/**
	 * @brief Serialize a BookData object to a JSON value
	 *
	 * @param book Reference to the BookData object to serialize
	 * @return json::value containing the serialized BookData object
	 */
	json::value serializeObject(const BookData& book) override;

	/**
	 * @brief Serialize a BookData object to a JSON value by its ID
	 *
	 * @param id Unique ID of the BookData object to serialize
	 * @return json::value containing the serialized BookData object
	 */
	json::value serializeObject(const std::string& id) override {
		return serializeObject(manager_.get<BookData>(id));
	}

	/**
	 * @brief Deserialize a BookData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized BookData object
	 */
	const BookData& deserializeObject(json::object& jsonObj) override;
};
} // namespace rm::rule::serial