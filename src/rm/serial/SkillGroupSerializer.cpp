#include <SkillGroupSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value SkillGroupSerializer::serializeObject(const SkillGroupData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());

	return obj;
}

const SkillGroupData& SkillGroupSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	SkillGroupData& ref = manager_.get<SkillGroupData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));

	return ref;
}

} // namespace rm::serial