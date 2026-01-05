#pragma once

#include <map>
#include <vector>
#include "BookData.h"
#include "DatafileParser.h"
#include "GameRuleDataCache.h"

static const std::string kRuleDataName{ "Book" };

class BookDatafileParser : public DatafileParser
{
public:
	BookDatafileParser(std::string_view filename, GameRuleDataCache& cache, FileFormat const filetype);
	BookDatafileParser(std::string_view filename, GameRuleDataCache& cache);
	void parse();
	void save(const std::string& filename, FileFormat const filetype);
	void save(const std::string& filename);
	void saveJSON(const std::string& filename);
};