#include <iostream>
#include <filesystem>
#include "BookDatafileParser.h"
#include "LanguageCategoryDatafileParser.h"

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	// Create the cache to store the game data
	GameRuleDataCache cache{};
	
	BookDatafileParser books("../../../data/Books.json", cache, DatafileParser::kJson);
	books.read();
	books.save("../../../data/Books2.json");

	LanguageCategoryDatafileParser language_categories("../../../data/LanguageCategories.json", cache, DatafileParser::kJson);
	language_categories.read();
	language_categories.save("../../../data/LanguageCategories2.json");
	
	return 0;
}
