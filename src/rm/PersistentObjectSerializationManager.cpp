#include <PersistentObjectSerializationManager.h>

namespace rm {

// Helper function to determine the root key for a given type T based on its name
const std::string PersistentObjectSerializationManager::getRootKeyForType(std::string_view prefix) {
	if (prefix.ends_with("y")) {
		return std::string(prefix.substr(0, prefix.size() - 1)) + "ies";
	} else {
		return std::string(prefix) + "s";
	}
}



void PersistentObjectSerializationManager::load() {
	using namespace rm::rule::serial;
	deserializeAllObjects<rm::rule::AnimalData>("Animals.json", "animals");
	deserializeAllObjects<rm::rule::ArmourTypeData>("ArmourTypes.json", "armourtypes");
	deserializeAllObjects<rm::rule::table::AttackTable>("AttackTables.json", "attacktables");
	deserializeAllObjects<rm::rule::BookData>("Books.json", "books");
	deserializeAllObjects<rm::rule::ClimateData>("Climates.json", "climates");
	deserializeAllObjects<rm::rule::CreaturePaceData>("CreaturePaces.json", "creaturepaces");
	deserializeAllObjects<rm::rule::CultureData>("Cultures.json", "cultures");
	deserializeAllObjects<rm::rule::CultureTypeData>("CultureTypes.json", "culturetypes");
	deserializeAllObjects<rm::rule::DiseaseData>("Diseases.json", "diseases");
	deserializeAllObjects<rm::rule::DiseaseTypeData>("DiseaseTypes.json", "diseasetypes");
	deserializeAllObjects<rm::rule::LanguageData>("Languages.json", "languages");
	deserializeAllObjects<rm::rule::LanguageCategoryData>("LanguageCategories.json", "languagecategories");
	deserializeAllObjects<rm::rule::PoisonData>("Poisons.json", "poisons");
	deserializeAllObjects<rm::rule::PoisonTypeData>("PoisonTypes.json", "poisontypes");
	deserializeAllObjects<rm::rule::ProfessionData>("Professions.json", "professions");
	deserializeAllObjects<rm::rule::RaceData>("Races.json", "races");
	deserializeAllObjects<rm::rule::SkillData>("Skills.json", "skills");
	deserializeAllObjects<rm::rule::SkillCategoryData>("SkillCategories.json", "skillcategories");
	deserializeAllObjects<rm::rule::SkillGroupData>("SkillGroups.json", "skillgroups");
	deserializeAllObjects<rm::rule::SkillProgressionTypeData>("SkillProgressionTypes.json", "skillprogressiontypes");
	deserializeAllObjects<rm::rule::table::SpecialAttackTable>("SpecialAttackTables.json", "attacktables");
	deserializeAllObjects<rm::rule::SpellListData>("SpellLists.json", "spelllists");
	deserializeAllObjects<rm::rule::TrainingPackageData>("TrainingPackages.json", "trainingpackages");
	deserializeAllObjects<rm::rule::TreasureCodeData>("TreasureCodes.json", "treasurecodes");
	deserializeAllObjects<rm::rule::WeaponTypeData>("WeaponTypes.json", "weapontypes");
	deserializeTsv<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
}

void PersistentObjectSerializationManager::save() {
	using namespace rm::rule::serial;
	serializeAllObjects<rm::rule::AnimalData>("Animals.json", "animals");
	serializeAllObjects<rm::rule::ArmourTypeData>("ArmourTypes.json", "armourtypes");
	serializeAllObjects<rm::rule::table::AttackTable>("AttackTables.json", "attacktables");
	serializeAllObjects<rm::rule::BookData>("Books.json", "books");
	serializeAllObjects<rm::rule::ClimateData>("Climates.json", "climates");
	serializeAllObjects<rm::rule::CreaturePaceData>("CreaturePaces.json", "creaturepaces");
	serializeAllObjects<rm::rule::CultureData>("Cultures.json", "cultures");
	serializeAllObjects<rm::rule::CultureTypeData>("CultureTypes.json", "culturetypes");
	serializeAllObjects<rm::rule::DiseaseData>("Diseases.json", "diseases");
	serializeAllObjects<rm::rule::DiseaseTypeData>("DiseaseTypes.json", "diseasetypes");
	serializeAllObjects<rm::rule::LanguageData>("Languages.json", "languages");
	serializeAllObjects<rm::rule::LanguageCategoryData>("LanguageCategories.json", "languagecategories");
	serializeAllObjects<rm::rule::PoisonData>("Poisons.json", "poisons");
	serializeAllObjects<rm::rule::PoisonTypeData>("PoisonTypes.json", "poisontypes");
	serializeAllObjects<rm::rule::ProfessionData>("Professions.json", "professions");
	serializeAllObjects<rm::rule::RaceData>("Races.json", "races");
	serializeAllObjects<rm::rule::SkillData>("Skills.json", "skills");
	serializeAllObjects<rm::rule::SkillCategoryData>("SkillCategories.json", "skillcategories");
	serializeAllObjects<rm::rule::SkillGroupData>("SkillGroups.json", "skillgroups");
	serializeAllObjects<rm::rule::SkillProgressionTypeData>("SkillProgressionTypes.json", "skillprogressiontypes");
	serializeAllObjects<rm::rule::table::SpecialAttackTable>("SpecialAttackTables.json", "attacktables");
	serializeAllObjects<rm::rule::SpellListData>("SpellLists.json", "spelllists");
	serializeAllObjects<rm::rule::TrainingPackageData>("TrainingPackages.json", "trainingpackages");
	serializeAllObjects<rm::rule::TreasureCodeData>("TreasureCodes.json", "treasurecodes");
	serializeAllObjects<rm::rule::WeaponTypeData>("WeaponTypes.json", "weapontypes");
	serializeTsv<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
}

std::string PersistentObjectSerializationManager::serializeAnyObject(const std::string& id) {
	using namespace rm::rule::serial;
	// Get the object from the manager and determine its type, then call the appropriate serializer
	auto obj = object_manager_.getAny(id);
	if (auto animal = dynamic_cast<const rule::AnimalData*>(obj)) {
		return serializeObject(*animal);
	} else if (auto armour_type = dynamic_cast<const rule::ArmourTypeData*>(obj)) {
		return serializeObject(*armour_type);
	} else if (auto attack_table = dynamic_cast<const rule::table::AttackTable*>(obj)) {
		return serializeObject(*attack_table);
	} else if (auto book = dynamic_cast<const rule::BookData*>(obj)) {
		return serializeObject(*book);
	} else if (auto climate = dynamic_cast<const rule::ClimateData*>(obj)) {
		return serializeObject(*climate);
	} else if (auto creature_pace = dynamic_cast<const rule::CreaturePaceData*>(obj)) {
		return serializeObject(*creature_pace);
	} else if (auto culture = dynamic_cast<const rule::CultureData*>(obj)) {
		return serializeObject(*culture);
	} else if (auto culture_type = dynamic_cast<const rule::CultureTypeData*>(obj)) {
		return serializeObject(*culture_type);
	} else if (auto disease = dynamic_cast<const rule::DiseaseData*>(obj)) {
		return serializeObject(*disease);
	} else if (auto disease_type = dynamic_cast<const rule::DiseaseTypeData*>(obj)) {
		return serializeObject(*disease_type);
	} else if (auto language = dynamic_cast<const rule::LanguageData*>(obj)) {
		return serializeObject(*language);
	} else if (auto language_category = dynamic_cast<const rule::LanguageCategoryData*>(obj)) {
		return serializeObject(*language_category);
	} else if (auto poison = dynamic_cast<const rule::PoisonData*>(obj)) {
		return serializeObject(*poison);
	} else if (auto poison_type = dynamic_cast<const rule::PoisonTypeData*>(obj)) {
		return serializeObject(*poison_type);
	} else if (auto profession = dynamic_cast<const rule::ProfessionData*>(obj)) {
		return serializeObject(*profession);
	} else if (auto race = dynamic_cast<const rule::RaceData*>(obj)) {
		return serializeObject(*race);
	} else if (auto skill = dynamic_cast<const rule::SkillData*>(obj)) {
		return serializeObject(*skill);
	} else if (auto skill_category = dynamic_cast<const rule::SkillCategoryData*>(obj)) {
		return serializeObject(*skill_category);
	} else if (auto skill_group = dynamic_cast<const rule::SkillGroupData*>(obj)) {
		return serializeObject(*skill_group);
	} else if (auto skill_progression_type = dynamic_cast<const rule::SkillProgressionTypeData*>(obj)) {
		return serializeObject(*skill_progression_type);
	} else if (auto special_attack_table = dynamic_cast<const rule::table::SpecialAttackTable*>(obj)) {
		return serializeObject(*special_attack_table);
	} else if (auto spell_list = dynamic_cast<const rule::SpellListData*>(obj)) {
		return serializeObject(*spell_list);
	} else if (auto training_package = dynamic_cast<const rule::TrainingPackageData*>(obj)) {
		return serializeObject(*training_package);
	} else if (auto treasure_code = dynamic_cast<const rule::TreasureCodeData*>(obj)) {
		return serializeObject(*treasure_code);
	} else if (auto weapon_type = dynamic_cast<const rule::WeaponTypeData*>(obj)) {
		return serializeObject(*weapon_type);
	} else {
		throw std::runtime_error("Unknown object type for ID: " + id);
	}
}

std::string PersistentObjectSerializationManager::serializeAllObjects(std::string_view prefix) {
	using namespace rm::rule::serial;

	std::string lower_prefix = archid::lcase(prefix);
	std::string root_key = getRootKeyForType(lower_prefix);
	if (root_key.empty()) {
		throw std::runtime_error("Unknown type for serialization: " + std::string(prefix));
	}

	if (lower_prefix == "animal") {
		return serializeAllObjects_Impl<rm::rule::AnimalData>(root_key);
	} else if (lower_prefix == "armourtype") {
		return serializeAllObjects_Impl<rm::rule::ArmourTypeData>(root_key);
	} else if (lower_prefix == "attacktable") {
		return serializeAllObjects_Impl<rm::rule::table::AttackTable>(root_key);
	} else if (lower_prefix == "book") {
		return serializeAllObjects_Impl<rm::rule::BookData>(root_key);
	} else if (lower_prefix == "climate") {
		return serializeAllObjects_Impl<rm::rule::ClimateData>(root_key);
	} else if (lower_prefix == "creaturepace") {
		return serializeAllObjects_Impl<rm::rule::CreaturePaceData>(root_key);
	} else if (lower_prefix == "culture") {
		return serializeAllObjects_Impl<rm::rule::CultureData>(root_key);
	} else if (lower_prefix == "culturetype") {
		return serializeAllObjects_Impl<rm::rule::CultureTypeData>(root_key);
	} else if (lower_prefix == "disease") {
		return serializeAllObjects_Impl<rm::rule::DiseaseData>(root_key);
	} else if (lower_prefix == "diseasetype") {
		return serializeAllObjects_Impl<rm::rule::DiseaseTypeData>(root_key);
	} else if (lower_prefix == "language") {
		return serializeAllObjects_Impl<rm::rule::LanguageData>(root_key);
	} else if (lower_prefix == "languagecategory") {
		return serializeAllObjects_Impl<rm::rule::LanguageCategoryData>(root_key);
	} else if (lower_prefix == "poison") {
		return serializeAllObjects_Impl<rm::rule::PoisonData>(root_key);
	} else if (lower_prefix == "poisontype") {
		return serializeAllObjects_Impl<rm::rule::PoisonTypeData>(root_key);
	} else if (lower_prefix == "profession") {
		return serializeAllObjects_Impl<rm::rule::ProfessionData>(root_key);
	} else if (lower_prefix == "race") {
		return serializeAllObjects_Impl<rm::rule::RaceData>(root_key);
	} else if (lower_prefix == "skill") {
		return serializeAllObjects_Impl<rm::rule::SkillData>(root_key);
	} else if (lower_prefix == "skillcategory") {
		return serializeAllObjects_Impl<rm::rule::SkillCategoryData>(root_key);
	} else if (lower_prefix == "skillgroup") {
		return serializeAllObjects_Impl<rm::rule::SkillGroupData>(root_key);
	} else if (lower_prefix == "skillprogressiontype") {
		return serializeAllObjects_Impl<rm::rule::SkillProgressionTypeData>(root_key);
	} else if (lower_prefix == "specialattacktable") {
		return serializeAllObjects_Impl<rm::rule::table::SpecialAttackTable>(root_key);
	} else if (lower_prefix == "spelllist") {
		return serializeAllObjects_Impl<rm::rule::SpellListData>(root_key);
	} else if (lower_prefix == "trainingpackage") {
		return serializeAllObjects_Impl<rm::rule::TrainingPackageData>(root_key);
	} else if (lower_prefix == "treasurecode") {
		return serializeAllObjects_Impl<rm::rule::TreasureCodeData>(root_key);
	} else if (lower_prefix == "weapontype") {
		return serializeAllObjects_Impl<rm::rule::WeaponTypeData>(root_key);
	} else {
		throw std::runtime_error("Unknown type for serialization: " + std::string(prefix));
	}
}

const std::string PersistentObjectSerializationManager::deserializeObject(json::object& obj, std::string_view prefix) {
	using namespace rm::rule::serial;

	std::string lower_prefix = archid::lcase(prefix);

	const rm::rule::GameRuleData* obj_ptr = nullptr;

	if (lower_prefix == "animal") {
		obj_ptr = &deserializeObject<rm::rule::AnimalData>(obj);
	} else if (lower_prefix == "armourtype") {
		obj_ptr = &deserializeObject<rm::rule::ArmourTypeData>(obj);
	} else if (lower_prefix == "attacktable") {
		obj_ptr = &deserializeObject<rm::rule::table::AttackTable>(obj);
	} else if (lower_prefix == "book") {
		obj_ptr = &deserializeObject<rm::rule::BookData>(obj);
	} else if (lower_prefix == "climate") {
		obj_ptr = &deserializeObject<rm::rule::ClimateData>(obj);
	} else if (lower_prefix == "creaturepace") {
		obj_ptr = &deserializeObject<rm::rule::CreaturePaceData>(obj);
	} else if (lower_prefix == "culture") {
		obj_ptr = &deserializeObject<rm::rule::CultureData>(obj);
	} else if (lower_prefix == "culturetype") {
		obj_ptr = &deserializeObject<rm::rule::CultureTypeData>(obj);
	} else if (lower_prefix == "disease") {
		obj_ptr = &deserializeObject<rm::rule::DiseaseData>(obj);
	} else if (lower_prefix == "diseasetype") {
		obj_ptr = &deserializeObject<rm::rule::DiseaseTypeData>(obj);
	} else if (lower_prefix == "language") {
		obj_ptr = &deserializeObject<rm::rule::LanguageData>(obj);
	} else if (lower_prefix == "languagecategory") {
		obj_ptr = &deserializeObject<rm::rule::LanguageCategoryData>(obj);
	} else if (lower_prefix == "poison") {
		obj_ptr = &deserializeObject<rm::rule::PoisonData>(obj);
	} else if (lower_prefix == "poisontype") {
		obj_ptr = &deserializeObject<rm::rule::PoisonTypeData>(obj);
	} else if (lower_prefix == "profession") {
		obj_ptr = &deserializeObject<rm::rule::ProfessionData>(obj);
	} else if (lower_prefix == "race") {
		obj_ptr = &deserializeObject<rm::rule::RaceData>(obj);
	} else if (lower_prefix == "skill") {
		obj_ptr = &deserializeObject<rm::rule::SkillData>(obj);
	} else if (lower_prefix == "skillcategory") {
		obj_ptr = &deserializeObject<rm::rule::SkillCategoryData>(obj);
	} else if (lower_prefix == "skillgroup") {
		obj_ptr = &deserializeObject<rm::rule::SkillGroupData>(obj);
	} else if (lower_prefix == "skillprogressiontype") {
		obj_ptr = &deserializeObject<rm::rule::SkillProgressionTypeData>(obj);
	} else if (lower_prefix == "specialattacktable") {
		obj_ptr = &deserializeObject<rm::rule::table::SpecialAttackTable>(obj);
	} else if (lower_prefix == "spelllist") {
		obj_ptr = &deserializeObject<rm::rule::SpellListData>(obj);
	} else if (lower_prefix == "trainingpackage") {
		obj_ptr = &deserializeObject<rm::rule::TrainingPackageData>(obj);
	} else if (lower_prefix == "treasurecode") {
		obj_ptr = &deserializeObject<rm::rule::TreasureCodeData>(obj);
	} else if (lower_prefix == "weapontype") {
		obj_ptr = &deserializeObject<rm::rule::WeaponTypeData>(obj);
	} else {
		throw std::runtime_error("Unknown type for serialization: " + std::string(prefix));
	}
	return obj_ptr->id();
}

} // namespace rm