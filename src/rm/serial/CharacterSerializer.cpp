#include <CharacterSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value CharacterSerializer::serializeObject(const Character& ref) const {
	json::object obj;

	// Identity
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name_);

	return obj;
}

const Character& CharacterSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");

	// If this object does not have an ID, create a new Character to get a cached object with a generated ID.
	if (id.empty()) {
		Character& tmp = manager_.get<Character>();
		id = tmp.id(); // Generate a new ID for this character builder
	}

	// Now we know we have an id, we can get the reference to the Character that we will populate with data. This will be either the newly created one (if there was no ID in the JSON) or an existing one (if there was an ID in the
	// JSON).
	Character& ref = manager_.get<Character>(id);

	return ref;
}

} // namespace rm::serial