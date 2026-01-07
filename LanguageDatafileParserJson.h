#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

class LanguageDatafileParserJson : public DatafileParserJson {
public:
	LanguageDatafileParserJson(GameRuleDataCache& cache);

	// Write language game rule data from the cache to a json file
	void save(const std::string& filename);

private:
	// Parse a boost::ptree containing the language rule data from a json file, convert to objects and store in the game rule data cache
	void parse();
};