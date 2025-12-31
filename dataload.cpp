#include <iostream>
#include <filesystem>
#include "BookDatafileParser.h"

int main()
{
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	
	/*
	BookDatafileParser books("../../../data/Books.xml");
	books.read();
	books.save("../../../data/Books2.xml", DatafileParser::xml);
	books.saveJSON("../../../data/Books1.json");
	*/

	BookDatafileParser books2("../../../data/Books.json", DatafileParser::json);
	books2.read();
	books2.saveJSON("../../../data/Books2.json");
	return 0;
}
