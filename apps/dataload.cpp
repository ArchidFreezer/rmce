#include <filesystem>
#include <iostream>

#include <AnimalSerializer.h>
#include <ArmourTypeSerializer.h>
#include <AttackTableSerializer.h>
#include <BookSerializer.h>
#include <ClimateSerializer.h>
#include <CreaturePaceSerializer.h>
#include <CultureSerializer.h>
#include <CultureTypeSerializer.h>
#include <DiseaseSerializer.h>
#include <DiseaseTypeDatafileParserJson.h>
#include <FixedTableCreator.h>
#include <LanguageCategoryDatafileParserJson.h>
#include <LanguageDatafileParserJson.h>
#include <PersistentCache.h>
#include <PersistentObjectFileSerializer.h>
#include <PersistentObjectManager.h>
#include <PoisonDatafileParserJson.h>
#include <PoisonTypeDatafileParserJson.h>
#include <ProfessionDatafileParserXml.h>
#include <ProfessionDatefileParserJson.h>
#include <RaceDatafileParserJson.h>
#include <SkillCategoryDatafileParserJson.h>
#include <SkillDatafileParserJson.h>
#include <SkillGroupDatafileParserJson.h>
#include <SkillProgressionTypeDatafileParserJson.h>
#include <SpecialAttackTableDatafileParserJson.h>
#include <SpellListDatafileParserJson.h>
#include <TrainingPackageCostTableFileSerializer.h>
#include <TrainingPackageDatafileParserJson.h>
#include <TreasureCodeDatafileParserJson.h>
#include <WeaponTypeDatafileParserJson.h>

using namespace rm::rule::parser;

