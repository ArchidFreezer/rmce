#include "DatafileParser.h"

DatafileParser::DatafileParser(GameRuleDataCache& cache, const std::string& datatype) :
	cache_{ cache },
	datatype_ { datatype }
{}

const std::string DatafileParser::getID(std::string_view type, std::string_view val) {
	std::stringstream ss{};

	// Add the chracters from the type
	bool lastAlpha { false };
	for (char c : type) {
		if (isalnum(c)) {
			lastAlpha = true;
			ss.put(toupper(c));
		}
		else if (lastAlpha){
			ss.put('_');
			lastAlpha = false;
		}
	}

	// Add the type/val separator
	if (lastAlpha) ss.put('_');

	// Add the chracters from the val
	lastAlpha = false;
	for (char c : val) {
		if (isalnum(c)) {
			lastAlpha = true;
			ss.put(toupper(c));
		}
		else if (lastAlpha) {
			ss.put('_');
			lastAlpha = false;
		}
	}

	// Convert to a string so we can use the pop_back method
	std::string ret = ss.str();
	// If the last character was not an alphanum then remove it
	if (!lastAlpha) ret.pop_back();

	return ret;
}

/*
void DatafileParser::save(const std::string& filename, FileFormat const filetype) {
	switch (filetype) {
	case DatafileParser::kXml:
	{
		pt::xml_writer_settings<std::string> settings('\t', 1);
		pt::write_xml(filename, ptree_, std::locale(), settings);
		break;
	}
	case DatafileParser::kJson:
		saveJson(filename);
		break;
	}

}
*/
