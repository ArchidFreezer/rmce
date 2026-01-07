#pragma once

#include "DatafileParser.h"

class DatafileParserJson : public DatafileParser {
public:
	DatafileParserJson(GameRuleDataCache& cache, const std::string& datatype);
	virtual ~DatafileParserJson() = default;

	// Read game rule data from a json file, convert to objects and store in the game rule data cache
	void read(const std::string& filename);

protected:
	std::string root_node_{};
};