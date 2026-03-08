#include <TreasureCodeSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value TreasureCodeSerializer::serializeObject(const TreasureCodeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "items-value-type", toString(ref.itemsValueType()));
	JsonConverter::setString(obj, "wealth-value-type", toString(ref.wealthValueType()));

	return obj;
}

const TreasureCodeData& TreasureCodeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	TreasureCodeData& ref = manager_.get<TreasureCodeData>(id);

	TreasureValueType::Type items_value_type{};
	fromString(JsonConverter::getString(jsonObj, "items-value-type"), items_value_type);
	ref.setItemsValueType(items_value_type);

	TreasureValueType::Type wealth_value_type{};
	fromString(JsonConverter::getString(jsonObj, "wealth-value-type"), wealth_value_type);
	ref.setWealthValueType(wealth_value_type);
	

	return ref;
}

} // namespace rm::rule::serial