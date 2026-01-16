#include <DatafileParser.h>
#include <GameRuleDataCache.h>

DatafileParser::DatafileParser(GameRuleDataCache& cache, std::string_view datatype, std::string_view filename) :
	cache_{ cache },
	rule_datatype_ { datatype },
	filename_ {filename}
{}

DatafileParser::DatafileParser(GameRuleDataCache& cache, std::string_view datatype) :
	DatafileParser(cache, datatype, "") {}
