#pragma once

#include <map>
#include <vector>
#include "BookData.h"
#include "DatafileParser.h"
#include "GameRuleDataCache.h"

class BookDatafileParser : public DatafileParser
{
public:
	BookDatafileParser(GameRuleDataCache& cache, FileFormat const filetype);
	BookDatafileParser(GameRuleDataCache& cache);

protected:
	void parse();
	void saveJson(const std::string& filename);
};