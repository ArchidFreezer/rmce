#pragma once

#include <map>
#include <vector>
#include "BookData.h"
#include "DatafileParser.h"
#include "GameRuleDataCache.h"

class BookDatafileParser : public DatafileParser
{
public:
	BookDatafileParser(std::string_view filename, GameRuleDataCache& cache, FileFormat const filetype);
	BookDatafileParser(std::string_view filename, GameRuleDataCache& cache);
	void parse();
	void saveJson(const std::string& filename);
};