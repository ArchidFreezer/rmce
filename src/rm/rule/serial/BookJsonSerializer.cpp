#include <BookJsonSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value BookJsonSerializer::serializeObject(const BookData& book) {
	json::object obj;

	JsonConverter::setString(obj, "id", book.id());
	JsonConverter::setString(obj, "name", book.name());
	JsonConverter::setString(obj, "code", book.code());
	JsonConverter::setString(obj, "abbreviation", book.abbreviation());

	return obj;
}

const BookData& BookJsonSerializer::deserializeObject(json::object& jsonObj) {
	std::string id = JsonConverter::getString(jsonObj, "id");
	BookData& ref = manager_.get<BookData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setCode(JsonConverter::getString(jsonObj, "code"));
	ref.setAbbreviation(JsonConverter::getString(jsonObj, "abbreviation"));
	return ref;
}

} // namespace rm::rule::serial