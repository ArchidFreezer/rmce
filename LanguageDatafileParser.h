#pragma once

#include <map>
#include <vector>
#include "LanguageData.h"
#include "DatafileParser.h"
#include "GameRuleDataCache.h"

class LanguageDatafileParser : public DatafileParser {
public:
	LanguageDatafileParser(GameRuleDataCache& cache, FileFormat const filetype);
	LanguageDatafileParser(GameRuleDataCache& cache);
protected:
	void parse();
	void saveJson(const std::string& filename);
};