#include <SkillProgressionTypeSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value SkillProgressionTypeSerializer::serializeObject(const SkillProgressionTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setFloat(obj, "zero", ref.zero());
	JsonConverter::setFloat(obj, "ten", ref.ten());
	JsonConverter::setFloat(obj, "twenty", ref.twenty());
	JsonConverter::setFloat(obj, "thirty", ref.thirty());
	JsonConverter::setFloat(obj, "remaining", ref.remaining());

	return obj;
}

const SkillProgressionTypeData& SkillProgressionTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	SkillProgressionTypeData& ref = manager_.get<SkillProgressionTypeData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setZero(JsonConverter::getFloat(jsonObj, "zero"));
	ref.setTen(JsonConverter::getFloat(jsonObj, "ten"));
	ref.setTwenty(JsonConverter::getFloat(jsonObj, "twenty"));
	ref.setThirty(JsonConverter::getFloat(jsonObj, "thirty"));
	ref.setRemaining(JsonConverter::getFloat(jsonObj, "remaining"));

	return ref;
}

} // namespace rm::serial