#include <iostream>
#include <filesystem>
#include "BookDatafileParserJson.h"
#include "LanguageDatafileParserJson.h"
#include "LanguageCategoryDatafileParserJson.h"

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	// Create the cache to store the game data
	GameRuleDataCache cache{};
	
	BookDatafileParserJson books(cache);
	books.read("../../../../data/Books.json");
	books.save("../../../../data/Books2.json");

	LanguageCategoryDatafileParserJson language_categories(cache);
	language_categories.read("../../../../data/LanguageCategories.json");
	language_categories.save("../../../../data/LanguageCategories2.json");
	
	LanguageDatafileParserJson languages(cache);
	languages.read("../../../../data/Languages.json");
	languages.save("../../../../data/Languages2.json");

	return 0;
}
