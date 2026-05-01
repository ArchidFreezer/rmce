#include <AutoCharacterBuilderSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value AutoCharacterBuilderSerializer::serializeObject(const AutoCharacterBuilder& ref) const {
	json::object obj;
	JsonConverter::setString(obj, "id", ref.id());

	return obj;
}

const AutoCharacterBuilder& AutoCharacterBuilderSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");

	// If this object does not have an ID, create a new AutoCharacterBuilder to get a cached object with a generated ID.
	if (id.empty()) {
		AutoCharacterBuilder& tmp = manager_.get<AutoCharacterBuilder>();
		id = tmp.id(); // Generate a new ID for this character builder
	}

	// Now we know we have an id, we can get the reference to the AutoCharacterBuilder that we will populate with data. This will be either the newly created one (if there was no ID in the JSON) or an existing one (if there was an ID in the
	// JSON).
	AutoCharacterBuilder& ref = manager_.get<AutoCharacterBuilder>(id);

	// No data to deserialize for this class as it is just a helper for the character builder and does not have any state of its own.

	return ref;
}

} // namespace rm::serial