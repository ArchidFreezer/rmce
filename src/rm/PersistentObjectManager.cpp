#include <sstream>
#include <PersistentObjectManager.h>
#include <GameRuleDatas.h>
#include <Character.h>

namespace rm {

const std::set<std::string> PersistentObjectManager::getAllDataPrefixes() const {
	std::set<std::string> result;
	// Much as we would ike to use the type information in the cache to get the prefixes, this is not possible as some objects, e.g. SubcategoriedSkillData, are stored in the cache with the same type as other objects, e.g. SkillData This
	// means we need to generate and maintain this list manually.
	result.insert("animal");
	result.insert("armourtype");
	result.insert("attacktable");
	result.insert("book");
	result.insert("climate");
	result.insert("creaturepace");
	result.insert("culture");
	result.insert("culturetype");
	result.insert("disease");
	result.insert("diseasetype");
	result.insert("foragable");
	result.insert("languagecategory");
	result.insert("language");
	result.insert("poison");
	result.insert("poisontype");
	result.insert("profession");
	result.insert("race");
	result.insert("skillcategory");
	result.insert("skillgroup");
	result.insert("skillprogressiontype");
	result.insert("skill");
	result.insert("specialattacktable");
	result.insert("spelllist");
	result.insert("trainingpackage");
	result.insert("treasurecode");
	result.insert("weapontype");
	return result;
}

const std::set<std::string> PersistentObjectManager::getAllObjectLabels() const {
	std::set<std::string> result;
	// Much as we would ike to use the type information in the cache to get the labels, this is not possible so we need to maintain this list manually.
	result.insert("character");
	return result;
}

const rm::game::GameObject* PersistentObjectManager::getAny(std::string_view id, std::string_view prefix) {
	using namespace rm::game;

	const GameObject* result{nullptr};

	// We first need to determine the type of the object which is done be checking the prefix of the id.
	std::string lprefix = archid::lcase(prefix);
	if (lprefix == "character") {
		result = &cache_.get<character::Character>(std::string(id));
	} else {
		throw std::out_of_range("Could not determine the object type for object with id " + std::string(id));
	}

	return result;
}

const rm::rule::GameRuleData* PersistentObjectManager::getAny(std::string id) {
	using namespace rm::rule;

	const GameRuleData* result{nullptr};

	// We first need to determine the type of the object which is done be checking the prefix of the id.
	std::string prefix = archid::lcase(id.substr(0, id.find('_')));
	if (prefix == "animal") {
		result = &cache_.get<AnimalData>(id);
	} else if (prefix == "armourtype") {
		result = &cache_.get<ArmourTypeData>(id);
	} else if (prefix == "attacktable") {
		result = &cache_.get<AttackTable>(id);
	} else if (prefix == "book") {
		result = &cache_.get<BookData>(id);
	} else if (prefix == "creaturepace") {
		result = &cache_.get<CreaturePaceData>(id);
	} else if (prefix == "culture") {
		result = &cache_.get<CultureData>(id);
	} else if (prefix == "culturetype") {
		result = &cache_.get<CultureTypeData>(id);
	} else if (prefix == "disease") {
		result = &cache_.get<DiseaseData>(id);
	} else if (prefix == "diseasetype") {
		result = &cache_.get<DiseaseTypeData>(id);
	} else if (prefix == "foragable") {
		result = &cache_.get<ForagableData>(id);
	} else if (prefix == "languagecategory") {
		result = &cache_.get<LanguageCategoryData>(id);
	} else if (prefix == "language") {
		result = &cache_.get<LanguageData>(id);
	} else if (prefix == "poison") {
		result = &cache_.get<PoisonData>(id);
	} else if (prefix == "poisontype") {
		result = &cache_.get<PoisonTypeData>(id);
	} else if (prefix == "profession") {
		result = &cache_.get<ProfessionData>(id);
	} else if (prefix == "race") {
		result = &cache_.get<RaceData>(id);
	} else if (prefix == "skillcategory") {
		result = &cache_.get<SkillCategoryData>(id);
	} else if (prefix == "skillgroup") {
		result = &cache_.get<SkillGroupData>(id);
	} else if (prefix == "skill") {
		result = &cache_.get<SkillData>(id);
	} else if (prefix == "skillprogressiontype") {
		result = &cache_.get<SkillProgressionTypeData>(id);
	} else if (prefix == "specialattacktable") {
		result = &cache_.get<SpecialAttackTable>(id);
	} else if (prefix == "spelllist") {
		result = &cache_.get<SpellListData>(id);
	} else if (prefix == "trainingpackage") {
		result = &cache_.get<TrainingPackageData>(id);
	} else if (prefix == "treasurecode") {
		result = &cache_.get<TreasureCodeData>(id);
	} else if (prefix == "weapontype") {
		result = &cache_.get<WeaponTypeData>(id);
	} else {
		throw std::out_of_range("Could not determine the object type for object with id " + id);
	}

	return result;
}

const std::set<std::string> PersistentObjectManager::getAllIds(std::string_view prefix) const {
	using namespace rm::rule;
	using namespace rm::game;

	std::string lprefix = archid::lcase(std::string(prefix));

	std::set<std::string> result;
	if (lprefix == "animal") {
		cache_.keys<AnimalData>(result);
	} else if (lprefix == "armourtype") {
		cache_.keys<ArmourTypeData>(result);
	} else if (lprefix == "attacktable") {
		cache_.keys<AttackTable>(result);
	} else if (lprefix == "book") {
		cache_.keys<BookData>(result);
	} else if (lprefix == "character") {
		cache_.keys<character::Character>(result);
	} else if (lprefix == "creaturepace") {
		cache_.keys<CreaturePaceData>(result);
	} else if (lprefix == "culture") {
		cache_.keys<CultureData>(result);
	} else if (lprefix == "culturetype") {
		cache_.keys<CultureTypeData>(result);
	} else if (lprefix == "disease") {
		cache_.keys<DiseaseData>(result);
	} else if (lprefix == "diseasetype") {
		cache_.keys<DiseaseTypeData>(result);
	} else if (lprefix == "foragable") {
		cache_.keys<ForagableData>(result);
	} else if (lprefix == "languagecategory") {
		cache_.keys<LanguageCategoryData>(result);
	} else if (lprefix == "language") {
		cache_.keys<LanguageData>(result);
	} else if (lprefix == "poison") {
		cache_.keys<PoisonData>(result);
	} else if (lprefix == "poisontype") {
		cache_.keys<PoisonTypeData>(result);
	} else if (lprefix == "profession") {
		cache_.keys<ProfessionData>(result);
	} else if (lprefix == "race") {
		cache_.keys<RaceData>(result);
	} else if (lprefix == "skillcategory") {
		cache_.keys<SkillCategoryData>(result);
	} else if (lprefix == "skillgroup") {
		cache_.keys<SkillGroupData>(result);
	} else if (lprefix == "skill") {
		cache_.keys<SkillData>(result);
	} else if (lprefix == "skillprogressiontype") {
		cache_.keys<SkillProgressionTypeData>(result);
	} else if (lprefix == "specialattacktable") {
		cache_.keys<SpecialAttackTable>(result);
	} else if (lprefix == "spelllist") {
		cache_.keys<SpellListData>(result);
	} else if (lprefix == "trainingpackage") {
		cache_.keys<TrainingPackageData>(result);
	} else if (lprefix == "treasurecode") {
		cache_.keys<TreasureCodeData>(result);
	} else if (lprefix == "weapontype") {
		cache_.keys<WeaponTypeData>(result);
	} else {
		throw std::out_of_range("Could not determine the object type for objects with prefix " + std::string(prefix));
	}

	// Remove any deleted objects from the result
	for (auto it = result.begin(); it != result.end();) {
		if (isDeleted(*it))
			it = result.erase(it);
		else
			++it;
	}
	return result;
	
}

rm::rule::SubcategoriedSkillData& PersistentObjectManager::subcategoriedSkillData(const rm::rule::SkillData& skill_data, std::optional<std::string_view> subcategory) {
	std::string id{skill_data.id() + (subcategory ? "_" + std::string(subcategory.value()) : "")};
	if (isDeleted(id))
		throw std::out_of_range("Object with id " + id + " has been deleted and cannot be retrieved.");

	if (cache_.exists<rm::rule::SubcategoriedSkillData>(id))
		return cache_.get<rm::rule::SubcategoriedSkillData>(id);
	if (subcategory)
		cache_.add<rm::rule::SubcategoriedSkillData>(std::move(std::make_unique<rm::rule::SubcategoriedSkillData>(skill_data, subcategory)));
	else
		cache_.add<rm::rule::SubcategoriedSkillData>(std::move(std::make_unique<rm::rule::SubcategoriedSkillData>(skill_data)));
	return cache_.get<rm::rule::SubcategoriedSkillData>(id);
}

} // namespace rm