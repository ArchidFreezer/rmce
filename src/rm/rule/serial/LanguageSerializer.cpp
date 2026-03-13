#include <LanguageSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value LanguageSerializer::serializeObject(const LanguageData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "category", ref.category());
	// We only serialize the base language if it is different from the language name, as this is the most common case and avoids unnecessary duplication in the JSON data, which can save space and improve readability.
	if (ref.baseLanguage() != ref.name())
		JsonConverter::setString(obj, "baseLanguage", ref.baseLanguage());
	JsonConverter::setBool(obj, "isSpoken", ref.isSpoken());
	JsonConverter::setBool(obj, "isWritten", ref.isWritten());
	JsonConverter::setBool(obj, "isSomantic", ref.isSomantic());

	return obj;
}

const LanguageData& LanguageSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	LanguageData& ref = manager_.get<LanguageData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	// We default the base language to the language itself if it is not provided, as this is the most common case and ensures that the field is always populated with a valid value, which simplifies logic elsewhere in the code that relies on
	// this field being set.
	ref.setBaseLanguage(JsonConverter::getString(jsonObj, "baseLanguage", ref.name()));
	ref.setCategory(JsonConverter::getString(jsonObj, "category", ""));
	ref.setIsSpoken(JsonConverter::getBool(jsonObj, "isSpoken", false));
	ref.setIsWritten(JsonConverter::getBool(jsonObj, "isWritten", false));
	ref.setIsSomantic(JsonConverter::getBool(jsonObj, "isSomantic", false));

	return ref;
}

} // namespace rm::rule::serial