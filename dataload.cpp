#include <iostream>
#include <filesystem>
#include "BookDatafileParser.h"

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	BookDatafileParser books("../../../data/Books.json", DatafileParser::json);
	books.read();
	books.saveJSON("../../../data/Books2.json");
	return 0;
}
