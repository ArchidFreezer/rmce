#include "DatafileParserJson.h"
#include <iostream>

DatafileParserJson::DatafileParserJson(GameRuleDataCache& cache, std::string_view datatype, std::string_view filename) : DatafileParser(cache, datatype, filename) {}

DatafileParserJson::DatafileParserJson(GameRuleDataCache& cache, std::string_view datatype) : DatafileParser(cache, datatype) {}

void DatafileParserJson::read(const std::string& filename, bool id_only) {

	if (filename.empty()) throw FilenameNotSetException("You are attempting to read file a file without setting the filename first.");

	try {
		// Read the file and place the contents into the boost ptree if it is not already populated
		if (ptree().size() < 1)	pt::read_json(filename, ptree());
		
		// Call the virtual method to parse the ptree into data objects
		parse(id_only);
	} catch (const pt::json_parser::json_parser_error& err) {
		std::cerr << err.what();
	}

}

