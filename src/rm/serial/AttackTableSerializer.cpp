#include <AttackTableSerializer.h>
#include <JsonConverter.h>
#include <NumberMatcherFactory.h>

namespace rm::serial {

json::value AttackTableSerializer::serializeObject(const rm::rule::table::AttackTable& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());

	int max_row{0};

	{
		// The game data stores the ranges as pointers, which is essentially a random sort. We want them ordered so derefence
		// the pointers and place them into a set which will order them as we want
		std::set<archid::NumberRange<int>> ranges{};
		for (auto& m : ref.modified()) {
			ranges.insert(*m.first);
			max_row = std::max(max_row, m.first->max());
		}
		JsonConverter::setInt(obj, "maxRow", max_row);

		// Now we have a soted set of the ranges, we can iterate through them and serialize the rows in order
		json::array modified_rows{};
		for (auto& range : ranges) {
			int min = range.min();

			json::object row_obj{};
			JsonConverter::setInt(row_obj, "min", min);
			JsonConverter::setInt(row_obj, "max", range.max());
			JsonConverter::setString(row_obj, "at1", ref.cell(ArmourType::kAT1, min));
			JsonConverter::setString(row_obj, "at2", ref.cell(ArmourType::kAT2, min));
			JsonConverter::setString(row_obj, "at3", ref.cell(ArmourType::kAT3, min));
			JsonConverter::setString(row_obj, "at4", ref.cell(ArmourType::kAT4, min));
			JsonConverter::setString(row_obj, "at5", ref.cell(ArmourType::kAT5, min));
			JsonConverter::setString(row_obj, "at6", ref.cell(ArmourType::kAT6, min));
			JsonConverter::setString(row_obj, "at7", ref.cell(ArmourType::kAT7, min));
			JsonConverter::setString(row_obj, "at8", ref.cell(ArmourType::kAT8, min));
			JsonConverter::setString(row_obj, "at9", ref.cell(ArmourType::kAT9, min));
			JsonConverter::setString(row_obj, "at10", ref.cell(ArmourType::kAT10, min));
			JsonConverter::setString(row_obj, "at11", ref.cell(ArmourType::kAT11, min));
			JsonConverter::setString(row_obj, "at12", ref.cell(ArmourType::kAT12, min));
			JsonConverter::setString(row_obj, "at13", ref.cell(ArmourType::kAT13, min));
			JsonConverter::setString(row_obj, "at14", ref.cell(ArmourType::kAT14, min));
			JsonConverter::setString(row_obj, "at15", ref.cell(ArmourType::kAT15, min));
			JsonConverter::setString(row_obj, "at16", ref.cell(ArmourType::kAT16, min));
			JsonConverter::setString(row_obj, "at17", ref.cell(ArmourType::kAT17, min));
			JsonConverter::setString(row_obj, "at18", ref.cell(ArmourType::kAT18, min));
			JsonConverter::setString(row_obj, "at19", ref.cell(ArmourType::kAT19, min));
			JsonConverter::setString(row_obj, "at20", ref.cell(ArmourType::kAT20, min));
			modified_rows.emplace_back(row_obj);
		}
		obj["modifiedRows"] = modified_rows;
	}

	{
		std::set<archid::NumberRange<int>> ranges{};
		for (auto& m : ref.unmodified()) {
			ranges.insert(*m.first);
			max_row = std::max(max_row, m.first->max());
		}

		// Now we have a soted set of the ranges, we can iterate through them and serialize the rows in order
		json::array unmodified_rows{};
		for (auto& range : ranges) {
			int min = range.min();

			json::object row_obj{};
			JsonConverter::setInt(row_obj, "min", min);
			JsonConverter::setInt(row_obj, "max", range.max());
			JsonConverter::setString(row_obj, "at1", ref.cell(ArmourType::kAT1, min, min));
			JsonConverter::setString(row_obj, "at2", ref.cell(ArmourType::kAT2, min, min));
			JsonConverter::setString(row_obj, "at3", ref.cell(ArmourType::kAT3, min, min));
			JsonConverter::setString(row_obj, "at4", ref.cell(ArmourType::kAT4, min, min));
			JsonConverter::setString(row_obj, "at5", ref.cell(ArmourType::kAT5, min, min));
			JsonConverter::setString(row_obj, "at6", ref.cell(ArmourType::kAT6, min, min));
			JsonConverter::setString(row_obj, "at7", ref.cell(ArmourType::kAT7, min, min));
			JsonConverter::setString(row_obj, "at8", ref.cell(ArmourType::kAT8, min, min));
			JsonConverter::setString(row_obj, "at9", ref.cell(ArmourType::kAT9, min, min));
			JsonConverter::setString(row_obj, "at10", ref.cell(ArmourType::kAT10, min, min));
			JsonConverter::setString(row_obj, "at11", ref.cell(ArmourType::kAT11, min, min));
			JsonConverter::setString(row_obj, "at12", ref.cell(ArmourType::kAT12, min, min));
			JsonConverter::setString(row_obj, "at13", ref.cell(ArmourType::kAT13, min, min));
			JsonConverter::setString(row_obj, "at14", ref.cell(ArmourType::kAT14, min, min));
			JsonConverter::setString(row_obj, "at15", ref.cell(ArmourType::kAT15, min, min));
			JsonConverter::setString(row_obj, "at16", ref.cell(ArmourType::kAT16, min, min));
			JsonConverter::setString(row_obj, "at17", ref.cell(ArmourType::kAT17, min, min));
			JsonConverter::setString(row_obj, "at18", ref.cell(ArmourType::kAT18, min, min));
			JsonConverter::setString(row_obj, "at19", ref.cell(ArmourType::kAT19, min, min));
			JsonConverter::setString(row_obj, "at20", ref.cell(ArmourType::kAT20, min, min));
			unmodified_rows.emplace_back(row_obj);
		}
		if (unmodified_rows.size())	obj["unmodifiedRows"] = unmodified_rows;
	}

	return obj;
}

const rm::rule::table::AttackTable& AttackTableSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	rm::rule::table::AttackTable& ref = manager_.get<rm::rule::table::AttackTable>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setMaxRow(JsonConverter::getInt(jsonObj, "maxRow"));

	// Get a factory for the matchers
	rm::rule::table::NumberMatcherFactory matchers;

	{
		// Deserialize the modified rows
		if (jsonObj.contains("modifiedRows")) {
			for (const auto& row : JsonConverter::getJsonArray(jsonObj, "modifiedRows")) {
				int min = JsonConverter::getInt(row.as_object(), "min");
				int max = JsonConverter::getInt(row.as_object(), "max");

				rm::rule::table::TableRow<std::string> table_row{};
				for (int i{1}; i < 21; i++) {
					table_row.addCell(JsonConverter::getString(row.as_object(), "at" + std::to_string(i)));
				}
				ref.addRow(matchers.matcher(min, max), table_row);
			}
		}
	}

	{
		// Deserialize the unmodified rows
		if (jsonObj.contains("unmodifiedRows")) {
			for (const auto& row : JsonConverter::getJsonArray(jsonObj, "unmodifiedRows")) {
				int min = JsonConverter::getInt(row.as_object(), "min");
				int max = JsonConverter::getInt(row.as_object(), "max");
				rm::rule::table::TableRow<std::string> table_row{};
				for (int i{1}; i < 21; i++) {
					table_row.addCell(JsonConverter::getString(row.as_object(), "at" + std::to_string(i)));
				}
				ref.addUnmodifiedRow(matchers.matcher(min, max), table_row);
			}
		}
	}

	return ref;
}

} // namespace rm::serial