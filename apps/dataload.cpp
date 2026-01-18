#include <iostream>
#include <filesystem>
#include "BookDatafileParserJson.h"
#include "LanguageDatafileParserJson.h"
#include "LanguageCategoryDatafileParserJson.h"
#include "SkillProgressionTypeDatafileParserJson.h"
#include <AttackTableDatafileParserJson.h>

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	// Create the cache to store the game data
	GameRuleDataCache cache{};

	BookDatafileParserJson book_parser(cache, "../../../../data/Books.json");
	LanguageCategoryDatafileParserJson language_category_parser(cache, "../../../../data/LanguageCategories.json");
	LanguageDatafileParserJson language_parser(cache, "../../../../data/Languages.json");
	SkillProgressionTypeDatafileParserJson skill_progression_parser(cache, "../../../../data/SkillProgressionTypes.json");
	AttackTableDatafileParserJson attack_table_parser(cache, "../../../../data/AttackTables.json");

	// Store the parsers in a vector so we can iterate through them
	std::vector<DatafileParser*> parsers;
	parsers.push_back(&book_parser);
	parsers.push_back(&language_category_parser);
	parsers.push_back(&language_parser);
	parsers.push_back(&skill_progression_parser);
	parsers.push_back(&attack_table_parser);

	for (auto& parser : parsers) {
		parser->read(true);
	}
	for (auto& parser : parsers) {
		parser->read(false);
	}

	book_parser.save("../../../../data/Books2.json");
	language_category_parser.save("../../../../data/LanguageCategories2.json");
	language_parser.save("../../../../data/Languages2.json");
	skill_progression_parser.save("../../../../data/SkillProgressionTypes2.json");
	// We don't resave the attack tables as the nature of the objects means the rows are unordered so the files won't match

	return 0;
}
