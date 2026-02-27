#include <TreasureCodeDatafileParserJson.h>

namespace rm::rule::parser {


	void TreasureCodeDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		TreasureCodeData& game_data = factory().get<TreasureCodeData>(id);

		datum.put("id", game_data.id());
		datum.put("items-value-type", rule::enums::TreasureValueType::toString(game_data.itemsValueType()));
		datum.put("wealth-value-type", rule::enums::TreasureValueType::toString(game_data.wealthValueType()));
	}

	void TreasureCodeDatafileParserJson::parse() {
		std::cout << "Loading TreasureCode data ... ";

		// Get the lists to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string id = v.second.get<std::string>("id");

			TreasureCodeData& ref = factory().get<TreasureCodeData>(id);

			rule::enums::TreasureValueType::Type items_value_type{};
			fromString(v.second.get<std::string>("items-value-type"), items_value_type);
			ref.setItemsValueType(items_value_type);

			rule::enums::TreasureValueType::Type wealth_value_type{};
			fromString(v.second.get<std::string>("wealth-value-type"), wealth_value_type);
			ref.setWealthValueType(wealth_value_type);

			std::cout << "\tTreasureCode name: " << ref.id() << std::endl;

		}
	}

} // namespace rm::rule::parser