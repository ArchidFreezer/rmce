#include <WeaponTypeSerializer.h>
#include <NumberMatcherFactory.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value WeaponTypeSerializer::serializeObject(const WeaponTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "notes", ref.notes());
	JsonConverter::setString(obj, "skill", ref.skill().id());
	JsonConverter::setString(obj, "book", ref.book().id());
	JsonConverter::setString(obj, "attack-table", ref.attackTable().id());
	JsonConverter::setInt(obj, "fumble", ref.fumble());
	JsonConverter::setInt(obj, "breakage", ref.breakage());
	JsonConverter::setInt(obj, "min-length", ref.minLength());
	JsonConverter::setInt(obj, "max-length", ref.maxLength());
	JsonConverter::setInt(obj, "min-strength", ref.minStrength());
	JsonConverter::setInt(obj, "max-strength", ref.maxStrength());
	JsonConverter::setInt(obj, "min-weight", ref.minWeight());
	JsonConverter::setInt(obj, "max-weight", ref.maxWeight());
	JsonConverter::setBool(obj, "wooden-haft", ref.woodenHaft());

	// Criticals
	json::array criticals_arr;
	for (const auto& critical_type : ref.criticals()) {
		json::object criticalObj;
		JsonConverter::setString(criticalObj, "critical", CriticalType::toString(critical_type));
		JsonConverter::setInt(criticalObj, "modifier", ref.criticalModifier(critical_type));
		criticals_arr.push_back(criticalObj);
	}
	if (!criticals_arr.empty())
		obj["criticals"] = criticals_arr;

	// Ranges
	// The game data stores the ranges as pointers, which is essentially a random sort. We want them ordered so derefence the pointers and place them into a set which will order them as we want
	std::set<archid::NumberRange<int>> ranges{};
	for (auto& range : ref.ranges()) {
		ranges.insert(*range);
	}
	json::array ranges_arr;
	for (const auto& range : ranges) {
		json::object rangeObj;
		JsonConverter::setInt(rangeObj, "min", range.min());
		JsonConverter::setInt(rangeObj, "max", range.max());
		JsonConverter::setInt(rangeObj, "modifier", ref.rangeModifier(range.min()));
		ranges_arr.push_back(rangeObj);
	}
	if (!ranges_arr.empty())
		obj["ranges"] = ranges_arr;

	return obj;
}

const WeaponTypeData& WeaponTypeSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	WeaponTypeData& ref = manager_.get<WeaponTypeData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setNotes(JsonConverter::getString(jsonObj, "notes"));

	std::string skill_id = JsonConverter::getString(jsonObj, "skill");
	ref.setSkill(manager_.get<SkillData>(skill_id));

	std::string book_id = JsonConverter::getString(jsonObj, "book");
	ref.setBook(manager_.get<BookData>(book_id));

	std::string attack_table_id = JsonConverter::getString(jsonObj, "attack-table");
	ref.setAttackTable(manager_.get<rm::rule::table::AttackTable>(attack_table_id));

	ref.setFumble(JsonConverter::getInt(jsonObj, "fumble", 0));
	ref.setBreakage(JsonConverter::getInt(jsonObj, "breakage", 0));
	ref.setMinLength(JsonConverter::getInt(jsonObj, "min-length", 0));
	ref.setMaxLength(JsonConverter::getInt(jsonObj, "max-length", 0));
	ref.setMinStrength(JsonConverter::getInt(jsonObj, "min-strength", 0));
	ref.setMaxStrength(JsonConverter::getInt(jsonObj, "max-strength", 0));
	ref.setMinWeight(JsonConverter::getInt(jsonObj, "min-weight", 0));
	ref.setMaxWeight(JsonConverter::getInt(jsonObj, "max-weight", 0));
	ref.setWoodenHaft(JsonConverter::getBool(jsonObj, "wooden-haft", false));

	// Criticals
	auto it = jsonObj.find("criticals");
	if (it != jsonObj.end() && it->value().is_array()) {
		json::array criticals_arr = it->value().as_array();
		for (const auto& critical_val : criticals_arr) {
			if (!critical_val.is_object())
				continue;
			json::object criticalObj = critical_val.as_object();
			std::string critical = JsonConverter::getString(criticalObj, "critical");
			int modifier = JsonConverter::getInt(criticalObj, "modifier");
			ref.addCriticalModifier(CriticalType::fromString(critical).value(), modifier);
		}
	}
	// Get a factory for the matchers
	rm::rule::table::NumberMatcherFactory matchers;

	// Ranges
	auto it_ranges = jsonObj.find("ranges");
	if (it_ranges != jsonObj.end() && it_ranges->value().is_array()) {
		json::array ranges_arr = it_ranges->value().as_array();
		for (const auto& range_val : ranges_arr) {
			if (!range_val.is_object())
				continue;
			json::object rangeObj = range_val.as_object();
			int min = JsonConverter::getInt(rangeObj, "min");
			int max = JsonConverter::getInt(rangeObj, "max");
			int modifier = JsonConverter::getInt(rangeObj, "modifier");
			ref.addRange(*matchers.matcher(min, max), modifier);
		}
	}

	return ref;
}

} // namespace rm::rule::serial