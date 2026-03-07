#include <ProfessionSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value ProfessionSerializer::serializeObject(const ProfessionData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	JsonConverter::setString(obj, "book", ref.book().value()->id());
	JsonConverter::setString(obj, "spell-user-type", SpellUserType::toString(ref.spellUserType()));
	JsonConverter::setEnumSet(obj, "realms", ref.realms());
	JsonConverter::setEnumSet(obj, "stats", ref.stats());
	JsonConverter::setDataChoiceSet(obj, "base-spell-list-choices", ref.baseSpellListChoices());
	JsonConverter::setSkillPrimitiveMap(obj, "skill-bonuses", ref.skillBonuses());
	JsonConverter::setSkillEnumMap(obj, "skill-development-types", ref.skillDevelopmentTypes());
	JsonConverter::setSkillChoiceEnumMap(obj, "skill-development-type-choices", ref.skillDevelopmentTypeChoices());
	JsonConverter::setDataChoiceEnumMap(obj, "skill-subcategory-development-type-choices", ref.skillSubcategoryDevelopmentTypeChoices());
	JsonConverter::setDataPrimitiveMap(obj, "skill-category-profession-bonuses", ref.skillCategoryProfessionBonuses());
	JsonConverter::setDataPrimitiveMap(obj, "skill-category-special-bonuses", ref.skillCategorySpecialBonuses());
	JsonConverter::setDataEnumMap(obj, "skill-category-skill-development-types", ref.skillCategorySkillDevelopmentTypes());
	JsonConverter::setDataChoiceEnumMap(obj, "skill-category-skill-development-type-choices", ref.skillCategorySkillDevelopmentTypeChoices());

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
		obj["skill-category-costs"] = std::move(arr);
	}

	JsonConverter::setDataPrimitiveMap(obj, "skill-group-profession-bonuses", ref.skillGroupProfessionBonuses());
	JsonConverter::setDataPrimitiveMap(obj, "skill-group-special-bonuses", ref.skillGroupSpecialBonuses());
	JsonConverter::setDataEnumMap(obj, "skill-group-skill-development-types", ref.skillGroupSkillDevelopmentTypes());
	JsonConverter::setDataChoiceEnumMap(obj, "skill-group-skill-development-type-choices", ref.skillGroupSkillDevelopmentTypeChoices());

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

	ref.setSpellUserType(SpellUserType::fromString(JsonConverter::getString(jsonObj, "spell-user-type")).value());
	ref.setRealms(JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "realms"));
	ref.setStats(JsonConverter::getEnumVector<StatType::Type>(jsonObj, "stats"));
	ref.setBaseSpellListChoices(JsonConverter::getDataChoiceSet<SpellListData>(jsonObj, "base-spell-list-choices", manager_));
	ref.setSkillBonuses(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skill-bonuses", manager_));
	ref.setSkillDevelopmentTypes(JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "skill-development-types", manager_));
	ref.setSkillDevelopmentTypeChoices(JsonConverter::getSkillChoiceEnumMap<SkillDevelopmentType::Type>(jsonObj, "skill-development-type-choices", manager_));
	ref.setSkillSubcategoryDevelopmentTypeChoices(JsonConverter::getDataChoiceEnumMap<SkillData, SkillDevelopmentType::Type>(jsonObj, "skill-subcategory-development-type-choices", manager_));
	ref.setSkillCategoryProfessionBonuses(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skill-category-profession-bonuses", manager_));
	ref.setSkillCategorySpecialBonuses(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "skill-category-special-bonuses", manager_));
	ref.setSkillCategorySkillDevelopmentTypes(JsonConverter::getDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(jsonObj, "skill-category-skill-development-types", manager_));
	ref.setSkillCategorySkillDevelopmentTypeChoices(JsonConverter::getDataChoiceEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(jsonObj, "skill-category-skill-development-type-choices", manager_));

	// Skill category development costs
	json::array skillCategoryDevelopmentCostArr = JsonConverter::getJsonArray(jsonObj, "skill-category-costs");
	for (const auto& item : skillCategoryDevelopmentCostArr) {
		if (item.is_object()) {
			json::object obj = item.as_object();
			std::string category_id = JsonConverter::getString(obj, "category");
			std::string cost = JsonConverter::getString(obj, "cost");
			ref.addSkillCategoryDevelopmentCost(manager().get<SkillCategoryData>(category_id), std::move(rm::game::character::SkillDevelopmentCost(cost)));
		}
	}

	ref.setSkillGroupProfessionBonuses(JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "skill-group-profession-bonuses", manager_));
	ref.setSkillGroupSpecialBonuses(JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "skill-group-special-bonuses", manager_));
	ref.setSkillGroupSkillDevelopmentTypes(JsonConverter::getDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(jsonObj, "skill-group-skill-development-types", manager_));
	ref.setSkillGroupSkillDevelopmentTypeChoices(JsonConverter::getDataChoiceEnumMap<SkillGroupData, SkillDevelopmentType::Type>(jsonObj, "skill-group-skill-development-type-choices", manager_));

	return ref;
}

} // namespace rm::rule::serial