#include <BookSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value BookSerializer::serializeObject(const BookData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "code", ref.code());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "abbreviation", ref.abbreviation());
	JsonConverter::setString(obj, "isbn", ref.isbn());

	return obj;
}

const BookData& BookSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	BookData& ref = manager_.get<BookData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setCode(JsonConverter::getString(jsonObj, "code"));
	ref.setAbbreviation(JsonConverter::getString(jsonObj, "abbreviation"));
	ref.setIsbn(JsonConverter::getString(jsonObj, "isbn"));
	return ref;
}

} // namespace rm::serial