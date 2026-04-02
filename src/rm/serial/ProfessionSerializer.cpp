#include <ProfessionSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value ProfessionSerializer::serializeObject(const ProfessionData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "book", ref.book().value()->id());
	JsonConverter::setString(obj, "spellUserType", SpellUserType::toString(ref.spellUserType()));
	JsonConverter::setDataSet(obj, "allowedRaces", ref.allowedRaces());
	JsonConverter::setEnumSet(obj, "realms", ref.realms());
	JsonConverter::setEnumSet(obj, "stats", ref.stats());
	JsonConverter::setDataChoiceSet(obj, "baseSpellListChoices", ref.baseSpellListChoices());
	JsonConverter::setSkillPrimitiveMap(obj, "skillBonuses", ref.skillBonuses());
	JsonConverter::setSkillEnumMap(obj, "skillDevelopmentTypes", ref.skillDevelopmentTypes());
	JsonConverter::setSkillChoiceEnumMap(obj, "skillDevelopmentTypeChoices", ref.skillDevelopmentTypeChoices());
	JsonConverter::setDataChoiceEnumMap(obj, "skillSubcategoryDevelopmentTypeChoices", ref.skillSubcategoryDevelopmentTypeChoices());
	JsonConverter::setDataPrimitiveMap(obj, "skillCategoryProfessionBonuses", ref.skillCategoryProfessionBonuses());
	JsonConverter::setDataPrimitiveMap(obj, "skillCategorySpecialBonuses", ref.skillCategorySpecialBonuses());
	JsonConverter::setDataEnumMap(obj, "skillCategorySkillDevelopmentTypes", ref.skillCategorySkillDevelopmentTypes());
	JsonConverter::setDataChoiceEnumMap(obj, "skillCategorySkillDevelopmentTypeChoices", ref.skillCategorySkillDevelopmentTypeChoices());

	// Skill category development costs
	{
		json::array arr;
		std::map<std::string, const SkillCategoryData*> sorted_categories{};
		for (const auto& category : ref.skillCategoriesWithCost()) {
			sorted_categories.emplace(category->id(), category);
		}
		for (const auto& pair : sorted_categories) {
			json::object category_cost_obj;
			category_cost_obj["category"] = pair.first;
			category_cost_obj["cost"] = ref.skillCategoryDevelopmentCost(*pair.second).toString();
			arr.emplace_back(std::move(category_cost_obj));
		}
		obj["skillCategoryCosts"] = std::move(arr);
	}

	JsonConverter::setDataPrimitiveMap(obj, "skillGroupProfessionBonuses", ref.skillGroupProfessionBonuses());
	JsonConverter::setDataPrimitiveMap(obj, "skillGroupSpecialBonuses", ref.skillGroupSpecialBonuses());
	JsonConverter::setDataEnumMap(obj, "skillGroupSkillDevelopmentTypes", ref.skillGroupSkillDevelopmentTypes());
	JsonConverter::setDataChoiceEnumMap(obj, "skillGroupSkillDevelopmentTypeChoices", ref.skillGroupSkillDevelopmentTypeChoices());

	return obj;
}

const ProfessionData& ProfessionSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	ProfessionData& ref = manager_.get<ProfessionData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));

	// Get the book reference
	std::string book_id = JsonConverter::getString(jsonObj, "book");
	ref.setBook(manager_.get<BookData>(book_id));

	ref.setSpellUserType(SpellUserType::fromString(JsonConverter::getString(jsonObj, "spellUserType")).value());
	ref.setAllowedRaces(JsonConverter::getDataSet<RaceData>(jsonObj, "allowedRaces", manager_));
	ref.setRealms(JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "realms"));
	ref.setStats(JsonConverter::getEnumVector<StatType::Type>(jsonObj, "stats"));
	ref.setBaseSpellListChoices(JsonConverter::getDataChoiceSet<SpellListData>(jsonObj, "baseSpellListChoices", manager_));
	ref.setSkillBonuses(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillBonuses", manager_));
	ref.setSkillDevelopmentTypes(JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "skillDevelopmentTypes", manager_));
	ref.setSkillDevelopmentTypeChoices(JsonConverter::getSkillChoiceEnumMap<SkillDevelopmentType::Type>(jsonObj, "skillDevelopmentTypeChoices", manager_));
	ref.setSkillSubcategoryDevelopmentTypeChoices(JsonConverter::getDataChoiceEnumMap<SkillData, SkillDevelopmentType::Type>(jsonObj, "skillSubcategoryDevelopmentTypeChoices", manager_));
	ref.setSkillCategoryProfessionBonuses(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skillCategoryProfessionBonuses", manager_));
	ref.setSkillCategorySpecialBonuses(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skillCategorySpecialBonuses", manager_));
	ref.setSkillCategorySkillDevelopmentTypes(JsonConverter::getDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(jsonObj, "skillCategorySkillDevelopmentTypes", manager_));
	ref.setSkillCategorySkillDevelopmentTypeChoices(JsonConverter::getDataChoiceEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(jsonObj, "skillCategorySkillDevelopmentTypeChoices", manager_));

	// Skill category development costs
	json::array skillCategoryDevelopmentCostArr = JsonConverter::getJsonArray(jsonObj, "skillCategoryCosts");
	for (const auto& item : skillCategoryDevelopmentCostArr) {
		if (item.is_object()) {
			json::object obj = item.as_object();
			std::string category_id = JsonConverter::getString(obj, "category");
			std::string cost = JsonConverter::getString(obj, "cost");
			ref.addSkillCategoryDevelopmentCost(manager().get<SkillCategoryData>(category_id), std::move(rm::game::character::SkillDevelopmentCost(cost)));
		}
	}

	ref.setSkillGroupProfessionBonuses(JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "skillGroupProfessionBonuses", manager_));
	ref.setSkillGroupSpecialBonuses(JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "skillGroupSpecialBonuses", manager_));
	ref.setSkillGroupSkillDevelopmentTypes(JsonConverter::getDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(jsonObj, "skillGroupSkillDevelopmentTypes", manager_));
	ref.setSkillGroupSkillDevelopmentTypeChoices(JsonConverter::getDataChoiceEnumMap<SkillGroupData, SkillDevelopmentType::Type>(jsonObj, "skillGroupSkillDevelopmentTypeChoices", manager_));

	return ref;
}

} // namespace rm::serial