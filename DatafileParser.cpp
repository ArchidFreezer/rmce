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