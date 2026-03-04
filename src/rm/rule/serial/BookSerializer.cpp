#include <BookSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value BookSerializer::serializeObject(const BookData& book) const {
	json::object obj;

	JsonConverter::setString(obj, "id", book.id());
	JsonConverter::setString(obj, "code", book.code());
	JsonConverter::setString(obj, "name", book.name());
	JsonConverter::setString(obj, "abbreviation", book.abbreviation());
	JsonConverter::setString(obj, "isbn", book.isbn());

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

} // namespace rm::rule::serial