int main() {
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';

	// Create the cache to store the game data
	rm::PersistentCache cache{};
	rm::PersistentObjectManager object_manager{cache};

	PersistentObjectFileSerializer<AnimalSerializer> animal_serializer(object_manager, "../../../../data/Animals.json");
	PersistentObjectFileSerializer<ArmourTypeSerializer> armour_type_serializer(object_manager, "../../../../data/ArmourTypes.json");
	PersistentObjectFileSerializer<AttackTableSerializer> attack_table_serializer(object_manager, "../../../../data/AttackTables.json");
	PersistentObjectFileSerializer<BookSerializer> book_serializer(object_manager, "../../../../data/Books.json");
	PersistentObjectFileSerializer<ClimateSerializer> climate_serializer(object_manager, "../../../../data/Climates.json");
	PersistentObjectFileSerializer<CreaturePaceSerializer> creature_pace_serializer(object_manager, "../../../../data/CreaturePaces.json");
	PersistentObjectFileSerializer<CultureSerializer> culture_serializer(object_manager, "../../../../data/Cultures.json");
	PersistentObjectFileSerializer<CultureTypeSerializer> culture_type_serializer(object_manager, "../../../../data/CultureTypes.json");
	PersistentObjectFileSerializer<DiseaseSerializer> disease_serializer(object_manager, "../../../../data/Diseases.json");
	DiseaseTypeDatafileParserJson disease_type_parser(object_manager, "../../../../data/DiseaseTypes.json");
	LanguageCategoryDatafileParserJson language_category_parser(object_manager, "../../../../data/LanguageCategories.json");
	LanguageDatafileParserJson language_parser(object_manager, "../../../../data/Languages.json");
	PoisonDatafileParserJson poison_parser(object_manager, "../../../../data/Poisons.json");
	PoisonTypeDatafileParserJson poison_type_parser(object_manager, "../../../../data/PoisonTypes.json");
	ProfessionDatafileParserJson profession_parser(object_manager, "../../../../data/Professions.json");
	RaceDatafileParserJson race_parser(object_manager, "../../../../data/Races.json");
	SkillCategoryDatafileParserJson skill_category_parser(object_manager, "../../../../data/SkillCategories.json");
	SkillDatafileParserJson skill_parser(object_manager, "../../../../data/Skills.json");
	SkillGroupDatafileParserJson skill_group_parser(object_manager, "../../../../data/SkillGroups.json");
	SkillProgressionTypeDatafileParserJson skill_progression_parser(object_manager, "../../../../data/SkillProgressionTypes.json");
	SpecialAttackTableDatafileParserJson special_attack_table_parser(object_manager, "../../../../data/SpecialAttackTables.json");
	SpellListDatafileParserJson spell_list_parser(object_manager, "../../../../data/SpellLists.json");
	TrainingPackageCostTableFileSerializer training_package_cost_table(object_manager, "../../../../data/TrainingPackageCosts.tsv");
	TrainingPackageDatafileParserJson training_package_parser(object_manager, "../../../../data/TrainingPackages.json");
	TreasureCodeDatafileParserJson treasure_code_parser(object_manager, "../../../../data/TreasureCodes.json");
	WeaponTypeDatafileParserJson weapon_type_parser(object_manager, "../../../../data/WeaponTypes.json");

	// Store the parsers in a vector so we can iterate through them
	std::vector<DatafileParser*> parsers;
	parsers.push_back(&disease_type_parser);
	parsers.push_back(&language_category_parser);
	parsers.push_back(&language_parser);
	parsers.push_back(&poison_parser);
	parsers.push_back(&poison_type_parser);
	parsers.push_back(&profession_parser);
	parsers.push_back(&race_parser);
	parsers.push_back(&skill_category_parser);
	parsers.push_back(&skill_group_parser);
	parsers.push_back(&skill_parser);
	parsers.push_back(&skill_progression_parser);
	parsers.push_back(&special_attack_table_parser);
	parsers.push_back(&spell_list_parser);
	parsers.push_back(&training_package_parser);
	parsers.push_back(&treasure_code_parser);
	parsers.push_back(&weapon_type_parser);

	std::vector<PersistentObjectSerializer*> serializers;
	serializers.push_back(&animal_serializer);
	serializers.push_back(&armour_type_serializer);
	serializers.push_back(&attack_table_serializer);
	serializers.push_back(&book_serializer);
	serializers.push_back(&climate_serializer);
	serializers.push_back(&creature_pace_serializer);
	serializers.push_back(&culture_serializer);
	serializers.push_back(&culture_type_serializer);
	serializers.push_back(&disease_serializer);
	serializers.push_back(&training_package_cost_table);


	try {

		FixedTableCreator fixed_table_creator{object_manager};
		fixed_table_creator.createFixedTables();

		// Iterate through the parsers populating the cache with game data objects from the datafiles
		for (auto& parser : parsers) {
			parser->read();
		}

		for (auto& serializer : serializers) {
			serializer->read();
		}

		// disease_type_parser.save("../../../../data/DiseaseTypes2.json");
		// language_category_parser.save("../../../../data/LanguageCategories2.json");
		// language_parser.save("../../../../data/Languages2.json");
		// poison_parser.save("../../../../data/Poisons2.json");
		// poison_type_parser.save("../../../../data/PoisonTypes2.json");
		// profession_parser.save("../../../../data/Professions2.json");
		// race_parser.save("../../../../data/Races2.json");
		// skill_category_parser.save("../../../../data/SkillCategories2.json");
		// skill_group_parser.save("../../../../data/SkillGroups2.json");
		// skill_parser.save("../../../../data/Skills2.json");
		// skill_progression_parser.save("../../../../data/SkillProgressionTypes2.json");
		// special_attack_table_parser.save("../../../../data/SpecialAttackTables2.json");
		// spell_list_parser.save("../../../../data/SpellLists2.json");
		// training_package_parser.save("../../../../data/TrainingPackages2.json");
		// treasure_code_parser.save("../../../../data/TreasureCodes2.json");
		// weapon_type_parser.save("../../../../data/WeaponTypes2.json");

		animal_serializer.save("../../../../data2/Animals.json");
		armour_type_serializer.save("../../../../data2/ArmourTypes.json");
		attack_table_serializer.save("../../../../data2/AttackTables.json");
		book_serializer.save("../../../../data2/Books.json");
		climate_serializer.save("../../../../data2/Climates.json");
		creature_pace_serializer.save("../../../../data2/CreaturePaces.json");
		culture_serializer.save("../../../../data2/Cultures.json");
		culture_type_serializer.save("../../../../data2/CultureTypes.json");
		disease_serializer.save("../../../../data2/Diseases.json");
		disease_type_parser.save("../../../../data2/DiseaseTypes.json");
		language_category_parser.save("../../../../data2/LanguageCategories.json");
		language_parser.save("../../../../data2/Languages.json");
		poison_parser.save("../../../../data2/Poisons.json");
		poison_type_parser.save("../../../../data2/PoisonTypes.json");
		profession_parser.save("../../../../data2/Professions.json");
		race_parser.save("../../../../data2/Races.json");
		skill_category_parser.save("../../../../data2/SkillCategories.json");
		skill_group_parser.save("../../../../data2/SkillGroups.json");
		skill_parser.save("../../../../data2/Skills.json");
		skill_progression_parser.save("../../../../data2/SkillProgressionTypes.json");
		special_attack_table_parser.save("../../../../data2/SpecialAttackTables.json");
		spell_list_parser.save("../../../../data2/SpellLists.json");
		training_package_cost_table.save("../../../../data2/TrainingPackageCosts.tsv");
		training_package_parser.save("../../../../data2/TrainingPackages.json");
		treasure_code_parser.save("../../../../data2/TreasureCodes.json");
		weapon_type_parser.save("../../../../data2/WeaponTypes.json");

	} catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
