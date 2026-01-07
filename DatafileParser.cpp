#include "DatafileParser.h"

DatafileParser::DatafileParser(GameRuleDataCache& cache, const std::string& datatype) :
	cache_{ cache },
	rule_datatype_ { datatype }
{}

const std::string DatafileParser::generateId(std::string_view type, std::string_view val) {
	std::stringstream ss{};

	// Add the chracters from the type
	bool last_alpha { false };
	for (char c : type) {
		if (isalnum(c)) {
			last_alpha = true;
			ss.put(toupper(c));
		}
		else if (last_alpha){
			ss.put('_');
			last_alpha = false;
		}
	}

	// Add the type/val separator
	if (last_alpha) ss.put('_');

	// Add the chracters from the val
	last_alpha = false;
	for (char c : val) {
		if (isalnum(c)) {
			last_alpha = true;
			ss.put(toupper(c));
		}
		else if (last_alpha) {
			ss.put('_');
			last_alpha = false;
		}
	}

	// Convert to a string so we can use the pop_back method
	std::string ret = ss.str();
	// If the last character was not an alphanum then remove it
	if (!last_alpha) ret.pop_back();

	return ret;
}
