#include <AutoCharacterBuilderSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value AutoCharacterBuilderSerializer::serializeObject(const AutoCharacterBuilder& ref) const {
	json::object obj;
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name_);

	/*
	 * Character trait varaibles used to assist in choices
	 */
	JsonConverter::setInt(obj, "aggression", ref.aggression_);
	JsonConverter::setInt(obj, "combatCloseness", ref.combat_closeness_);
	JsonConverter::setInt(obj, "combatCasting", ref.combat_casting_);
	JsonConverter::setInt(obj, "focussed", ref.focussed_);

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

	// Identity + basic state
	ref.name_ = JsonConverter::getString(jsonObj, "name");

	/*
	 * Character trait varaibles used to assist in choices
	 */
	ref.aggression_ = JsonConverter::getInt(jsonObj, "aggression");
	ref.combat_closeness_ = JsonConverter::getInt(jsonObj, "combatCloseness");
	ref.combat_casting_ = JsonConverter::getInt(jsonObj, "combatCasting");
	ref.focussed_ = JsonConverter::getInt(jsonObj, "focussed");

	return ref;
}

} // namespace rm::serial