#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

class BookDatafileParserJson : public DatafileParserJson {
public:
	BookDatafileParserJson(GameRuleDataCache& cache);

	// Write book game rule data from the cache to a json file
	void save(const std::string& filename);

private:
	// Parse a boost::ptree containing the book rule data from a json file, convert to objects and store in the game rule data cache
	void parse();
};