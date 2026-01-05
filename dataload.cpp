#include <iostream>
#include <filesystem>
#include "BookDatafileParser.h"

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	// Create the cache to store the game data
	GameRuleDataCache cache{};
	
	BookDatafileParser books("../../../data/Books.json", cache, DatafileParser::kJson);
	books.read();
	books.saveJSON("../../../data/Books2.json");
	
	return 0;
}
