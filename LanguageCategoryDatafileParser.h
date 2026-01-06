#pragma once

#include <map>
#include <vector>
#include "LanguageCategoryData.h"
#include "DatafileParser.h"
#include "GameRuleDataCache.h"

class LanguageCategoryDatafileParser : public DatafileParser {
public:
	LanguageCategoryDatafileParser(std::string_view filename, GameRuleDataCache& cache, FileFormat const filetype);
	LanguageCategoryDatafileParser(std::string_view filename, GameRuleDataCache& cache);
	void parse();
	void save(const std::string& filename, FileFormat const filetype);
	void save(const std::string& filename);
	void saveJSON(const std::string& filename);
};