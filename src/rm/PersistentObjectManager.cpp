#include <sstream>
#include <PersistentObjectManager.h>
#include <GameRuleDatas.h>

namespace rm {
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

const std::set<std::string> PersistentObjectManager::getAllIds(std::string prefix) const {
	using namespace rm::rule;

	std::set<std::string> result;
	if (prefix == "ANIMAL") {
		cache_.keys<AnimalData>(result);
	} else if (prefix == "ARMOURTYPE") {
		cache_.keys<ArmourTypeData>(result);
	} else if (prefix == "ATTACKTABLE") {
		cache_.keys<AttackTable>(result);
	} else if (prefix == "BOOK") {
		cache_.keys<BookData>(result);
	} else if (prefix == "CLIMATE") {
		cache_.keys<ClimateData>(result);
	} else if (prefix == "CREATUREPACE") {
		cache_.keys<CreaturePaceData>(result);
	} else if (prefix == "CULTURE") {
		cache_.keys<CultureData>(result);
	} else if (prefix == "CULTURETYPE") {
		cache_.keys<CultureTypeData>(result);
	} else if (prefix == "DISEASE") {
		cache_.keys<DiseaseData>(result);
	} else if (prefix == "DISEASETYPE") {
		cache_.keys<DiseaseTypeData>(result);
	} else if (prefix == "LANGUAGECATEGORY") {
		cache_.keys<LanguageCategoryData>(result);
	} else if (prefix == "LANGUAGE") {
		cache_.keys<LanguageData>(result);
	} else if (prefix == "POISON") {
		cache_.keys<PoisonData>(result);
	} else if (prefix == "POISONTYPE") {
		cache_.keys<PoisonTypeData>(result);
	} else if (prefix == "PROFESSION") {
		cache_.keys<ProfessionData>(result);
	} else if (prefix == "RACE") {
		cache_.keys<RaceData>(result);
	} else if (prefix == "SKILLCATEGORY") {
		cache_.keys<SkillCategoryData>(result);
	} else if (prefix == "SKILLGROUP") {
		cache_.keys<SkillGroupData>(result);
	} else if (prefix == "SKILL") {
		cache_.keys<SkillData>(result);
	} else if (prefix == "SPECIALATTACKTABLE") {
		cache_.keys<SpecialAttackTable>(result);
	} else if (prefix == "SPELLLIST") {
		cache_.keys<SpellListData>(result);
	} else if (prefix == "TRAININGPACKAGE") {
		cache_.keys<TrainingPackageData>(result);
	} else if (prefix == "TREASURECODE") {
		cache_.keys<TreasureCodeData>(result);
	} else if (prefix == "WEAPONTYPE") {
		cache_.keys<WeaponTypeData>(result);
	} else {
		throw std::out_of_range("Could not determine the object type for objects with prefix " + prefix);
	}
	return result;
}

} // namespace rm