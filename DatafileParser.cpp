#include "DatafileParser.h"

DatafileParser::DatafileParser(std::string_view filename, DatafileParser::FileFormat const filetype) :
	m_datafile{ filename },
	m_filetype{ filetype }
{}

DatafileParser::DatafileParser(std::string_view filename) :	DatafileParser(filename, DatafileParser::xml) {}

void DatafileParser::read() {
	if (m_datafile.empty()) return;

	switch (m_filetype) {
	case DatafileParser::xml:
		pt::read_xml(m_datafile, m_ptree, pt::xml_parser::trim_whitespace);
		break;
	case DatafileParser::json:
		pt::read_json(m_datafile, m_ptree);
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