#include "DatafileParserXml.h"

void DatafileParserXml::read(const std::string& filename) {
	if (filename.empty()) return;

	pt::read_xml(filename, ptree(), pt::xml_parser::trim_whitespace);
	parse();
}

void DatafileParserXml::save(const std::string& filename) {
	if (filename.empty()) return;

	pt::xml_writer_settings<std::string> settings('\t', 1);
	pt::write_xml(filename, ptree(), std::locale(), settings);
}

