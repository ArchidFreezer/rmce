#include <iostream>
#include <filesystem>
#include "BookDatafileParser.h"
#include "LanguageDatafileParser.h"
#include "LanguageCategoryDatafileParser.h"

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	// Create the cache to store the game data
	GameRuleDataCache cache{};
	
	BookDatafileParser books(cache, DatafileParser::kJson);
	books.read("../../../data/Books.json");
	books.save("../../../data/Books2.json");

	LanguageCategoryDatafileParser language_categories(cache, DatafileParser::kJson);
	language_categories.read("../../../data/LanguageCategories.json");
	language_categories.save("../../../data/LanguageCategories2.json");
	
	LanguageDatafileParser languages(cache, DatafileParser::kJson);
	languages.read("../../../data/Languages.json");
	languages.save("../../../data/Languages2.json");

	return 0;
}
