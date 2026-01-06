#pragma once

#include <map>
#include <vector>
#include "LanguageCategoryData.h"
#include "DatafileParser.h"
#include "GameRuleDataCache.h"

class LanguageCategoryDatafileParser : public DatafileParser {
public:
	LanguageCategoryDatafileParser(GameRuleDataCache& cache, FileFormat const filetype);
	LanguageCategoryDatafileParser(GameRuleDataCache& cache);

protected:
	void parse();
	void saveJson(const std::string& filename);
};