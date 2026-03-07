#include <SpellListSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value SpellListSerializer::serializeObject(const SpellListData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "book", ref.book().value()->id());
	JsonConverter::setString(obj, "type", SpellListType::toString(ref.type()));
	JsonConverter::setBool(obj, "evil", ref.isEvil());
	JsonConverter::setBool(obj, "summoning", ref.isSummoning());
	JsonConverter::setEnumSet(obj, "realms", ref.realms());

	return obj;
}

const SpellListData& SpellListSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	SpellListData& ref = manager_.get<SpellListData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setBook(manager_.get<BookData>(JsonConverter::getString(jsonObj, "book")));
	std::optional<SpellListType::Type> type = SpellListType::fromString(JsonConverter::getString(jsonObj, "type"));
	if (type)
		ref.setType(type.value());
	ref.setIsEvil(JsonConverter::getBool(jsonObj, "evil", false));
	ref.setIsSummoning(JsonConverter::getBool(jsonObj, "summoning", false));
	ref.setRealms(JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "realms"));

	return ref;
}

} // namespace rm::rule::serial