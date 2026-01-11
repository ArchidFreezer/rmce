#include "DatafileParserJson.h"

DatafileParserJson::DatafileParserJson(GameRuleDataCache& cache, const std::string& datatype) : DatafileParser(cache, datatype) {}

void DatafileParserJson::read(const std::string& filename) {
	if (filename.empty()) return;

	// Read the file and place the contents into the boost ptree
	pt::read_json(filename, ptree());

	// Call the virtual method to parse the ptree into data objects
	parse();
}

