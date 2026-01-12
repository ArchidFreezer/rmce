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

	// Store the parsers in a vector so we can iterate through them
	std::vector<std::unique_ptr<DatafileParser>> parsers;
	parsers.push_back(std::move(std::make_unique<BookDatafileParserJson>(cache, "../../../../data/Books.json")));
	parsers.push_back(std::move(std::make_unique<LanguageCategoryDatafileParserJson>(cache, "../../../../data/LanguageCategories.json")));
	parsers.push_back(std::move(std::make_unique<LanguageDatafileParserJson>(cache, "../../../../data/Languages.json")));

	for (auto& parser : parsers) {
		parser->read(true);
	}
	for (auto& parser : parsers) {
		parser->read(false);
	}

	BookDatafileParserJson books(cache);
	books.setFilename("../../../../data/Books2.json");
	books.save();

	LanguageCategoryDatafileParserJson language_categories(cache);
	language_categories.save("../../../../data/LanguageCategories2.json");
	
	LanguageDatafileParserJson languages(cache);
	languages.save("../../../../data/Languages2.json");

	return 0;
}
