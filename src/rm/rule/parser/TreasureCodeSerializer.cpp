#include <TreasureCodeSerializer.h>

namespace rm::rule::parser {

void TreasureCodeSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading TreasureCode data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string id = v.second.get<std::string>("id");

		TreasureCodeData& ref = manager().get<TreasureCodeData>(id);

		TreasureValueType::Type items_value_type{};
		fromString(v.second.get<std::string>("items-value-type"), items_value_type);
		ref.setItemsValueType(items_value_type);

		TreasureValueType::Type wealth_value_type{};
		fromString(v.second.get<std::string>("wealth-value-type"), wealth_value_type);
		ref.setWealthValueType(wealth_value_type);

		std::cout << "\tTreasureCode name: " << ref.id() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void TreasureCodeSerializer::populateDatum(std::string key, pt::ptree& datum) {
	TreasureCodeData& game_data = manager().get<TreasureCodeData>(key);
	datum.put("id", game_data.id());
	datum.put("items-value-type", TreasureValueType::toString(game_data.itemsValueType()));
	datum.put("wealth-value-type", TreasureValueType::toString(game_data.wealthValueType()));
}

std::string TreasureCodeSerializer::rootNode() const {
	return "treasure-codes";
}

} // namespace rm::rule::parser