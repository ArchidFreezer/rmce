#include "DatafileParserJson.h"

DatafileParserJson::DatafileParserJson(GameRuleDataCache& cache, const std::string& datatype) : DatafileParser(cache, datatype) {}

void DatafileParserJson::read(const std::string& filename) {
	if (filename.empty()) return;

	pt::read_json(filename, ptree());
	parse();
}

