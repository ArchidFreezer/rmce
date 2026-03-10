#include <PersistentObjectSerializationManager.h>

namespace rm {

void PersistentObjectSerializationManager::load() {
	using namespace rm::rule::serial;
	deserializeAllObjects<rm::rule::AnimalData>("Animals.json", "animals");
	deserializeAllObjects<rm::rule::ArmourTypeData>("ArmourTypes.json", "armour-types");
	deserializeAllObjects<rm::rule::table::AttackTable>("AttackTables.json", "attack-tables");
	deserializeAllObjects<rm::rule::BookData>("Books.json", "books");
	deserializeAllObjects<rm::rule::ClimateData>("Climates.json", "climates");
	deserializeAllObjects<rm::rule::CreaturePaceData>("CreaturePaces.json", "creature-paces");
	deserializeAllObjects<rm::rule::CultureData>("Cultures.json", "cultures");
	deserializeAllObjects<rm::rule::CultureTypeData>("CultureTypes.json", "culture-types");
	deserializeAllObjects<rm::rule::DiseaseData>("Diseases.json", "diseases");
	deserializeAllObjects<rm::rule::DiseaseTypeData>("DiseaseTypes.json", "disease-types");
	deserializeAllObjects<rm::rule::LanguageData>("Languages.json", "languages");
	deserializeAllObjects<rm::rule::LanguageCategoryData>("LanguageCategories.json", "language-categories");
	deserializeAllObjects<rm::rule::PoisonData>("Poisons.json", "poisons");
	deserializeAllObjects<rm::rule::PoisonTypeData>("PoisonTypes.json", "poison-types");
	deserializeAllObjects<rm::rule::ProfessionData>("Professions.json", "professions");
	deserializeAllObjects<rm::rule::RaceData>("Races.json", "races");
	deserializeAllObjects<rm::rule::SkillData>("Skills.json", "skills");
	deserializeAllObjects<rm::rule::SkillCategoryData>("SkillCategories.json", "skill-categories");
	deserializeAllObjects<rm::rule::SkillGroupData>("SkillGroups.json", "skill-groups");
	deserializeAllObjects<rm::rule::SkillProgressionTypeData>("SkillProgressionTypes.json", "skill-progression-types");
	deserializeAllObjects<rm::rule::table::SpecialAttackTable>("SpecialAttackTables.json", "attack-tables");
	deserializeAllObjects<rm::rule::SpellListData>("SpellLists.json", "spell-lists");
	deserializeAllObjects<rm::rule::TrainingPackageData>("TrainingPackages.json", "training-packages");
	deserializeAllObjects<rm::rule::TreasureCodeData>("TreasureCodes.json", "treasure-codes");
	deserializeAllObjects<rm::rule::WeaponTypeData>("WeaponTypes.json", "weapon-types");
	deserializeTsv<rm::rule::table::TrainingPackageCostTable>("TrainingPackageCosts.tsv");
}

void PersistentObjectSerializationManager::save() {
	using namespace rm::rule::serial;
	serializeAllObjects<rm::rule::AnimalData>("Animals.json", "animals");
	serializeAllObjects<rm::rule::ArmourTypeData>("ArmourTypes.json", "armour-types");
	serializeAllObjects<rm::rule::table::AttackTable>("AttackTables.json", "attack-tables");
	serializeAllObjects<rm::rule::BookData>("Books.json", "books");
	serializeAllObjects<rm::rule::ClimateData>("Climates.json", "climates");
	serializeAllObjects<rm::rule::CreaturePaceData>("CreaturePaces.json", "creature-paces");
	serializeAllObjects<rm::rule::CultureData>("Cultures.json", "cultures");
	serializeAllObjects<rm::rule::CultureTypeData>("CultureTypes.json", "culture-types");
	serializeAllObjects<rm::rule::DiseaseData>("Diseases.json", "diseases");
	serializeAllObjects<rm::rule::DiseaseTypeData>("DiseaseTypes.json", "disease-types");
	serializeAllObjects<rm::rule::LanguageData>("Languages.json", "languages");
	serializeAllObjects<rm::rule::LanguageCategoryData>("LanguageCategories.json", "language-categories");
	serializeAllObjects<rm::rule::PoisonData>("Poisons.json", "poisons");
	serializeAllObjects<rm::rule::PoisonTypeData>("PoisonTypes.json", "poison-types");
	serializeAllObjects<rm::rule::ProfessionData>("Professions.json", "professions");
	serializeAllObjects<rm::rule::RaceData>("Races.json", "races");
	serializeAllObjects<rm::rule::SkillData>("Skills.json", "skills");
	serializeAllObjects<rm::rule::SkillCategoryData>("SkillCategories.json", "skill-categories");
	serializeAllObjects<rm::rule::SkillGroupData>("SkillGroups.json", "skill-groups");
	serializeAllObjects<rm::rule::SkillProgressionTypeData>("SkillProgressionTypes.json", "skill-progression-types");
	serializeAllObjects<rm::rule::table::SpecialAttackTable>("SpecialAttackTables.json", "attack-tables");
	serializeAllObjects<rm::rule::SpellListData>("SpellLists.json", "spell-lists");
	serializeAllObjects<rm::rule::TrainingPackageData>("TrainingPackages.json", "training-packages");
	serializeAllObjects<rm::rule::TreasureCodeData>("TreasureCodes.json", "treasure-codes");
	serializeAllObjects<rm::rule::WeaponTypeData>("WeaponTypes.json", "weapon-types");
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

} // namespace rm