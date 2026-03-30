#include <LanguageCategorySerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value LanguageCategorySerializer::serializeObject(const LanguageCategoryData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());

	return obj;
}

const LanguageCategoryData& LanguageCategorySerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	LanguageCategoryData& ref = manager_.get<LanguageCategoryData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));

	return ref;
}

} // namespace rm::serial