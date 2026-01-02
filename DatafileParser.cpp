#include "DatafileParser.h"

DatafileParser::DatafileParser(std::string_view filename, DatafileParser::FileFormat const filetype) :
	datafile_{ filename },
	filetype_{ filetype }
{}

DatafileParser::DatafileParser(std::string_view filename) :	DatafileParser(filename, DatafileParser::kXml) {}

void DatafileParser::read() {
	if (datafile_.empty()) return;

	switch (filetype_) {
	case DatafileParser::kXml:
		pt::read_xml(datafile_, ptree_, pt::xml_parser::trim_whitespace);
		break;
	case DatafileParser::kJson:
		pt::read_json(datafile_, ptree_);
		break;
	}

	parse();
}

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