#include "DatafileParserXml.h"

void DatafileParserXml::read(const std::string& filename) {

	if (filename.empty()) throw FilenameNotSetException("You are attempting to read file a file without setting the filename first.");

	try {
		// Read the file and place the contents into the boost ptree if it is not already populated
		if (ptree().size() < 1)	pt::read_xml(filename, ptree(), pt::xml_parser::trim_whitespace);

		// Call the virtual method to parse the ptree into data objects
		parse();
	} catch (const pt::json_parser::json_parser_error& err) {
		std::cerr << err.what();
	}

}
