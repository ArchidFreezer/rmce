#include "DatafileParserXml.h"

void DatafileParserXml::read(const std::string& filename) {
	if (filename.empty()) return;

	// Read the file and place the contents into the boost ptree
	pt::read_xml(filename, ptree(), pt::xml_parser::trim_whitespace);
	// Call the virtual method to parse the ptree into data objects
	parse();
}

void DatafileParserXml::save(const std::string& filename) {
	if (filename.empty()) return;

	// Configure the xml writer to use tabs corrcetly
	pt::xml_writer_settings<std::string> settings('\t', 1);
	// Write the data from the ptree into the xml file
	pt::write_xml(filename, ptree(), std::locale(), settings);
}

