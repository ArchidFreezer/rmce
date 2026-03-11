#include <sstream>
#include <PersistentObjectManager.h>
#include <GameRuleDatas.h>

namespace rm {

const std::set<std::string> PersistentObjectManager::getAllPrefixes() const {
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

const std::string PersistentObjectManager::generateId(std::string_view type, std::string_view val) {
	std::stringstream ss;
	// Add the characters from the type, replacing non alphanum characters with an underscore
	// discarding consecutive underscores.
	bool last_alpha{false};
	for (char c : type) {
		if (iswalnum(c)) {
			last_alpha = true;
			ss.put(toupper(c));
		} else if (last_alpha) {
			ss.put('_');
			last_alpha = false;
		}
	}
	// Add the type/val separator
	if (last_alpha)
		ss.put('_');
	// Add the chracters from the val, replacing non alphanum characters with an underscore
	// discarding consecutive underscores.
	last_alpha = false;
	for (char c : val) {
		if (iswalnum(c)) {
			last_alpha = true;
			ss.put(toupper(c));
		} else if (last_alpha) {
			ss.put('_');
			last_alpha = false;
		}
	}
	// Convert to a string so we can use the pop_back method
	std::string ret = ss.str();
	// If the last character was not an alphanum then remove it
	if (!last_alpha)
		ret.pop_back();
	return ret;
}

const rm::rule::GameRuleData* PersistentObjectManager::getAny(std::string id) {
	using namespace rm::rule;

	const GameRuleData* result{nullptr};

	// We first need to determine the type of the object which is done be checking the prefix of the id.
	std::string prefix{id.substr(0, id.find('_'))};
	if (prefix == "ANIMAL") {
		result = &cache_.get<AnimalData>(id);
	} else if (prefix == "ARMOURTYPE") {
		result = &cache_.get<ArmourTypeData>(id);
	} else if (prefix == "ATTACKTABLE") {
		result = &cache_.get<AttackTable>(id);
	} else if (prefix == "BOOK") {
		result = &cache_.get<BookData>(id);
	} else if (prefix == "CLIMATE") {
		result = &cache_.get<ClimateData>(id);
	} else if (prefix == "CREATUREPACE") {
		result = &cache_.get<CreaturePaceData>(id);
	} else if (prefix == "CULTURE") {
		result = &cache_.get<CultureData>(id);
	} else if (prefix == "CULTURETYPE") {
		result = &cache_.get<CultureTypeData>(id);
	} else if (prefix == "DISEASE") {
		result = &cache_.get<DiseaseData>(id);
	} else if (prefix == "DISEASETYPE") {
		result = &cache_.get<DiseaseTypeData>(id);
	} else if (prefix == "LANGUAGECATEGORY") {
		result = &cache_.get<LanguageCategoryData>(id);
	} else if (prefix == "LANGUAGE") {
		result = &cache_.get<LanguageData>(id);
	} else if (prefix == "POISON") {
		result = &cache_.get<PoisonData>(id);
	} else if (prefix == "POISONTYPE") {
		result = &cache_.get<PoisonTypeData>(id);
	} else if (prefix == "PROFESSION") {
		result = &cache_.get<ProfessionData>(id);
	} else if (prefix == "RACE") {
		result = &cache_.get<RaceData>(id);
	} else if (prefix == "SKILLCATEGORY") {
		result = &cache_.get<SkillCategoryData>(id);
	} else if (prefix == "SKILLGROUP") {
		result = &cache_.get<SkillGroupData>(id);
	} else if (prefix == "SKILL") {
		result = &cache_.get<SkillData>(id);
	} else if (prefix == "SPECIALATTACKTABLE") {
		result = &cache_.get<SpecialAttackTable>(id);
	} else if (prefix == "SPELLLIST") {
		result = &cache_.get<SpellListData>(id);
	} else if (prefix == "TRAININGPACKAGE") {
		result = &cache_.get<TrainingPackageData>(id);
	} else if (prefix == "TREASURECODE") {
		result = &cache_.get<TreasureCodeData>(id);
	} else if (prefix == "WEAPONTYPE") {
		result = &cache_.get<WeaponTypeData>(id);
	} else {
		throw std::out_of_range("Could not determine the object type for object with id " + id);
	}

	return result;
}

const std::set<std::string> PersistentObjectManager::getAllIds(std::string_view prefix) const {
	using namespace rm::rule;

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
	} else if (lprefix == "climate") {
		cache_.keys<ClimateData>(result);
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