#pragma once

#include "DatafileParser.h"

class DatafileParserXml : DatafileParser {
public:
	void read(const std::string& filename);
	void save(const std::string& filename);
};