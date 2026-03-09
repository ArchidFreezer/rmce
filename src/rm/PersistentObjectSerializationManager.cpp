#include <PersistentObjectSerializationManager.h>
#include <TrainingPackageCostTableSerializer.h>

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

	TrainingPackageCostTableSerializer tsv_serializer(object_manager_);
	TsvFileSerializer<rm::rule::table::TrainingPackageCostTable> tp_cost_file_serializer(tsv_serializer, (data_directory_ + "TrainingPackageCosts.tsv"));
	tp_cost_file_serializer.load();
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

	TrainingPackageCostTableSerializer tsv_serializer(object_manager_);
	TsvFileSerializer<rm::rule::table::TrainingPackageCostTable> tp_cost_file_serializer(tsv_serializer, (data_directory_ + "TrainingPackageCosts.tsv"));
	tp_cost_file_serializer.save();
}

} // namespace rm