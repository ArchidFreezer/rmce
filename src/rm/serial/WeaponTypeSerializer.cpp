#include <WeaponTypeSerializer.h>
#include <NumberMatcherFactory.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value WeaponTypeSerializer::serializeObject(const WeaponTypeData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "notes", ref.notes());
	JsonConverter::setString(obj, "book", ref.book().id());
	JsonConverter::setString(obj, "attackTable", ref.attackTable().id());
	JsonConverter::setInt(obj, "fumble", ref.fumble());
	JsonConverter::setInt(obj, "breakage", ref.breakage());
	JsonConverter::setInt(obj, "minLength", ref.minLength());
	JsonConverter::setInt(obj, "maxLength", ref.maxLength());
	JsonConverter::setInt(obj, "minStrength", ref.minStrength());
	JsonConverter::setInt(obj, "maxStrength", ref.maxStrength());
	JsonConverter::setInt(obj, "minWeight", ref.minWeight());
	JsonConverter::setInt(obj, "maxWeight", ref.maxWeight());
	JsonConverter::setBool(obj, "woodenHaft", ref.woodenHaft());
	JsonConverter::setDataSet(obj, "skills", ref.skills());

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

	std::set<const SkillData*> skills = JsonConverter::getDataSet<SkillData>(jsonObj, "skills", manager_);
	ref.setSkills(std::move(skills));

	std::string book_id = JsonConverter::getString(jsonObj, "book");
	ref.setBook(manager_.get<BookData>(book_id));

	std::string attack_table_id = JsonConverter::getString(jsonObj, "attackTable");
	ref.setAttackTable(manager_.get<rm::rule::table::AttackTable>(attack_table_id));

	ref.setFumble(JsonConverter::getInt(jsonObj, "fumble", 0));
	ref.setBreakage(JsonConverter::getInt(jsonObj, "breakage", 0));
	ref.setMinLength(JsonConverter::getInt(jsonObj, "minLength", 0));
	ref.setMaxLength(JsonConverter::getInt(jsonObj, "maxLength", 0));
	ref.setMinStrength(JsonConverter::getInt(jsonObj, "minStrength", 0));
	ref.setMaxStrength(JsonConverter::getInt(jsonObj, "maxStrength", 0));
	ref.setMinWeight(JsonConverter::getInt(jsonObj, "minWeight", 0));
	ref.setMaxWeight(JsonConverter::getInt(jsonObj, "maxWeight", 0));
	ref.setWoodenHaft(JsonConverter::getBool(jsonObj, "woodenHaft", false));

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

} // namespace rm::serial