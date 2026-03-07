#include <SpecialAttackTableSerializer.h>
#include <JsonConverter.h>
#include <NumberMatcherFactory.h>

namespace rm::rule::serial {

json::value SpecialAttackTableSerializer::serializeObject(const rm::rule::table::SpecialAttackTable& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setInt(obj, "small", ref.limit(AttackSizeType::kSmall));
	JsonConverter::setInt(obj, "medium", ref.limit(AttackSizeType::kMedium));
	JsonConverter::setInt(obj, "large", ref.limit(AttackSizeType::kLarge));
	JsonConverter::setInt(obj, "huge", ref.limit(AttackSizeType::kHuge));


	int max_row{0};

	{
		// The game data stores the ranges as pointers, which is essentially a random sort. We want them ordered so derefence
		// the pointers and place them into a set which will order them as we want
		std::set<archid::NumberRange<int>> ranges{};
		for (auto& m : ref.modified()) {
			ranges.insert(*m.first);
			max_row = std::max(max_row, m.first->max());
		}
		JsonConverter::setInt(obj, "max-row", max_row);

		// Now we have a soted set of the ranges, we can iterate through them and serialize the rows in order
		json::array modified_rows{};
		for (auto& range : ranges) {
			int min = range.min();

			json::object row_obj{};
			JsonConverter::setInt(row_obj, "min", min);
			JsonConverter::setInt(row_obj, "max", range.max());
			JsonConverter::setString(row_obj, "at1", ref.cell(ArmourType::kAT1, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at2", ref.cell(ArmourType::kAT2, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at3", ref.cell(ArmourType::kAT3, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at4", ref.cell(ArmourType::kAT4, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at5", ref.cell(ArmourType::kAT5, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at6", ref.cell(ArmourType::kAT6, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at7", ref.cell(ArmourType::kAT7, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at8", ref.cell(ArmourType::kAT8, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at9", ref.cell(ArmourType::kAT9, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at10", ref.cell(ArmourType::kAT10, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at11", ref.cell(ArmourType::kAT11, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at12", ref.cell(ArmourType::kAT12, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at13", ref.cell(ArmourType::kAT13, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at14", ref.cell(ArmourType::kAT14, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at15", ref.cell(ArmourType::kAT15, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at16", ref.cell(ArmourType::kAT16, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at17", ref.cell(ArmourType::kAT17, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at18", ref.cell(ArmourType::kAT18, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at19", ref.cell(ArmourType::kAT19, AttackSizeType::kHuge, min));
			JsonConverter::setString(row_obj, "at20", ref.cell(ArmourType::kAT20, AttackSizeType::kHuge, min));
			modified_rows.emplace_back(row_obj);
		}
		obj["modified-rows"] = modified_rows;
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
			JsonConverter::setString(row_obj, "at1", ref.cell(ArmourType::kAT1, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at2", ref.cell(ArmourType::kAT2, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at3", ref.cell(ArmourType::kAT3, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at4", ref.cell(ArmourType::kAT4, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at5", ref.cell(ArmourType::kAT5, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at6", ref.cell(ArmourType::kAT6, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at7", ref.cell(ArmourType::kAT7, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at8", ref.cell(ArmourType::kAT8, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at9", ref.cell(ArmourType::kAT9, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at10", ref.cell(ArmourType::kAT10, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at11", ref.cell(ArmourType::kAT11, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at12", ref.cell(ArmourType::kAT12, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at13", ref.cell(ArmourType::kAT13, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at14", ref.cell(ArmourType::kAT14, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at15", ref.cell(ArmourType::kAT15, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at16", ref.cell(ArmourType::kAT16, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at17", ref.cell(ArmourType::kAT17, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at18", ref.cell(ArmourType::kAT18, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at19", ref.cell(ArmourType::kAT19, AttackSizeType::kHuge, min, min));
			JsonConverter::setString(row_obj, "at20", ref.cell(ArmourType::kAT20, AttackSizeType::kHuge, min, min));
			unmodified_rows.emplace_back(row_obj);
		}
		if (unmodified_rows.size())
			obj["unmodified-rows"] = unmodified_rows;
	}

	return obj;
}

const rm::rule::table::SpecialAttackTable& SpecialAttackTableSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	rm::rule::table::SpecialAttackTable& ref = manager_.get<rm::rule::table::SpecialAttackTable>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setLimit(AttackSizeType::kSmall, JsonConverter::getInt(jsonObj, "small"));
	ref.setLimit(AttackSizeType::kMedium, JsonConverter::getInt(jsonObj, "medium"));
	ref.setLimit(AttackSizeType::kLarge, JsonConverter::getInt(jsonObj, "large"));
	ref.setLimit(AttackSizeType::kHuge, JsonConverter::getInt(jsonObj, "huge"));

	// Get a factory for the matchers
	rm::rule::table::NumberMatcherFactory matchers;

	{
		// Deserialize the modified rows
		if (jsonObj.contains("modified-rows")) {
			for (const auto& row : JsonConverter::getJsonArray(jsonObj, "modified-rows")) {
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
		if (jsonObj.contains("unmodified-rows")) {
			for (const auto& row : JsonConverter::getJsonArray(jsonObj, "unmodified-rows")) {
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

} // namespace rm::rule::serial