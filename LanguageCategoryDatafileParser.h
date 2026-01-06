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

protected:
	void parse();
	void saveJson(const std::string& filename);
};