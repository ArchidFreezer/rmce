#include <iostream>
#include <filesystem>
#include <BookDatafileParserJson.h>
#include <LanguageDatafileParserJson.h>
#include <LanguageCategoryDatafileParserJson.h>
#include <SkillProgressionTypeDatafileParserJson.h>
#include <AttackTableDatafileParserJson.h>
#include <SpecialAttackTableDatafileParserJson.h>
#include <SpellListDatafileParserJson.h>
#include <SkillCategoryDatafileParserJson.h>
#include <ArmourTypeDatafileParserJson.h>
#include <SkillDatafileParserJson.h>
#include <WeaponTypeDatafileParserJson.h>
#include <RaceDatafileParserJson.h>
#include <ProfessionDatafileParserXml.h>
#include <ProfessionDatefileParserJson.h>
#include <SkillGroupDatafileParserJson.h>

int main() {
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';

	// Create the cache to store the game data
	GameRuleDataCache cache{};

	BookDatafileParserJson book_parser(cache, "../../../../data/Books.json");
	LanguageCategoryDatafileParserJson language_category_parser(cache, "../../../../data/LanguageCategories.json");
	LanguageDatafileParserJson language_parser(cache, "../../../../data/Languages.json");
	SkillProgressionTypeDatafileParserJson skill_progression_parser(cache, "../../../../data/SkillProgressionTypes.json");
	AttackTableDatafileParserJson attack_table_parser(cache, "../../../../data/AttackTables.json");
	SpecialAttackTableDatafileParserJson special_attack_table_parser(cache, "../../../../data/SpecialAttackTables.json");
	SpellListDatafileParserJson spell_list_parser(cache, "../../../../data/SpellLists.json");
	SkillCategoryDatafileParserJson skill_category_parser(cache, "../../../../data/SkillCategories.json");
	ArmourTypeDatafileParserJson armour_type_parser(cache, "../../../../data/ArmourTypes.json");
	SkillDatafileParserJson skill_parser(cache, "../../../../data/Skills.json");
	WeaponTypeDatafileParserJson weapon_type_parser(cache, "../../../../data/WeaponTypes.json");
	RaceDatafileParserJson race_parser(cache, "../../../../data/Races.json");
	SkillGroupDatafileParserJson skill_group_parser(cache, "../../../../data/SkillGroups.json");
	ProfessionDatafileParserJson profession_parser(cache, "../../../../data/Professions.json");
	ProfessionDatafileParserXml profession_parser_xml(cache, "../../../../data/Professions.xml");

	// Store the parsers in a vector so we can iterate through them
	std::vector<DatafileParser*> parsers;
	parsers.push_back(&book_parser);
	parsers.push_back(&language_category_parser);
	parsers.push_back(&language_parser);
	parsers.push_back(&skill_progression_parser);
	parsers.push_back(&attack_table_parser);
	parsers.push_back(&special_attack_table_parser);
	parsers.push_back(&spell_list_parser);
	parsers.push_back(&skill_category_parser);
	parsers.push_back(&armour_type_parser);
	parsers.push_back(&skill_parser);
	parsers.push_back(&weapon_type_parser);
	parsers.push_back(&race_parser);
	parsers.push_back(&skill_group_parser);
	parsers.push_back(&profession_parser_xml);

	try {
		// Iterate through the parsers retrieving the ID only and populating the cache with empty game data objects
		for (auto& parser : parsers) {
			parser->read();
		}


//		book_parser.save("../../../../data/Books2.json");
//		language_category_parser.save("../../../../data/LanguageCategories2.json");
//		language_parser.save("../../../../data/Languages2.json");
//		skill_progression_parser.save("../../../../data/SkillProgressionTypes2.json");
//		attack_table_parser.save("../../../../data/AttackTables2.json");
		special_attack_table_parser.save("../../../../data/SpecialAttackTables2.json");
//		spell_list_parser.save("../../../../data/SpellLists2.json");
//		skill_category_parser.save("../../../../data/SkillCategories2.json");
//		armour_type_parser.save("../../../../data/ArmourTypes2.json");
//		skill_parser.save("../../../../data/Skills2.json");
//		weapon_type_parser.save("../../../../data/WeaponTypes2.json");
//		race_parser.save("../../../../data/Races2.json");
//		skill_group_parser.save("../../../../data/SkillGroups2.json");
//		profession_parser.save("../../../../data/Professions2.json");
	} catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
